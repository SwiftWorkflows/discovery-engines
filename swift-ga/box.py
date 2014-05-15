#!/usr/bin/env python

import sys

box_index = int(sys.argv[1])
generation = int(sys.argv[2])
output_file = sys.argv[3]
score_file  = sys.argv[4]
winner_name = sys.argv[5]

with open(winner_name) as f:
    winner = f.read()

print ('box: ' + str(generation) + ':' + str(box_index) + ' previous winner: ' + winner)

with open(output_file, 'w') as f:
    f.write('box: generation: '+ str(generation) + ' individual: ' + str(box_index) + '\n')

with open(score_file, 'w') as f:
    f.write(str(box_index+generation) + '\n')
