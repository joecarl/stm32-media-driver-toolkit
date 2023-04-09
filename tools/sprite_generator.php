<?php

$img = imagecreatefrompng($argv[1]);

$output_file = $argv[2] ?? null;

$output = '';

$w = imagesx($img);
$h = imagesy($img);


for ($j = 0; $j < $h; $j++) {

	for ($i = 0; $i < $w; $i++) {

		$color = imagecolorat($img, $i, $j);
		$rgba = imagecolorsforindex($img, $color);

		if ($rgba['alpha'] > 127) {
			$hex = 0xC7;
		} else {
			$red = $rgba['red'] & 0b11000000;
			$green = ($rgba['green'] >> 2) & 0b00111000;
			$blue = ($rgba['blue'] >> 5) & 0b00000111;
			$hex = $red | $green | $blue;
		}
		
		$output .= '0x'.str_pad(strtoupper(dechex($hex)), 2, '0', STR_PAD_LEFT).', '; 

	}
	
	$output .= "\n";

}

if ($output_file) {
	file_put_contents($output_file, $output);
} else {
	echo $output;
}
