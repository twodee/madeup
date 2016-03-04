<?php

if (isset($_REQUEST['source'])) {
  $in = $_REQUEST;
} else {
  $json = file_get_contents("php://input");
  $in = json_decode($json, true);
}

// $in is a dictionary we get from the client with the following allowable 
// key-value pairs:
//   source: The Madeup source code to interpret.

// $out is a dictionary we send back to the client with the following 
// key-value pairs:
//   exit_status: the exit value of the interpreter
//   stdout: the interpreter's stdout
//   tree: the Lisp model of the program
$out = array();

// Write uploaded Madeup source to a file, because that's how the interpreter 
// expects to get input.
$in_path = tempnam(sys_get_temp_dir(), 'mupin');
file_put_contents($in_path, $in['source']);

error_log("Madeup source: {$in['source']}");
error_log("Input file: $in_path");

$lines = array();
$command = sprintf("./merp --tree -q %s", $in_path);
exec($command, $lines, $out['exit_status']);
$out['tree'] = implode("\n", $lines);

echo json_encode($out);

// Clean up the temporary files we created for the interpreter.
if (file_exists($in_path)) {
  $ok = unlink($in_path);
}

error_log("Results: " . json_encode($out));
?>
