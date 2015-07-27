<?php

if (isset($_REQUEST['source'])) {
  $in = $_REQUEST;
} else {
  $json = file_get_contents("php://input");
  $in = json_decode($json, true);
}

$back = array();
$back['input'] = $in['source'];

$mode = $in['mode'];
/* error_log("MODE MODE MODE: $mode"); */

// The interpreter will write the geometry out to a file when it finishes 
// interpreting. Let's make that file a temporary one.
$out_path = tempnam(sys_get_temp_dir(), 'mupout');
rename($out_path, $out_path . '.' . $in['extension']);
$out_path .= '.' . $in['extension'];
$back['out_path'] = $out_path;

// Write source to a file, 'cuz that's what the interpreter expects to read 
// input from.
$in_path = tempnam(sys_get_temp_dir(), 'mupin');
$back['in_path'] = $in_path;
file_put_contents($in_path, $in['source']);
error_log(json_encode($back));

$shading_mode = $in['shadingMode'];

// Execute the interpreter and collect its output.
$lines = array();
exec("./forn 15 ./merp --timeout 10 --shading $shading_mode -o $out_path --geometry $mode $in_path 2>&1", $lines, $back['exit_status']);
foreach ($lines as $line) {
  error_log("lineline: $line");
}
$back['output'] = implode("\n", $lines);
error_log(json_encode($back));

if ($in['extension'] === 'obj') {
  header($_SERVER["SERVER_PROTOCOL"] . " 200 OK");
  header("Cache-Control: public"); // needed for i.e.
  header("Content-Type: application/x-please-download-me");
  header("Content-Transfer-Encoding: Binary");
  header("Content-Length:" . filesize($out_path));
  header("Content-Disposition: attachment; filename=madeup.obj");
  error_log("FILE: $out_path");
  error_log("CONTENTS: " . readfile($out_path));
  readfile($out_path);
} else {
  error_log("EXIT: {$back['exit_status']}");
  if ($back['exit_status'] == 0) {
    $back['model'] = file_get_contents($out_path);
  } else {
    $back['model'] = '';
  }
  echo json_encode($back);
  error_log(json_encode($back));
  error_log("AFTER EXIT: {$back['exit_status']}");
}

error_log(json_encode($back));

if (file_exists($out_path)) {
  $ok = unlink($out_path);
}
$ok = unlink($in_path);

?>
