import cv2

from matplotlib import pyplot as plt

import numpy as np

import imutils

import easyocr


IMAGE_PATH="image PATH"

reader=easyocr.Reader(['en'],gpu=False)

result =reader.readtext(IMAGE_PATH)

print("the plate number is ",result)
