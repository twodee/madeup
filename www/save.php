<?php

$json = file_get_contents("php://input");
$in = json_decode($json, true);

$timestamp = date('Y_m_d_H_i_s');
$source = "-- ${in['name']}\n-- $timestamp\n${in['source']}";

$outpath = sprintf("saves/%s_%s", $timestamp, preg_replace('/[^A-Za-z0-9]/', '_', $in['name']));
file_put_contents($outpath, $source);

?>
