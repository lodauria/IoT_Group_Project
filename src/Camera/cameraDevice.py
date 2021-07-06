import pygame.camera
import numpy as np
import paho.mqtt.client as mqtt
from PIL import Image
import cv2
import pytesseract
import re
import time

# IoT Core MQTT broker settings
AWS_HOST = "a3paxdkh0ekdqh-ats.iot.us-east-1.amazonaws.com"  # AWS endpoint
AWS_PORT = 8883
AWS_CA_CERTS = "/etc/mosquitto/certs/root-CA.crt"
AWS_CERT_FILE = "/etc/mosquitto/certs/smartmarina.cert.pem"
AWS_KEY_FILE = "/etc/mosquitto/certs/smartmarina.private.key"

# Send MQTT message with the boat id
def send_message(mess):
    
    print(client.publish("camera/boat", '{"boat_id": "' + mess + '"}'))


# Setup MQTT
client = mqtt.Client()
client.tls_set(AWS_CA_CERTS, AWS_CERT_FILE, AWS_KEY_FILE)
client.connect(AWS_HOST, AWS_PORT)

# Initialize the camera
pygame.init()
pygame.camera.init()
cam = pygame.camera.Camera("/dev/video0")

# Sensing loop
while 1:

    # Get image from the webcam
    cam.start()
    raw_image = cam.get_image()
    cam.stop()

    # Transform image
    raw_image = pygame.transform.rotate(pygame.transform.flip(raw_image, False, True), -90)
    pil_image = Image.fromarray(pygame.surfarray.array3d(raw_image)).convert('RGB')
    open_cv_image = np.array(pil_image)
    img = open_cv_image[:, :, ::-1].copy()

    # Grayscale, Gaussian blur, Otsu's threshold
    image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    image = cv2.GaussianBlur(image, (3, 3), 0)
    image = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)[1]

    # Morph open to remove noise and invert image
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3,3))
    image = cv2.morphologyEx(image, cv2.MORPH_OPEN, kernel, iterations=1)
    image = 255 - image

    # Extract text
    text = pytesseract.image_to_string(image, config='--psm 6')

    # For debug
    # cv2.imshow('image', image)
    # cv2.waitKey()
    # print(text)
    # text = "AR3973"

    # Identify a boat license plate
    res = re.findall("[A-Z]{2}[ ]?[0-9]{2,6}", text)

    # Send message if license plate is detected
    if len(res) != 0:
        
        lic = res[0].replace(" ", "")
        print("Boat detected: ", lic)
        send_message(lic)

    else:

        print("Boat license plate not detected")

    time.sleep(1)
