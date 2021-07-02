import pytesseract
import pygame
from pygame.locals import *
import pygame.camera

# Initialization  
pygame.init()
pygame.camera.init()
cam = pygame.camera.Camera("/dev/video0",(640,480))

cam.start()
image = cam.get_image()
cam.stop()

text = pytesseract.image_to_string(image)