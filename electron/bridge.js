var tmp = require('tmp');
var fs = require('fs');
var Dialogs = require('dialogs');
var dialogs = Dialogs();
const screenshot = require('electron-screenshot');
const fsdialog = require('electron').remote.dialog;
const LocalStorage = require('node-localstorage').LocalStorage;
let localStorage = new LocalStorage(require('os').homedir() + '/.madeup');

function interpret(options, onSuccess, onError) {
  var tmpIn = tmp.fileSync();
  var tmpOut = tmp.fileSync({postfix: '.' + options.extension});

  fs.writeFileSync(tmpIn.name, options.source, function(err) {
    if (err) {
      return console.log(err);
    } else {
      console.log("The file was saved!");
    }
  }); 

  const
    spawn = require('child_process').spawn,
    merp = spawn('../build/merp', ['--timeout', '10', '--shading', options.shading_mode, '--geometry', options.geometry_mode, '-o', tmpOut.name, tmpIn.name]);

  out = {};
  out.stdout = '';
  out.stderr = '';
  out.timestamp = options.timestamp;
  out.geometry_mode = options.geometry_mode;

  merp.stdout.on('data', data => {
    out.stdout += data.toString();
  });

  merp.stderr.on('data', data => {
    out.stderr += data.toString();
  });

  merp.on('close', code => {
    if (code == 0) {
      out.exit_status = 0;
      out.model = fs.readFileSync(tmpOut.name).toString();
      onSuccess(out);
    } else {
      // console.log(`child process exited with code ${code}`);
      out.stdout = out.stderr;
      out.exit_status = code;
      onSuccess(out);
    }
  });
}

function textToBlocks(text, onSuccess, onError) {
  var tmpIn = tmp.fileSync();

  fs.writeFileSync(tmpIn.name, text, function(err) {
    if (err) {
      return console.log(err);
    } else {
      console.log("The file was saved!");
    }
  }); 

  const
    spawn = require('child_process').spawn,
    merp = spawn('../build/merp', ['--tree', '-q', tmpIn.name]);

  out = {};
  out.tree = null;
  out.stdout = '';

  merp.stdout.on('data', data => {
    out.tree = data.toString();
  });

  merp.stderr.on('data', data => {
    console.log(`stderr: ${data}`);
  });

  merp.on('close', code => {
    if (code != 0) {
      console.log(`child process exited with code ${code}`);
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
