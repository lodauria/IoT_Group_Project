import pygame.camera
import paho.mqtt.client as mqtt
from PIL import Image, ImageEnhance, ImageFilter
import pytesseract
import re
import time

AWS_HOST = "a3paxdkh0ekdqh-ats.iot.us-east-1.amazonaws.com"  # AWS endpoint
AWS_PORT = 8883
AWS_CA_CERTS = "/etc/mosquitto/certs/root-CA.crt"
AWS_CERT_FILE = "/etc/mosquitto/certs/smartmarina.cert.pem"
AWS_KEY_FILE = "/etc/mosquitto/certs/smartmarina.private.key"

def send_message(mess):
    #if isConnected:
    print(client.publish("camera/boat", '{"boat_id": "' + mess + '"}'))


# Setup MQTT
client = mqtt.Client()
client.tls_set(AWS_CA_CERTS, AWS_CERT_FILE, AWS_KEY_FILE)
client.connect(AWS_HOST, AWS_PORT)

# Initialization camera
pygame.init()
pygame.camera.init()
cam = pygame.camera.Camera("/dev/video0")

while 1:
    # Get image from the webcam
    cam.start()
    raw_image = cam.get_image()
    cam.stop()

    # Trasform image if needed
    image = pygame.transform.rotate(pygame.transform.flip(raw_image, False, True), -90)

    # Convert to PIL image and enhance colors
    img = Image.fromarray(pygame.surfarray.array3d(image))
    img = img.filter(ImageFilter.MedianFilter())
    enhancer = ImageEnhance.Contrast(img)
    img = enhancer.enhance(4)
    # Extract text
    text = pytesseract.image_to_string(img)

    # DEBUG
    #img.show()
    #print(text)
    #text = "AR3973"

    # Identify a boat license plate if present
    res = re.findall("[A-Z]{2}[0-9]{2,6}", text)

    if len(res) != 0:
        print("Boat detected: ", res[0])
        send_message(res[0])

    else:
        print("Boat license plate not detected")

    time.sleep(1)
