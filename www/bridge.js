function interpret(options, onSuccess) {
  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify(options),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: onSuccess
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

function sendToChris(text) {
  var name = prompt("What's your name?");
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
