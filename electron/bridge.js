var tmp = require('tmp');
var fs = require('fs');
var Dialogs = require('dialogs');
var dialogs = Dialogs();
var screenshot = require('electron-screenshot');
var fsdialog = require('electron').remote.dialog;
var LocalStorage = require('node-localstorage').LocalStorage;
var localStorage = new LocalStorage(require('os').homedir() + '/.madeup');

function platformize() {
  $('#panel-section-file').remove();  
  $('#panel-section-mups').remove();  
}

function interpret(options, onSuccess, onError) {
  var tmpIn = tmp.fileSync();
  var tmpOut = tmp.fileSync({postfix: '.' + options.extension});

  fs.writeFileSync(tmpIn.name, options.source);

  var
    spawn = require('child_process').spawn,
    merp = spawn('../build/merp', ['--timeout', '10', '--shading', options.shading_mode, '--geometry', options.geometry_mode, '-o', tmpOut.name, tmpIn.name]);

  var out = {};
  out.stdout = '';
  out.stderr = '';
  out.timestamp = options.timestamp;
  out.geometry_mode = options.geometry_mode;

  merp.stdout.on('data', function(data) {
    out.stdout += data.toString();
  });

  merp.stderr.on('data', function(data) {
    out.stderr += data.toString();
  });

  merp.on('close', function(code) {
    if (code == 0) {
      out.exit_status = 0;
      out.model = fs.readFileSync(tmpOut.name).toString();
      onSuccess(out);
    } else {
      out.stdout = out.stderr;
      out.exit_status = code;
      onSuccess(out);
    }
  });
}

function textToBlocks(text, onSuccess, onError) {
  var tmpIn = tmp.fileSync();

  fs.writeFileSync(tmpIn.name, text);

  var
    spawn = require('child_process').spawn,
    merp = spawn('../build/merp', ['--tree', '-q', tmpIn.name]);

  var out = {};
  out.tree = null;
  out.stdout = '';

  merp.stdout.on('data', function(data) {
    out.tree = data.toString();
  });

  merp.stderr.on('data', function(data) {
    console.log("stderred: " + data);
  });

  merp.on('close', function(code) {
    if (code != 0) {
      onError(out);
    } else {
      out.exit_status = 0;
      onSuccess(out);
    }
  });
}

function ask(question, onAnswer) {
  dialogs.prompt(question, function(answer) {
    onAnswer(answer);
  });
}

function sendToChris(text) {
  dialogs.prompt("What's your name?", function(name) {
    $.ajax({
      type: 'POST',
      url: 'http://madeup.xyz/save.php',
      data: JSON.stringify({
        name: name,
        source: text
      }),
      contentType: 'application/json; charset=utf-8',
      success: function(data) {
        console.log("Saved!");
      },
      error: function(message) {
        console.log('Failure. :(');
        console.log(message);
      }
    });
  });
}

function configureDownloader() {
  var form = $('#downloader')[0];

  $('#downloader').submit(function(e) {
    isDownloading = true;
    var defaultPath = form.tag.value + '.obj';
    interpret({
      timestamp: form.timestamp.value,
      source: form.source.value,
      tag: form.tag.value,
      extension: form.extension.value,
      geometry_mode: form.geometry_mode.value,
      shading_mode: form.shading_mode.value
    }, function(data) {
      fsdialog.showSaveDialog({
        title: 'Save as...',
        defaultPath: defaultPath
      }, function(path) {
        fs.writeFile(path, data.model, function(e) {
          isDownloading = false;
        });
      });
    }, function(errorMessage) {
      console.log('Failure. :(');
      isDownloading = false;
    });

    return false; // suppress page reload
  });
}

function takeScreenshot(path) {
  screenshot({filename: path});
}

function needsUnsavedPrompt() {
  return false;
}

function writeMup(path, source, onSuccess) {
  fs.writeFile(path, source, function(e) {
    console.log(e);
    onSuccess();
    $('#message').html('I saved your program at ' + path + '. It is precious!');
    onSuccess();
  });
}

function platformSave(path, source, mode, onSuccess) {
  if (path == 'untitled') {
    var file = {
      'mup' : path,
      'mode' : mode,
      'updated_at' : new Date().toString(),
      'source' : source
    };
    localStorage.setItem(path, JSON.stringify(file));
  } else {
    writeMup(path, source, onSuccess);
  }
}

function platformPromptForSaveAs(onSuccess) {
  fsdialog.showSaveDialog({
    title: 'Save as...',
  }, function(path) {
    onSuccess(path);
  });
}

function platformOpen() {
  fsdialog.showOpenDialog({
    title: 'Open...',
  }, function(path) {
    if (path) {
      load(path[0]);    
    }
  });
}

function platformLoad(path, onSuccess) {
  var source = null;
  if (path == 'untitled') {
    source = JSON.parse(localStorage.getItem(path)).source;
  } else {
    source = fs.readFileSync(path).toString();
  }
  onSuccess(source);
}

require('electron').ipcRenderer.on('open', function(event, data) {
  platformOpen();
});

require('electron').ipcRenderer.on('save', function(event, data) {
  save();
});

require('electron').ipcRenderer.on('saveAs', function(event, data) {
  promptForSaveAs();
});
