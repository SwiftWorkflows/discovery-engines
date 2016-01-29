
import io;
import sys;

/**
 @param rown Integer in [1,270]
*/
@dispatch=WORKER
  (float result[]) FitOrientation_Swift(string ParameterFileName, int rown)
"tclnfhedm" "0.0"
[
----
    set s [ blobutils_malloc 160 ]
    set s [ blobutils_cast_to_char_ptr $s ]    
    set b [ FitOrientation_Swift <<ParameterFileName>> <<rown>> $s ]
    if { ! b } { puts "ERROR in FitOrientation!" }
    puts "result: $s"
    blob_free $s    
----
];

main
{
  ParameterFileName = argv("p");
  MicrostructureFileName = argv("m");
  start = toint(argp(1));
  end   = toint(argp(2));

  printf("FitOrientation-T: parameters=%s rown=%i:%i",
         ParameterFileName, start, end);
  foreach rown in [start:end] {
    // printf("rown: %i", rown);
    FitOrientation_Swift(ParameterFileName, rown);
  }
}
