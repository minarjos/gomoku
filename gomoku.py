#!/usr/bin/python3

import os
import sys
from argparse import ArgumentParser
parser = ArgumentParser()

parser.add_argument('-n', '--boardsize', type=int, required=False,
                    help='Size of the board. Default value is 15.',
                    default=15)

parser.add_argument('-t', '--time', type=int, required=False,
                    help='Time for AI per move in miliseconds. Default value is 1000.',
                    default=1000)

parser.add_argument('-o', '--ostarts', required=False,
                    help='The player with O symbol starts.',
                    action='store_true')

parser.add_argument('-p', '--firsthuman', required=False,
                    help='First player is human.',
                    action='store_true')

parser.add_argument('-a', '--secondai', required=False,
                    help='Second player is ai.',
                    action='store_true')

arguments = parser.parse_args(sys.argv[1:])

to_move = 1
if arguments.ostarts:
    to_move = 2

os.system("./main " + str(arguments.boardsize) + " " + str(to_move)
           + " " + str(arguments.time) + " " + str(int(arguments.secondai))
           + " " + str(int(arguments.firsthuman)))
