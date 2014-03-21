echo "Compiling codes: MakeHexGrid, MakeDiffrSpots, FitOrientationParameters and  FindOrientation."
./CompileCode.sh MakeHexGrid
./CompileCode.sh MakeDiffrSpots
cp FitOrientation.c backups/FitOrientation.c.bak
cp CalcDiffractionSpots.c backups/CalcDiffractionSpots.c.bak
cp midaco.c backups/midaco.c.bak
gcc -o MedianImage MedianImage.c -I/lib64/dip/Linuxa64/include -L/lib64/dip/Linuxa64/lib -ldipio -ldip -lm
gcc -o ImageProcessing ImageProcessing.c -lm
gcc -o FitOrientation FitOrientation.c CalcDiffractionSpots.c SharedFuncsFit.c -fPIC -ldl -lm -lnlopt
gcc -o FitOrientationParametersNLOpt FitOrientationParametersNLOpt.c CalcDiffractionSpots.c SharedFuncsFit.c -fPIC -ldl -lm -lnlopt
gcc -o CheckSpotsUsingOrientation CheckSpotsUsingOrientation.c CalcDiffractionSpots.c SharedFuncsFit.c -fPIC -ldl -lm
echo "Compilation succeeded."
# ./MedianImage Parameters.txt $1 # DistanceNr (starting from 1)
# ./ImageProcessing Parameters.txt $1 $2 # DistanceNr(starting from 1) and FileNumber (starting typically from 0)
./MakeDiffrSpots $1
# ./MakeHexGrid Parameters.txt
./FitOrientationParametersNLOpt $1 $2 # Grid point number
# ./FitOrientation $1 $2 # Grid point number
