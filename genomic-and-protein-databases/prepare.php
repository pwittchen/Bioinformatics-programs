<?php

set_time_limit(0);

$lines = file('mito.aa');
$start = rand(0, count($lines));
$i = $start;
$loop = TRUE;
$output = array();
$outputString = '';
$beginString = FALSE;
$endString = FALSE;

while($loop)
{
	if($beginString && !$endString)
	{
		$output[] .= $lines[$i];
	}
	
	if( $lines[$i]{0} == '>' && !$beginString && !$endString)
	{
		$name = $lines[$i];
		$beginString = TRUE;
	}
	elseif( $lines[$i]{0} == '>' && $beginString && !$endString)
	{
		$endString = TRUE;
		$loop = FALSE;
	}
	
	$i++;
}

unset($output[count($output) - 1]);
unset($lines);

foreach($output as $value)
	$outputString .= $value;

$outputString = str_replace("\n","",$outputString);
	
echo $name;

file_put_contents('mito_prepared.aa',$outputString);

?>
