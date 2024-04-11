#include <Servo.h>

Servo myservo; 
// Motor Pin Declaration Starts
int motor1pin1 = 22;
int motor1pin2 = 2;

int motor2pin1 = 3;
int motor2pin2 = 23;

int motor3pin1 = 4;
int motor3pin2 = 24;

int motor4pin1 = 25;
int motor4pin2 = 5;
// Motor pin declaration ends
// IR Pins Declaration
const int IR1 = 26;
const int IR2 = 29;
const int IR3 = 28;
int IR1Val, IR2Val, IR3Val;

// Defining the modes
bool userMode = false;
bool automaticMode = false;
bool trialMode = false;

// Defines pins numbers for ultrasonic sensor
const int trigPin1 = 6;
const int echoPin1 = 7;
const int trigPin2 = 8;
const int echoPin2 = 9;
const int trigPin3 = 10;
const int echoPin3 = 11;
// Defines variables
long duration1;
int distance1;
long duration2;
int distance2;
long duration3;
int distance3;

// State variables
bool isMoving = false;
unsigned long lastMotionTime = 0;
const unsigned long motionInterval = 10;// Time interval for each motion state in milliseconds
const unsigned int motorSpeed = 130; // speed of motor
const unsigned int trialMotor = 200;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Motor Declaration
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor3pin1, OUTPUT);
  pinMode(motor3pin2, OUTPUT);
  pinMode(motor4pin1, OUTPUT);
  pinMode(motor4pin2, OUTPUT);

  Serial.println("Motor Declaration Completed");
  // IR Declaration
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  Serial.println("IR Declaration Completed");
  myservo.attach(12);
  myservo.write(30);
  Serial.println("Initializing Servo to Zero");

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT);  // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT);  // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT);  // Sets the echoPin as an Input
  Serial.println("Ultrasonic SR04 Declaration Completed");
}

void loop() {
  if(!automaticMode && !userMode && !trialMode) {
      if(Serial.available() > 0) {
        char mode = Serial.read();
        if(mode == '1') {
          automaticMode = true;
          Serial.println("Entered automatic mode...");
        } else if(mode == '2') {
          userMode = true;
          Serial.println("Entered manual mode...");
        } else if(mode == '3') {
          trialMode = true;
          Serial.println("Entered trial mode...");
        } else if(mode == 'x') {
          stop();
          Serial.println("Stopping Vehicle...");
        }
      }
    } else {
      // If a mode is selected, execute the corresponding mode's logic
      if(automaticMode) {
        handleAutomaticMode();
      } else if(userMode) {
        handleManualMode();
      } else if(trialMode) {
        handleTrialMode();
      }
    }
}

void handleTrialMode() {
  if(Serial.available() > 0) {
      char val = Serial.read();
      if(val == 'w') {
        moveForward();
        delay(200);
      } else if(val == 's') {
        moveBackward();
        delay(200);
      } else if(val == 'a') {
        turnLeft();
        delay(200);
      } else if(val == 'd') {
        turnRight();
        delay(200);
      } else if(val == 'x') {
        stop();
      }
   }
}

void handleManualMode() {
    // Manual Mode
    if(Serial.available() > 0) {
      char val = Serial.read();
      if(val == 'w') {
        moveForward();
        delay(200);
      } else if(val == 's') {
        moveBackward();
        delay(200);
      } else if(val == 'a') {
        turnLeft();
        delay(200);
      } else if(val == 'd') {
        turnRight();
        delay(200);
      } else if(val == 'x') {
        stop();
      }
   }
}

void handleAutomaticMode() {
  IR1Val = digitalRead(IR1);
  IR2Val = digitalRead(IR2);
  IR3Val = digitalRead(IR3);
  Serial.print("IR1 - ");
  Serial.print(IR1Val);
  Serial.print("  IR2 - ");
  Serial.print(IR2Val);
  Serial.print("  IR3 - ");
  Serial.print(IR3Val);
  Serial.println("");

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print("  Distance2: ");
  Serial.print(distance2);
  Serial.print("  Distance3: ");
  Serial.print(distance3);
  Serial.println();

  unsigned long currentTime = millis();
  
  // Check if it's time to change motion state
  if (currentTime - lastMotionTime >= motionInterval) {
    isMoving = !isMoving; // Toggle motion state
    lastMotionTime = currentTime;
  }

  // Perform action based on motion state
   if (distance1 <= 7 && distance2 <= 7 && distance3 <= 7 && (IR1Val == 1 && IR2Val == 1 && IR3Val == 1)) {
     moveForward();
   } else if(IR1Val == 0 && IR2Val == 1 && IR3Val == 0){
     stop();
   } else {
      delay(5000);
     moveForward(); // called 2nd time
   }
}


void turnRight() {
    Serial.println("Turning Right...");
    
    // Motor A
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1pin1, motorSpeed);
    
    // Motor B
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(motor2pin1, motorSpeed);

    // Motor C
    digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);
    analogWrite(motor3pin1, motorSpeed);

    // Motor D
    digitalWrite(motor4pin1, HIGH);
    digitalWrite(motor4pin2, LOW);
    analogWrite(motor4pin1, motorSpeed);

    // checking modes
    if(automaticMode) {
      delay(50);
    } else if(userMode) {
      delay(200);
    } else if(trialMode) {
      delay(200);
    }
    
    stop();
}

void turnLeft() {
    Serial.println("Turning Left...");
    
    // Motor A
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1pin1, motorSpeed);
    
    // Motor B
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(motor2pin1, motorSpeed);

    // Motor C
    digitalWrite(motor3pin1, HIGH);
    digitalWrite(motor3pin2, LOW);
    analogWrite(motor3pin1, motorSpeed);

    // Motor D
    digitalWrite(motor4pin1, LOW);
    digitalWrite(motor4pin2, HIGH);
    analogWrite(motor4pin1, motorSpeed);

    // checking modes
    if(automaticMode) {
      delay(50);
    } else if(userMode) {
      delay(200);
    } else if(trialMode) {
      delay(200);
    }
    
    stop();
}

void moveBackward() {
    Serial.println("Moving Backward...");
    
    // Motor A
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1pin2, motorSpeed);
    
    // Motor B
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(motor2pin2, motorSpeed);

    // Motor C
    digitalWrite(motor3pin1, LOW);
    digitalWrite(motor3pin2, HIGH);
    analogWrite(motor3pin2, motorSpeed);

    // Motor D
    digitalWrite(motor4pin1, LOW);
    digitalWrite(motor4pin2, HIGH);
    analogWrite(motor4pin2, motorSpeed);

    // checking modes
    if(automaticMode) {
      delay(50);
    } else if(userMode) {
      delay(200);
    } else if(trialMode) {
      delay(200);
    }
    
    stop();
}

void moveForward() {
  Serial.println("Moving Forward...");

  // Motor A
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  analogWrite(motor1pin1, motorSpeed);

  // Motor B
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  analogWrite(motor2pin1, motorSpeed);

  // Motor C
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);
  analogWrite(motor3pin1, motorSpeed);

  // Motor D
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
  analogWrite(motor4pin1, motorSpeed);

  // checking modes
    if(automaticMode) {
      delay(50);
    } else if(userMode) {
      delay(200);
    } else if(trialMode) {
      delay(200);
    }
    
  stop();
}

void stop() {
  Serial.println("Stopping...");

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);

  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
}
