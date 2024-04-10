import serial
import cv2
import threading
from picamera2 import Picamera2
import main as mpy

picam2 = Picamera2()

# Function to capture camera preview
def show_camera_preview():
    picam2.start()
    while True:
        img = picam2.capture_array()
        
        img = cv2.resize(img, (320, 480))
        img = cv2.flip(img, -1)
        cv2.imshow('Camera Preview', img)
        
        # Check for user input
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):  # Press 'q' to quit
            break
    
    cap.release()
    cv2.destroyAllWindows()
    

def instruction():
    print("""
***************** MANUAL MODE - INSTRUCTIONS *****************
1. Press 'w' to move forward
2. Press 's' to move backward
3. Press 'a' to turn left
4. Press 'd' to turn right
3. Press 'f' to start the filling chamber
4. Press 'j' to stop the filling chamber
5. Press 'x' to exit MANUAL MODE
**************************************************************
""")
    
def main_code():
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.write(b'2')  # Send command to Arduino to switch to manual mode
    prev_input = ''
    while True:
        user_input = input("Enter command (w: forward, s: stop, f: filling, d: stop filling, x: stop, q: exit): ")
        
        if user_input == 'q' and prev_input != 'x':
            print("Please stop the motor first before exiting manual mode.")
        elif user_input == 'q' and prev_input == 'x':
            cv2.destroyAllWindows()
            picam2.stop_preview()
            ser.write(b'x')
            print("EXITING MANUAL MODE\n")
            mpy.main_code()
        else:
            prev_input = user_input
            if user_input == 'w':
                print("Moving forward")
                ser.write(b'w')
            elif user_input == 's':
                print("Moving backward...")
                ser.write(b's')
            elif user_input == 'a':
                print("Turning left...")
                ser.write(b'a')
            elif user_input == 'd':
                print("Turning right...")
                ser.write(b'd')
            elif user_input == 'x':
                print("Stopping...")
                ser.write(b'x')
            elif user_input == 'f':
                print("Start filling...")
                ser.write(b'f')
            elif user_input == 'j':
                print("Stop filling...")
                ser.write(b'j')
            else:
                print("INVALID INPUT FROM USER\n")

def control():
    while True:
        camera_thread = threading.Thread(target=show_camera_preview)
        input_thread = threading.Thread(target=main_code)

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
