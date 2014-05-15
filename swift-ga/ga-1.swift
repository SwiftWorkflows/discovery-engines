
import python;
import files;
import io;
import sys;
import string;

app (file o, file score) box(int individual, int generation, file winner_name)
{
  "python" "box.py" individual generation o score winner_name;
}

app (file winner_file) select(int generation, file population[])
{
  "python" "select.py" generation winner_file population ;
}

main
{
  individuals = toint(argv("individuals"));
  printf("individuals: %s", individuals);
  ngenerations = toint(argv("ngenerations"));
  printf("ngenerations: %s", ngenerations);

  file winners[];
  winners[0] = input("nobody.winner");

  for (int generation = 1; generation < ngenerations; generation = generation+1)
  {
    file population[];
    foreach box_index in [0:individuals-1]
    {
      file d<sprintf("d-%i-%i.out",generation,box_index)>;
      file s<sprintf("d-%i-%i.score",generation,box_index)>;
      (d,s) = box(box_index, generation, winners[generation-1]);
      population[box_index] = d;
    }
    file winner_file<sprintf("d-%i.winner", generation)> = select(generation, population);
    winners[generation] = winner_file;
  }
}

