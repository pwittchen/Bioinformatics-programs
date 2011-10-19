<?php

set_time_limit(0);

echo "<pre>\n"; // tag for preformated text (in case of running script in a web browser)

// reading and preparing the data

$alphabet = array('A','R','N','D','C','E','Q','G','H','I','L','K','M','F','P','S','T','W','Y','V','X');
$countAlphabetElements = count($alphabet);
$initialSequenceLines = file('mito_prepared.aa');
$initialSequenceString = '';
$initialSequenceArray = array();
$newStartingPoint = NULL;

foreach($initialSequenceLines as $line)
	$initialSequenceString .= str_replace("\r\n","",$line);
	
$initialSequenceArray = str_split($initialSequenceString); // array of letters
unset($initialSequenceLines);
unset($initialSequenceString);

$numberOfSequences = 50;  	// M - number of Sequences
$lengthOfSequence = 200;  	// N - length of a single sequence
$lengthOfSubsequence = 100; // L - length of subsequence
$iterations = 1000; 		// iterations of an algorithm

$sequenceUpperBound = count($initialSequenceArray) - $lengthOfSequence;
$sequenceArray = array();

// creating random sequences

for($i = 0; $i < $numberOfSequences; $i++)
	$sequenceArray[$i] = array_slice($initialSequenceArray, rand(0, $sequenceUpperBound), $lengthOfSequence);

unset($initialSequenceArray);

// creating subsequences (with random starting points)

$subSequenceUpperBound = $lengthOfSequence - $lengthOfSubsequence;
$subSequenceArray = array();

for($i = 0; $i < $numberOfSequences; $i++)
	$subSequenceArray[$i] = array_slice($sequenceArray[$i], rand(0, $subSequenceUpperBound), $lengthOfSubsequence);

// point no. 4 - beginning of the loop

for($loop = 0; $loop < $iterations; $loop++)
{
	if($newStartingPoint != NULL)
		$subSequenceArray[$indexOfSubSequenceToDelete] = array_slice($sequenceArray[$indexOfSubSequenceToDelete], $newStartingPoint, $lengthOfSubsequence);
	
	// choosing random sequence to delete
	
	$indexOfSubSequenceToDelete = rand(0, ($numberOfSequences - 1) );

	// creating the profile
	
	// preparing the data
	
	$profile = array();
	
	for($i = 0; $i < $lengthOfSubsequence; $i++)
		$profile[] = array_fill(0, ($countAlphabetElements - 1), 0);

	// calculating occurence of letters on each postion of each subSequence
		
	foreach($subSequenceArray as $subSequenceNumber => $subSequence)
	{
		if($subSequenceNumber != $indexOfSubSequenceToDelete) // neglecting removed sequence
		{
			foreach($subSequence as $letterPosition => $letter)
				$profile[$letterPosition][array_search($letter,$alphabet)]++;
		}
	}

	// calculating probability of letter occurrence - prob(a|P)

	foreach($profile as $aKey => $aValue)
	{
		foreach($aValue as $aLetterKey => $aLetterCount)
			$profile[$aKey][$aLetterKey] = $aLetterCount/$countAlphabetElements;
	}

	// generating the consensus string

	$consensusString = '';

	foreach($profile as $aKey => $aValue)
		$consensusString .= $alphabet[array_search(max($aValue), $aValue)];

	// generating slices of the removed sequence

	$fragmentsOfRemovedSequence = array();

	for($i = 0; $i <= $subSequenceUpperBound; $i++)
		$fragmentsOfRemovedSequence[] = array_slice($sequenceArray[$indexOfSubSequenceToDelete], $i, $lengthOfSubsequence);

	// calculating probability prob(a|P) 

	$probs = array();

	foreach($fragmentsOfRemovedSequence as $kFrag => $vFrag)
	{
		$probs[$kFrag] = 1;
		foreach($vFrag as $kLetter => $vLetter)
			$probs[$kFrag] *= $profile[$kLetter][array_search($vLetter, $alphabet)];
	}

	$motifWeight = max($probs);
	
	if($motifWeight != 0)
		$newStartingPoint = array_search($motifWeight, $probs);
	else
		$newStartingPoint = NULL; // avoidance of generating worse motifWeight
	
	echo("Iteration: ".$loop."\n");
	echo("Motif weight: ".$motifWeight."\n");
	echo("Consensus string: ".$consensusString."\n\n");
}

echo "\n</pre>";

?>
