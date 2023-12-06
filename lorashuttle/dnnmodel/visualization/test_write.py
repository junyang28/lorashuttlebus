import csv
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

lastx = ""
lasty = ""

with open('test380.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line = 0
    for row in csv_reader:
        print(len(row))
        x = row[2]
        y = row[3]
        if x == lastx and y == lasty:
            continue
        lastx = x
        lasty = y
        if x != '32767' and y  != '32767':
            f = open("sampleText.txt", "a")
            temp = x + "," + y
            f.write(temp)
            f.write("\n")
            f.close()
            line += 1
            time.sleep(0.3)
