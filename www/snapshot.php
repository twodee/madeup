<?php
$json = file_get_contents("php://input");
$in = json_decode($json, true);

date_default_timezone_set('America/Chicago');
$timestamp = date('Y_m_d_H_i_s');
$id = $in['id'];
$source = $in['source'];
$lesson = $in['lesson'];

$outpath = sprintf("saves/%s_%s_%s", $id, $lesson, $timestamp);
file_put_contents($outpath, $source);
?>
