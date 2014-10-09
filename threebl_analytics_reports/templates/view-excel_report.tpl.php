<?php
	header( "Content-Type: application/vnd.ms-excel" );
	header( "Content-disposition: attachment; filename=spreadsheet.xls" );
	
	echo ' Media Type' ."\t" . '#Fmr' . "\t" . '#Views' . "\t" .'#AVERAGE Views'. "\t" . '#AVERAGE Click'. "\n";
	echo ' Press Release' . "\t" . 'Doe' . "\t" . '555-5555' . "\t" . '555-5555' . "\t" . '555-5555'. "\n";
	echo ' Multimedia ' . "\t" . 'Doe' . "\t" . '555-5555' .  "\t" . '555-5555'."\t" . '555-5555'. "\n";
	echo ' Blogs' . "\t" . 'Doe' . "\t" . '555-5555' .  "\t" . '555-5555'."\t" . '555-5555'. "\n";
	echo ' Newsletter' . "\t" . 'Doe' . "\t" . '555-5555' .  "\t" . '555-5555'. "\t" . '555-5555'."\n";
	echo ' Article ' . "\t" . 'Doe' . "\t" . '555-5555' .  "\t" . '555-5555'."\t" . '555-5555'. "\n";
	echo ' All ' . "\t" . 'Doe' . "\t" . '555-5555' .  "\t" . '555-5555'."\t" . '555-5555'. "\n";
?>