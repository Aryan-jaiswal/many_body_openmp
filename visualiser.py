#! /usr/bin/python2

from numpy import *
from numpy import linalg
import numpy as np
import pygame
import time

def get_trajectory(filename): 
    data =[]
    with open(filename) as input_file:
        try:
            for line in input_file:
                line = line.strip()
                for number in line.split():
                    data.append(float(number))
        except IOError:
            print "Could not read file:", filename
    data = np.reshape(data,(1000,3))
    return data
num = 0
filename = "trajectory" +str(num) + ".bin"
data  = get_trajectory(filename)

# Pygame initialzation
pygame.init()
xResolution = 640
yResolution = 480
surface = pygame.display.set_mode((xResolution, yResolution))
xCenter = int(xResolution / 2)
yCenter = int(yResolution / 2)
font = pygame.font.Font(None, 20)

# Loop timesteps forever
timestep = 0
while True:

    filename = "trajectory" +str(timestep) + ".bin"
    # Increment timestep
    timestep = timestep + 100

    # Pygame update screen
    pygame.display.update()

    surface.fill((0,0,0))
    data = get_trajectory(filename)

    for i in range(0, len(data)):

        # Pygame draw body
        pygame.draw.circle(surface, (random.triangular(0,125,255),random.triangular(0,125,255),random.triangular(0,125,255)), (int(data[i][0]*640/200), int(data[i][1]*480/100)), 3 ,0)

        # Pygame write elapsed time
        text = font.render("Time: " + str(int(timestep * 0.1))+"s", True, (255,255,255))
        surface.blit(text,(10,10))
    time.sleep(0.1)


