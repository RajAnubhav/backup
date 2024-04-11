import serial
import manual_mode as mm
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)  # Update the port accordingly

def main_code():
    
    while True:
        val = input("Press 1 for automatic mode\nPress 2 for manual mode\nPress x to stop\n")
            
        if val == "1":
            print("Entered automatic mode:\n")
            ser.write(b'1')
        elif val == "2":
            print("Entered MANUAL MODE:\n")
            ser.write(b'2')
            if mm.control():
                continue
        elif val == "x":
            print("Thank you. Have a nice day!")
            exit(0)
        else:
            print("INVALID INPUT FROM USER\n")
   
if __name__ == "__main__":
    main_code()
