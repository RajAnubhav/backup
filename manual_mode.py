import serial
import cv2
import threading
from picamera2 import Picamera2
import main as mpy

# Initialize Picamera2 instance
picam2 = Picamera2()

# Function to capture camera preview
def show_camera_preview():
    picam2.start()
    while not picam2.stopped:
        img = picam2.capture_array()
        
        img = cv2.resize(img, (480, 520))
        img = cv2.flip(img, -1)
        cv2.imshow('Camera Preview', img)
        
        # Check for user input
        key = cv2.waitKey(1) & 0xFF
        if key == ord('x'):  # Press 'x' to quit
            picam2.stop()
            break

    cv2.destroyAllWindows()

# Function for manual mode
def manual_mode():
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.write(b'2')  # Send command to Arduino to switch to manual mode
    prev_input = ''
    while True:
        user_input = input("Enter command (w: forward, s: stop, f: filling, d: stop filling, x: stop, q: exit): ")
        
        if user_input == 'q' and prev_input != 'x':
            print("Please stop the motor first before exiting manual mode.")
        elif user_input == 'q' and prev_input == 'x':
            print("Exiting MANUAL MODE\n")
            break
        else:
            prev_input = user_input
            ser.write(user_input.encode())

# Function to control threads
def control():
    while True:
        camera_thread = threading.Thread(target=show_camera_preview)
        input_thread = threading.Thread(target=manual_mode)

        camera_thread.start()
        input_thread.start()

        camera_thread.join()
        input_thread.join()

        choice = input("Press 'm' to go back to manual mode, '1' for automatic mode, or 'x' to stop: ")
        if choice == 'm':
            continue
        elif choice == '1':
            return 'automatic'  # Return to main.py loop with 'automatic' mode
        elif choice == 'x':
            print("Exiting manual mode.")
            return 'exit'  # Return to main.py loop with 'exit'
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    control()
