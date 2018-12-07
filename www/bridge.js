function platformize() {
  handleClientLoad();
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

function textToAbstractSyntaxTree(text, onSuccess, onError) {
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

function uploadFile(mup, text, onSuccess) {
  var boundary = '-------314159265358979323846';
  var delimiter = "\r\n--" + boundary + "\r\n";
  var close_delim = "\r\n--" + boundary + "--";

  var metadata = { 
    description : 'Madeup source file',
    mimeType: 'application/json'
  };  

  var multipartRequestBody =
    delimiter +  'Content-Type: application/json\r\n\r\n' +
    JSON.stringify(metadata) +
    delimiter + 'Content-Type: application/json\r\n\r\n' +
    text +
    close_delim;

  gapi.client.request({ 
    'path': '/upload/drive/v3/files/' + mup.driveID,
    'method': 'PATCH',
    'params': {
      fileId: mup.driveID,
      uploadType: 'multipart'
    },
    'headers': {
      'Content-Type': 'multipart/form-data; boundary="' + boundary + '"'
    },
    'body': multipartRequestBody 
  }).execute(function(file) {
    $('#message').html('I saved your program. It is precious! Find it later under <image src="images/gear.png" id="gear-in-console" width="' + settings.get('fontSize') + 'pt"> / Mups / ' + mup.name + '.');
    onSuccess();
  }); 
}

function platformSave(mup, source, mode, onSuccess) {
  var file = {
    'name' : mup.name,
    'mode' : mode,
    'updated_at' : new Date().toString(),
    'source' : source
  };
  var json = JSON.stringify(file);
  console.log("json:", json);

  if (isGoogled) {
    return gapi.client.drive.files.list({
      q: "name = '" + mup.name + "' and trashed = false and '" + driveDirectory.id + "' in parents"
    }).then(function(files) {
      var matches = files.result.files;
      if (matches.length && matches.length > 0) {
				mup.driveID = matches[0].id;
        uploadFile(mup, json, onSuccess);
      } else {
        gapi.client.drive.files.create({
          name: mup.name,
          parents: [driveDirectory.id],
          mimeType: 'text/plain'
        }).then(function(res) {
					mup.driveID = res.result.id;
          uploadFile(mup, json, function() {
            populateMupsList();
            onSuccess();
          });
        });
      }
    });
  } else {
    localStorage.setItem(mup.name, json);
    $('#message').html('I saved your program. It is precious! Find it later under <image src="images/gear.png" id="gear-in-console" width="' + settings.get('fontSize') + 'pt"> / Mups / ' + mup.name + '.');
    populateMupsList();
    onSuccess();
  }
}

function getData(url) {
  xmlhttp = new XMLHttpRequest();
  xmlhttp.onreadystatechange = function() {
    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
      console.log(xmlhttp.responseText);
    }
  }
  xmlhttp.open('GET', url, true);
  // var myToken = gapi.auth.getToken();
  // xmlhttp.setRequestHeader('Authorization', 'Bearer ' + myToken.access_token);
  console.log(gapi.auth2.getAuthInstance().currentUser.get().getAuthResponse().access_token);
  xmlhttp.setRequestHeader('Authorization', 'Bearer ' + gapi.auth2.getAuthInstance().currentUser.get().getAuthResponse().access_token);
  console.log("xmlhttp:", xmlhttp);
  xmlhttp.send();
}

function platformLoad(mup, onSuccess) {
  if (isGoogled && mup.name != 'untitled') {
    // Thanks to https://developers.google.com/drive/v3/web/manage-downloads. Ugh.
    promise = gapi.client.drive.files.get({
      fileId: mup.driveID,
      alt: 'media'
    }).then(function(response) {
      onSuccess(response.result.source);
    });
  } else {
    var record = localStorage.getItem(mup.name);
    if (record == null) {
      var source = '';
    } else {
      var source = JSON.parse(record).source;
    }
    onSuccess(source);
  }
}

function platformPromptForSaveAs(onSuccess) {
  ask('Save under what name?', function(name) {
    onSuccess(name);
    populateMupsList();
  });
}

function populateMupsList() {
  var promise = null;
  if (isGoogled) {
    promise = gapi.client.drive.files.list({
      q: "'" + driveDirectory.id + "' in parents and trashed = false",
      corpora: 'user',
      pageSize: 100,
      fields: "nextPageToken, files(id, name, modifiedTime)"
    }).then(function(response) {
      var files = response.result.files;
      var mups = [];
      if (files && files.length > 0) {
        for (var i = 0; i < files.length; i++) {
          var file = files[i];
          mups.push(new Mup(file.name, Date.parse(file.modifiedTime), file.id));
        }
      }
      return mups;
    });
  } else {
    promise = new Promise(function(resolve, reject) {
      var mups = [];
      for (var i = 0; i < localStorage.length; ++i) {
        var key = localStorage.key(i);
        if (key != 'untitled' && key != 'settings') {
          var record = JSON.parse(localStorage.getItem(key));
          var mup = new Mup(key, record.updated_at);
          mups.push(mup);
        }
      }
      resolve(mups);
    });
  }

  promise.then(function(mups) {
    mups.sort(getSorter());

    var list = $('#mups')[0];
    list.innerHTML = '';

    mups.forEach(function(mup) {
      if (mup.name != 'untitled') {
        var anchor = document.createElement('a');
        anchor.style.cursor = 'pointer'; // no href, so cursor isn't default
        anchor.onclick = function() {
          load(mup);
        };
        anchor.appendChild(document.createTextNode(mup.name));
        list.appendChild(anchor);
        list.appendChild(document.createElement('br'));
      }
    });
  });
}

function getSorter() {
  if (settings.get('sortMupsBy') == 'date') {
    return function(mup1, mup2) {
      return Date.parse(mup2.modifiedTime) - Date.parse(mup1.modifiedTime);
    };
  } else {
    return function(mup1, mup2) {
      return mup1.name.charCodeAt(0) - mup2.name.charCodeAt(0);
    };
  }
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
    if (mup && mup.name && mup.isDirty && confirm('Save changes to ' + mup.name + '?')) {
      save();
    }
    load(new Mup('untitled'));
  });

  $('#fileDelete').click(function() {
    var ok = confirm('Delete ' + mup.name + '?');
    if (ok) {
      if (blocklyWorkspace) {
        clearWorkspace();
        // blocklyWorkspace.clear();
        // blocklyWorkspace.updateVariableStore();
      }

      var promise = null;
      if (isGoogled && mup.name != 'untitled') {
        promise = gapi.client.drive.files.delete({
          fileId: mup.driveID
        });
      } else {
        promise = new Promise(function(resolve, reject) {
          localStorage.removeItem(mup.name);
          resolve();
        });
      }

      promise.then(function() {
        populateMupsList();
        load(new Mup('untitled'));
      });
    }
  });

  $('input[type=radio][name=sortMupsBy]').change(function() {
    settings.set('sortMupsBy', $(this).val());
    populateMupsList();
  });

});

var CLIENT_ID = '1044882582652-7g4d00clc613n2ahn48neumroauv7tu2.apps.googleusercontent.com';
var DISCOVERY_DOCS = ["https://www.googleapis.com/discovery/v1/apis/drive/v3/rest"];
var SCOPES = [
  'https://www.googleapis.com/auth/drive',
  'https://www.googleapis.com/auth/drive.file',
].join(' ');
var isGoogled = false;
var driveDirectory = null;

function handleClientLoad() {
  gapi.load('client:auth2', {
    callback: initClient,
    onerror: onDriveFailed
  });
}

function onDriveFailed(e) {
  console.log(e);
  updateGoogleStatus(false);
}

function initClient() {
  gapi.client.init({
    discoveryDocs: DISCOVERY_DOCS,
    clientId: CLIENT_ID,
    scope: SCOPES
  }).then(function(err) {
    // Handle the initial sign-in state.
		updateGoogleStatus(gapi.auth2.getAuthInstance().isSignedIn.get());

    $('#storageLocal').click(function() {
      gapi.auth2.getAuthInstance().signOut().then(function() {
        updateGoogleStatus(false);
      }, onDriveFailed);
    });

    $('#storageDrive').click(function() {
      gapi.auth2.getAuthInstance().signIn().then(function() {
        updateGoogleStatus(true);
      }, onDriveFailed);
    });
  }, onDriveFailed);
}

function updateGoogleStatus(isConnected) {
	if (isConnected) {
		$('#googleLogin').text('Use local storage');
    $('#archiver').hide();
    $('#storageDrive').prop('checked', true);
		appFolder().then(function(parentDirectory) {
      isGoogled = true;
      driveDirectory = parentDirectory;
      populateMupsList();
    }, function() {
      isGoogled = false;
      updateGoogleStatus(false);
    });
	} else {
    // Signing out seems to mean something different than I expect. We want to
    // completely disconnect from Google Drive when we're doing local storage.
    if (gapi.auth2.getAuthInstance().isSignedIn.get()) {
      gapi.auth2.getAuthInstance().disconnect();
    }

		$('#googleLogin').text('Use Google Drive');
    $('#archiver').show();
    $('#storageLocal').prop('checked', true);
		isGoogled = false;
		populateMupsList();
	}
}

function appFolder() {
  return gapi.client.drive.files.list({
    q: "name = 'mups' and mimeType = 'application/vnd.google-apps.folder' and trashed = false"
  }).then(function(files) {
    var dir = files.result.files;
    if (!dir.length) {
      return gapi.client.drive.files.create({
        name: 'mups',
        mimeType: 'application/vnd.google-apps.folder'
      }).then(function(res) {
        return res.result;
      });
    } else {
      return dir[0];
    }
  });
}
