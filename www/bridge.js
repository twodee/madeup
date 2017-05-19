console.log('asdfasdfadsfasd');

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
