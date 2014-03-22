for LayerNr in {1..3}
do
	for FileNr in {0..180}
	do
		./ImageProcessing $1 $LayerNr $FileNr # ParameterFN
	done
done
