<?php

$inp_filename = $argv[1];
$output_file = $argv[2] ?? null;

$output = '';
$bin = fopen($inp_filename, 'rb');
$c = 0;

do {

	$contents = fread($bin, 1024);
	$byte_array = unpack("C*", $contents); 

	foreach ($byte_array as $i => $byte) {

		$byte = $byte_array[$i];
			
		$output .= '0x'.str_pad(strtoupper(dechex($byte)), 2, '0', STR_PAD_LEFT).', '; 

		if (++$c % 40 === 0) { // 40 bytes per line
			$output .= "\n";
		}

	}

} while (!feof($bin));

if ($output_file) {
	file_put_contents($output_file, $output);
} else {
	echo $output;
}
