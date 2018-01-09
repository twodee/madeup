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

  lesson = 'top';
}

function interpret(options, onSuccess, onError) {
  var tmpIn = tmp.fileSync();
  var tmpOut = tmp.fileSync({postfix: '.' + options.extension});

  fs.writeFileSync(tmpIn.name, options.source);

  var
    spawn = require('child_process').spawn,
    merp = spawn('../build/merp', ['--timeout', '100', '--shading', options.shading_mode, '--geometry', options.geometry_mode, '-o', tmpOut.name, tmpIn.name]);

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

function textToAbstractSyntaxTree(text, onSuccess, onError) {
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
}

  // var form = $('#downloader')[0];

  // $('#downloader').submit(function(e) {
    // isDownloading = true;
    // var defaultPath = form.tag.value + '.obj';
    // interpret({
      // timestamp: form.timestamp.value,
      // source: form.source.value,
      // tag: form.tag.value,
      // extension: form.extension.value,
      // geometry_mode: form.geometry_mode.value,
      // shading_mode: form.shading_mode.value
    // }, function(data) {
      // fsdialog.showSaveDialog({
        // title: 'Save as...',
        // defaultPath: defaultPath
      // }, function(path) {
        // fs.writeFile(path, data.model, function(e) {
          // isDownloading = false;
        // });
      // });
    // }, function(errorMessage) {
      // console.log('Failure. :(');
      // isDownloading = false;
    // });

    // return false; // suppress page reload
  // });
// }

function takeScreenshot(path) {
  screenshot({filename: path});
}

function needsUnsavedPrompt() {
  return false;
}

function writeMup(path, source, onSuccess) {
  fs.writeFile(path, source, function(e) {
    log('I saved your program at ' + mup.name + '. It is precious!');
    onSuccess();
  });
}

function platformSave(mup, source, mode, onSuccess) {
  if (mup.name == 'untitled') {
    var file = {
      'mup' : mup.name,
      'mode' : mode,
      'updated_at' : new Date().toString(),
      'source' : source
    };
    localStorage.setItem(mup.name, JSON.stringify(file));
    onSuccess();
  } else {
    writeMup(mup.name, source, onSuccess);
  }
}

function platformPromptForSaveAs(onSuccess) {
  fsdialog.showSaveDialog({
    title: 'Save as...',
  }, function(path) {
    onSuccess(path);
  });
}

function platformLoad(mup, onSuccess) {
  var source = null;
  if (mup.name == 'untitled') {
    source = JSON.parse(localStorage.getItem(mup.name)).source;
  } else {
    source = fs.readFileSync(mup.name).toString();
  }
  onSuccess(source);
}

require('electron').ipcRenderer.on('save', function(event, data) {
  save();
});

require('electron').ipcRenderer.on('saveAs', function(event, data) {
  promptForSaveAs();
});

require('electron').ipcRenderer.on('open', function(event, path) {
  load(new Mup(path));
});

function export3D(extension) {
  return function(event, data) {
    interpret({
      timestamp: new Date().getTime(),
      source: getSource(),
      extension: extension,
      geometry_mode: GeometryMode.SURFACE,
      shading_mode: settings.get('isFlatShaded') ? 'FLAT' : 'SMOOTH'
    }, function(data) {
      console.log(data);
      fsdialog.showSaveDialog({
        title: 'Save ' + extension.toUpperCase() + ' as...',
        defaultPath: mup.name.replace(/\.[^.]*$/, '') + '.' + extension
      }, function(path) {
        if (path) {
          fs.writeFileSync(path, data.model);
        }
      });
    }, function(errorMessage) {
      console.log('Failure. :(');
    });
  };
}

require('electron').ipcRenderer.on('exportOBJ', export3D('obj'));
require('electron').ipcRenderer.on('exportSTL', export3D('stl'));

require('electron').ipcRenderer.on('screenshot', function(event, data) {
  exportScreenshot();
});
