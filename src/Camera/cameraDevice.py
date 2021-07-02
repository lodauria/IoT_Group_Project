import pygame.camera
import paho.mqtt.client as mqtt
from PIL import Image, ImageEnhance, ImageFilter
import pytesseract
import re


def send_message(mess):

    # Setup MQTT
    client = mqtt.Client("camera_client")
    client.connect("localhost", 1885)
    client.publish("camera/boat", '{"boat_id": ' + mess + "}")
    # ourClient.on_message = messageFunction  # Attach the messageFunction to subscription
    # client.loop_start()  # Start the MQTT client


# Initialization
pygame.init()
pygame.camera.init()
cam = pygame.camera.Camera("/dev/video0")

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
