# Camera device

More details about the dock devices are reported below.

## Hardware wiring

![Hardware camera connection](../../resources/images/camera_device_connection.png)

In the prototype we used a Raspberry Pi 4 as main board to which we connected a USB webcam and an Ethernet cable for internet connection.

## How to setup

The libraries that has to be installed before running the code are:

- [Pillow](https://pypi.org/project/Pillow/)
- [Numpy](https://pypi.org/project/numpy/)
- [OpenCV](https://pypi.org/project/opencv-python/)
- [Eclipse Paho MQTT](https://pypi.org/project/paho-mqtt/)
- [Pytesseract](https://pypi.org/project/pytesseract/)
- [Pygame](https://pypi.org/project/pygame/)
- [Regex](https://pypi.org/project/regex/)

Run this command to install all required libraries:
```
pip install -U -r requirements.txt
```

You should install also [Tesseract](https://github.com/tesseract-ocr/tesseract), an OCR software developed by Google.
This procedure depends by your operating system.

On Arch Linux run:

```
sudo pacman -S tesseract tesseract-data-eng
```

## Network interface



## MQTT messages

The message sent when a boat is detected is published on the topic `camera/boat` with this format:

```
{
    "boat_id": "TS57845",
}
```

`boat_id` is the license plate of the boat.