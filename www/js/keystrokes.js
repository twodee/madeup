var ABSOLUTE_FRAME_PERIOD = 10;

function patch(preLines, patchLines) {
  var iPreLine = 0;
  var iPatchLine = 0;
  var postLines = new Array();

  // Read and apply the patch, one transaction at a time. Since not every
  // line may be altered by the transactions, before we apply a transaction,
  // we make sure all the lines previous to the lines the transaction affects
  // get copied over from pre to post.
  while (iPatchLine < patchLines.length) {
    var pattern = /(\d+)(?:,(\d+))?([acd])(\d+)(?:,(\d+))?/;
    var result = patchLines[iPatchLine].match(pattern);

    var command = result[3];
    var preStart = result[1] - 1;
    var preEnd = result[2] == undefined ? preStart : result[2] - 1;
    var postStart = result[4] - 1;
    var postEnd = result[5] == undefined ? postStart : result[5] - 1;

    for (; iPreLine < preStart; ++iPreLine) {
      postLines.push(preLines[iPreLine]);
    }

    if (command == "c") {
      iPatchLine += (preEnd - preStart + 1) + 2;
      for (var i = postStart; i <= postEnd; ++i) {
        postLines.push(patchLines[iPatchLine].substring(2));
        ++iPatchLine;
      }
    } else if (command == "a") {
      ++iPatchLine; // advance past the #a# line
      if (iPreLine <= preStart) {
        postLines.push(preLines[iPreLine]);
        ++iPreLine;
      }
      for (var i = postStart; i <= postEnd; ++i) {
        postLines.push(patchLines[iPatchLine].substring(2));
        ++iPatchLine;
      }
    } else if (command == "d") {
      iPatchLine += (preEnd - preStart + 1) + 1;
    } else {
      console.log("uh oh. bad command: " + command);
    }

    iPreLine = preEnd + 1;
  }

  for (; iPreLine < preLines.length; ++iPreLine) {
    postLines.push(preLines[iPreLine]);
  }

  return postLines;
}

function KeystrokesFrame(selectionStartLine, selectionStartColumn, selectionEndLine, selectionEndColumn, timestamp, lines) {
  this.selectionStartLine = selectionStartLine;
  this.selectionStartColumn = selectionStartColumn;
  this.selectionEndLine = selectionEndLine;
  this.selectionEndColumn = selectionEndColumn;
  this.timestamp = timestamp;
  this.lines = lines;
}

KeystrokesFrame.prototype.getText = function() {
  return this.getAbsolute().lines.join('\n');
}

KeystrokesFrame.prototype.getCursor = function(line, column) {
  // Our index will be at least the column we're in.
  console.log("Cursor for " + line + "," + column);
  var cursor = 0;

  // It will also reflect the number of lines we've shot past. Advance the
  // cursor forward based on the length of each line. We must include the line
  // terminator.
  var lengthOfTerminator = 1;
  for (var r = 0; r < line - 1 && r < this.lines.length; ++r) {
    cursor += this.lines[r].length + lengthOfTerminator;
    console.log('so far: ' + cursor + ' from adding ' + this.lines[r]);
  }
  cursor += column;
  console.log("cursor is " + cursor);

  return cursor;
}

function KeystrokesMovie(dictionary) {
  // Convert all the frames in the dictionary movie into real frames.
  this.frames = new Array();
  for (var i = 0; i < dictionary.frames.length; ++i) {
    var f = dictionary.frames[i];

    var frame = new KeystrokesFrame(f.selectionStartLine, f.selectionStartColumn, f.selectionEndLine, f.selectionEndColumn, f.timestamp, f.lines);
    if (i == 0) {
      frame.getAbsolute = function() {return this;}
    } else {
      frame.predecessor = this.frames[i - 1];
      frame.getAbsolute = function() {
        var preLines = this.predecessor.getAbsolute().lines;
        var postLines = patch(preLines, this.lines);
        var absoluteFrame = new KeystrokesFrame(this.selectionStartLine, this.selectionStartColumn, this.selectionEndLine, this.selectionEndColumn, this.timestamp, postLines);
        absoluteFrame.getAbsolute = function() {return this;}
        return absoluteFrame;
      }
    }

    if (i % ABSOLUTE_FRAME_PERIOD == 0) {
      frame = frame.getAbsolute();
    }

    this.frames.push(frame);
  }

  // Make a map from seconds to frame indices for fast scrubbing and playback lookup.
  this.secondsToFrameIDs = new Array();
  for (var i = this.frames.length - 1; i >= 0; --i) {
    var secondsIn = Math.ceil(this.frames[i].timestamp);
    //console.log(i + " -> " + this.frames[i].timestamp + " -> " + secondsIn);
    this.secondsToFrameIDs[secondsIn] = i;
  }

  // Next, replicate the known frames at other seconds.
  var lastSeen = 0;
  for (var i = 0; i < this.secondsToFrameIDs.length; ++i) {
    if (this.secondsToFrameIDs[i] == undefined) {
      this.secondsToFrameIDs[i] = lastSeen;
    } else {
      lastSeen = this.secondsToFrameIDs[i];
    }
  }
  //console.log(this.secondsToFrameIDs.join());
}

KeystrokesMovie.prototype.getFrameIndexAtSeconds = function(nSeconds) {
  // We start by looking in seconds map. We walk along from there, proceeding
  // along until we hit a frame that's past the specified time or we're out of
  // frames.
  var root = this.secondsToFrameIDs[Math.floor(nSeconds)];
  var next = root + 1;
  //console.log("nseconds: " + nSeconds);
  while (next < this.frames.length && this.frames[next].timestamp < nSeconds) {
    ++root;
    next = root + 1;
  }
  //console.log("root: " + root);
  return root;
}

KeystrokesMovie.prototype.getFrameAtSeconds = function(nSeconds) {
  return this.frames[this.getFrameIndexAtSeconds(nSeconds)];
}

// ----------------------------------------------------------------------------

var scrubber = undefined;
// var textEditor = undefined;
var movie = undefined;

function show(nseconds) {
  var frame = movie.getFrameAtSeconds(nseconds).getAbsolute();
  textEditor.setValue(frame.getText(), 1);
  if (frame.selectionStartLine == frame.selectionEndLine && frame.selectionStartColumn == frame.selectionEndColumn) {
    textEditor.moveCursorTo(frame.selectionStartLine - 1, frame.selectionStartColumn - 1);
  } else {
    textEditor.getSelection().setSelectionRange(new Range(frame.selectionStartLine - 1, frame.selectionStartColumn - 1, frame.selectionEndLine - 1, frame.selectionEndColumn));
  }
  textEditor.focus();
}

$(document).ready(function() {
  if (keystrokesMoviePrefix === null) {
    return;
  }

  scrubber = $('#scrubber')[0];

  $.getJSON(keystrokesMoviePrefix + '.json', function(data) {
    movie = new KeystrokesMovie(data);
  });

  $('#scrubber').bind('seeking', function() {
    show(scrubber.currentTime);
  });

  var periodicID;
  $('#scrubber').bind('play', function() {
    periodicID = window.setInterval(function() {
      show(scrubber.currentTime);
    }, 100);
  });

  $('#scrubber').bind('pause', function() {
    window.clearInterval(periodicID);
  });
});
