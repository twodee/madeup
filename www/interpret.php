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
//   shading_mode: One of the shading modes recognized by the interpreter.
//   geometry_mode: One of the geometry modes recognized by the interpreter.
//   extension: One of json or obj.
//   tag: The name of the model used for exporting.

// $out is a dictionary we send back to the client with the following 
// key-value pairs:
//   exit_status: the exit value of the interpreter
//   stdout: the interpreter's stdout
//   model: the JSON representation of the model if in.extension is json,
//      otherwise undefined
$out = array();

// The interpreter checks that shading_mode and geometry_mode are legal.
// But we should check that the output path is legal -- that the extension
// we are given hasn't been tampered with.
if (!(strcmp($in['extension'], 'json') == 0 || strcmp($in['extension'], 'obj') == 0)) {
  $out['exit_status'] = 1;  
  $out['stdout'] = "I don't know {$in['extension']}. I only know how to output JSON and OBJ.";
  echo json_encode($out);
} else {
  // The interpreter will write the geometry out to a file when it finishes 
  // interpreting. Let's make that file a temporary one.
  $out_path = tempnam(sys_get_temp_dir(), 'mupout');

  // The client may be requesting json output (for Three.JS) or obj output. The 
  // interpreter determines what kind of output to give based on the output 
  // file's extension, which we have to append manually.
  rename($out_path, $out_path . '.' . $in['extension']);
  $out_path = "$out_path.{$in['extension']}";

  // Write uploaded Madeup source to a file, because that's how the interpreter 
  // expects to get input.
  $in_path = tempnam(sys_get_temp_dir(), 'mupin');
  file_put_contents($in_path, $in['source']);

  error_log("Madeup source: {$in['source']}");
  error_log("Input file: $in_path");
  error_log("Output file: $out_path");

  // Execute the interpreter and collect its output.
  $lines = array();
  $command = sprintf("./forn 15 ./merp --timeout 10 --shading %s -o %s --geometry %s %s 2>&1",
                     escapeshellarg($in['shading_mode']),
                     $out_path,
                     escapeshellarg($in['geometry_mode']),
                     $in_path);
  exec($command, $lines, $out['exit_status']);
  $out['stdout'] = implode("\n", $lines);

  if ($in['extension'] === 'obj') {
    // We generate a file named after the model. But let's play it
    // safe and remove all the alphanumeric junk they try to feed
    // us.
    $tag = preg_replace("/\W/", '', $in['tag']);

    // If nothing's left, we default to madeup.obj.
    if (strcmp($tag, '') == 0) {
      $tag = 'madeup';
    }

    header($_SERVER["SERVER_PROTOCOL"] . " 200 OK");
    header("Cache-Control: public"); // needed for IE
    header("Content-Type: application/x-please-download-me");
    header("Content-Transfer-Encoding: Binary");
    header("Content-Length:" . filesize($out_path));
    header("Content-Disposition: attachment; filename=$tag.obj");
    readfile($out_path);
  } else {
    if ($out['exit_status'] == 0) {
      $out['model'] = file_get_contents($out_path);
    } else {
      $out['model'] = '';
    }
    echo json_encode($out);
  }

  // Clean up the temporary files we created for the interpreter.
  if (file_exists($out_path)) {
    $ok = unlink($out_path);
  }

  if (file_exists($in_path)) {
    $ok = unlink($in_path);
  }
}

error_log("Results: " . json_encode($out));
?>
