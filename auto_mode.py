import cv2
import numpy as np
import math
import serial
from picamera2 import Picamera2
import time

ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=1)  # Modify this to match your Arduino's serial port
    
def auto_mode():
    theta = 0
    minLineLength = 5
    maxLineGap = 10

    picam2 = Picamera2()

    picam2.start()
    while True:
        img = picam2.capture_array()
        img = cv2.resize(img, (480, 520))
        img = cv2.flip(img, -1)
        time.sleep(0.1)  # Allow the camera to warm up

        # Preprocess the img
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        edged = cv2.Canny(blurred, 85, 85)

        # Detect lines using Hough Transform
        lines = cv2.HoughLinesP(edged, 1, np.pi / 180, 10, minLineLength, maxLineGap)
        print(lines, end="\n")
        
        # Process detected lines
        if lines is not None:
            for line in lines:
                x1, y1, x2, y2 = line[0]
                cv2.line(img, (x1, y1), (x2, y2), (0, 255, 0), 3)
                theta += math.atan2((y2 - y1), (x2 - x1))

        threshold = 2  # Adjust this threshold as needed

        # Determine direction based on theta
        if lines is None:
            print("No lines detected, stopping")
            ser.write(b'x')  # Send command to stop
        elif theta > threshold:
            print("left")
            for i in range(7):
                ser.write(b'a')  # Send command for left
            time.sleep(2)
        elif theta < -threshold:
            print("right")
            for i in range(7):
                ser.write(b'd')  # Send command for right
            time.sleep(2)
        else:
            print("straight")
            ser.write(b'w')  # Send command for straight
            time.sleep(2)

        theta = 0

        # Display the processed img
        cv2.imshow("Frame", img)

        # Check for key press, if 'q' is pressed, exit the loop
        if cv2.waitKey(1) == ord('q'):
            print('Streaming Stopped because of key pressed event')
            break

    # Close all windows
    cv2.destroyAllWindows()

if __name__ == "__main__":
    auto_mode()
