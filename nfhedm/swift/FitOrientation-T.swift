
import io;
import sys;

/**
 @param rown Integer in [1,270]
*/
@dispatch=WORKER
  (int o) FitOrientation_Swift(string ParameterFileName, int rown,
                               string MicrostructureFileName)
"tclnfhedm" "0.0"
[ "set <<o>> [ FitOrientation_Swift <<ParameterFileName>> <<rown>> <<MicrostructureFileName>>]" ];

main
{
  ParameterFileName = argv("p");
  MicrostructureFileName = argv("m");
  start = toint(argp(1));
  end   = toint(argp(2));

  printf("FitOrientation-T: parameters=%s rown=%i:%i",
         ParameterFileName, start, end);
  foreach rown in [start:end] {
    printf("rown: %i", rown);
    FitOrientation_Swift(ParameterFileName, rown, MicrostructureFileName);
  }
}
