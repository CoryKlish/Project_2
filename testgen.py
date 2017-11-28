#! /usr/bin/env python3

import csv
import os
from random import *
loops = 11
for i in range(loops):

    #Word to Daniel Chen on Piazza 

    inputFile = "movie_metadata.csv"

    numFiles = int(input("Number of files to create: "))
    numRows = int(input("Number of rows between 1 and 5043: "))

    os.mkdir(str(numFiles))
    #This creates directory name of the new created directory
    path = "./" + str(numFiles) + "/"


    reader = csv.reader(open(inputFile, 'r'))
    reader = list(reader)

    #write all csv files
    for i in range(numFiles):
        writer = csv.writer(open(path +'metadata_{}.csv'.format(i), 'w'))
        writer.writerow(reader[0]) #category row
        for i in range(numRows):
            writer.writerow(reader[randint(1,5043)])