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
