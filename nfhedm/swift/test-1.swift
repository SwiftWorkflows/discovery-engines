
import io;

main
{
  file GoodOrientationsFile = input("good_orientations.data");
  blob GoodOrientations[] = readGoodOrientations(GoodOrientationsFile);
  blob results[];
  foreach i in [0:countAllPossibleOrientations-1]
  {
    results[i] = optimizeOrientation(GoodOrientations[i]);
  }
  // writeResults(results);
}

