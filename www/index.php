<?php

$use_minified = true;

$html = file_get_contents('index.html');

if (array_key_exists('embed', $_REQUEST) && strcmp($_REQUEST['embed'], 'true') == 0) {
  $html = str_replace('var isEmbedded = false; // PHP', 'var isEmbedded = true;', $html);
}

if (strpos($_SERVER['HTTP_HOST'], 'to.madeup') !== false ||
    (isset($_REQUEST['editor']) && strcmp($_REQUEST['editor'], 'blocks') == 0)) {
  $html = str_replace('var isBlocksURL = false; // PHP', 'var isBlocksURL = true;', $html);
}

if (array_key_exists('isPresenting', $_REQUEST)) {
  $html = str_replace('var isPresenting = false; // PHP', "var isPresenting = {$_REQUEST['isPresenting']};", $html);
}

if (array_key_exists('movie', $_REQUEST)) {
  $html = str_replace('var keystrokesMoviePrefix = null; // PHP', "var keystrokesMoviePrefix = '{$_REQUEST['movie']}';", $html);
}

if (array_key_exists('lesson', $_REQUEST)) {
  $html = str_replace('var lesson = null; // PHP', "var lesson = '{$_REQUEST['lesson']}';", $html);
  if (!array_key_exists('nosnaps', $_REQUEST)) {
    $html = str_replace('var isSnapshot = false; // PHP', "var isSnapshot = true;", $html);
  }
}

function comment($matches) {
  $lines = preg_replace('/^(.*)$/m', '<!-- \\1 -->', $matches[0]);
  return $lines;
}

function uncomment($matches) {
  $lines = preg_replace('/<!--\s*(.*?)\s*-->/m', '\\1', $matches[0]);
  return $lines;
}

if ($use_minified) {
  $html = preg_replace_callback("/(?<=<!-- Unminified -->\n).*?(?=^\s*<!-- \/Unminified -->)/ms", 'comment', $html);
  $html = preg_replace_callback("/(?<=<!-- Minified -->\n).*?(?=^\s*<!-- \/Minified -->)/ms", 'uncomment', $html);
}

$html = str_replace('var madeupPrefix = \'http://madeup.xyz\'; // PHP', "var madeupPrefix = '.';", $html);

if (array_key_exists('movie', $_REQUEST)) {
  $div = <<<EOF
    <div id="keystrokes">
      <audio controls="controls" id="scrubber">
        <source src="{$_REQUEST['MOVIE']}.wav" type="audio/wav">
        <source src="{$_REQUEST['MOVIE']}.ogg" type="audio/ogg">
      </audio>
    </div>
EOF;
  $html = str_replace('PHP:KEYSTROKES', $div, $html);
}

if (array_key_exists('src', $_REQUEST)) {
  $src = str_replace(array("\r\n", "\n", "\r"), "\\n", $_REQUEST['src']);
  $src = str_replace("'", "\\'", $src);
  $script = "source0 = '$src';";
  if (!array_key_exists('runZeroMode', $_REQUEST)) {
    $script .= "\nrunZeroMode = '{$_REQUEST['runZeroMode']}';";
  }
  $html = str_replace('// SRC:PHP', $script, $html);
}

echo $html;
?>
