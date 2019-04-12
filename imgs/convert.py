from __future__ import print_function
import cv2 
from matplotlib import pyplot as plt
import numpy as np
from os import listdir
from os.path import isfile, join


mypath = './tmp/'

def convert_and_output(filepath, filename):
    filename = filename.split('.')[0]
    grid = cv2.imread(filepath)

    r = 384.0 / grid.shape[1]
    dim = (384, int(grid.shape[0] * r))
    grid = cv2.resize(grid, dim) 

    grid_gray = cv2.cvtColor(grid, cv2.COLOR_BGR2GRAY)
    # ret,grid_thresh = cv2.threshold(grid_gray,90,255,cv2.THRESH_BINARY)
    # grid_thresh = cv2.adaptiveThreshold(grid_gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,21,cv2.THRESH_OTSU)
    grid_thresh = cv2.adaptiveThreshold(grid_gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,41,5)
    grid_thresh = 1*(grid_thresh<120)


    with open('./tmp/output/{0}.bytes'.format(filename), 'wb') as output:
        
        for line in grid_thresh:
            for pixel in range(0,len(line),8):
                output.write(line[pixel] | line[pixel+1] <<1| line[pixel+2] <<2|line[pixel+3] <<3|line[pixel+4] <<4|line[pixel+5] <<5| line[pixel+6] <<6|line[pixel+7] <<7)


for f in listdir(mypath):
    filepath = join(mypath, f)
    if isfile(filepath):
        convert_and_output(filepath, f)

