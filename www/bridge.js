function platformize() {
  populateMupsList();
}

function interpret(options, onSuccess, onError) {
  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify(options),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: onSuccess,
    error: onError
  });
}

function textToBlocks(text, onSuccess, onError) {
  $.ajax({
    type: 'POST',
    url: 'translate.php',
    data: JSON.stringify({source: text}),
    dataType: 'json',
    contentType: 'application/json; charset=utf-8',
    success: onSuccess,
    error: onError
  });
}

function ask(question, onAnswer) {
  var answer = prompt(question);
  if (answer != null) {
    onAnswer(answer);
  }
}

function sendToChris(text) {
  var name = prompt("What's your name?");
  if (name != null) {
    $.ajax({
      type: 'POST',
      url: 'save.php',
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
  }
}

function configureDownloader() {
  $('#downloader').attr('action', 'interpret.php');
  $('#downloader').attr('method', 'post');
}

function takeScreenshot(path) {
  console.log('not implemented');
}

function needsUnsavedPrompt() {
  return true;
}

function platformSave(mup, source, mode, onSuccess) {
  var file = {
    'mup' : mup,
    'mode' : mode,
    'updated_at' : new Date().toString(),
    'source' : source
  };
  localStorage.setItem(mup, JSON.stringify(file));
  $('#message').html('I saved your program. It is precious! Find it later under <image src="images/gear.png" id="gear-in-console" width="' + settings.get('fontSize') + 'pt"> / Mups / ' + mup + '.');
  onSuccess();
}

function platformLoad(mup, onSuccess) {
  var source = JSON.parse(localStorage.getItem(mup)).source;
  onSuccess(source);
}

function platformPromptForSaveAs(onSuccess) {
  ask('Save under what name?', function(name) {
    onSuccess(name);
    populateMupsList();
  });
}

function populateMupsList() {
  var list = '';

  var keys = [];
  for (var i = 0; i < localStorage.length; ++i) {
    keys.push(localStorage.key(i));
  }

  if (settings.get('sortMupsBy') == 'date') {
    keys.sort(function(key1, key2) {
      var mup1 = JSON.parse(localStorage.getItem(key1));
      var mup2 = JSON.parse(localStorage.getItem(key2));
      return Date.parse(mup2.updated_at) - Date.parse(mup1.updated_at);
    });
  } else {
    keys = keys.sort();
  }

  keys.forEach(function(key) {
    if (key != 'untitled' && key != 'settings') {
      list += '<a href="#" class="menu-link" onclick="load(\'' + key.replace(/'/g, '\\&#39;').replace(/"/g, '\\&quot;') + '\')">' + key + '</a><br/>';
    }
  });

  $('#mups').html(list);
}

$(document).ready(function() {
  $('#fileSaveAs').click(promptForSaveAs);
  $('#fileSave').click(save);

  $('#exportArchive').click(function() {
    var archive = new Object;
    for (var i = 0; i < localStorage.length; ++i) {
      var key = localStorage.key(i);
      var value = localStorage.getItem(key);
      if (key != 'untitled' && key != 'settings') {
        archive[key] = JSON.parse(value);
      }
    }

    generateDownloadable('mups_archive_' + yyyymmdd() + '.json', JSON.stringify(archive));
  });

  $('#importForm').hide();
  $('#importArchive').click(function() {
    $('#importForm').show();
  });
  $('#cancelImport').click(function() {
    $('#importForm').hide();
  });
  $('#archive').change(function() {
    var archive = this.files[0];
    $('#importForm').hide();

    var reader = new FileReader();
    reader.onload = function(e) {
      var mups = JSON.parse(e.target.result);
      for (mup in mups) {
        localStorage.setItem(mup, JSON.stringify(mups[mup]));
        populateMupsList();
      }
    };
    reader.readAsText(archive);
  });

  $('#fileClose').click(function() {
    if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
      save();
    }
    load('untitled');
  });

  $('#fileDelete').click(function() {
    var ok = confirm('Delete ' + mupName + '?');
    if (ok) {
      if (blocklyWorkspace) {
        blocklyWorkspace.clear();
        blocklyWorkspace.updateVariableList();
      }
      localStorage.removeItem(mupName);
      populateMupsList();
      mupName = null;
      load('untitled');
    }
  });

  $('input[type=radio][name=sortMupsBy]').change(function() {
    settings.set('sortMupsBy', $(this).val());
    populateMupsList();
  });

});
