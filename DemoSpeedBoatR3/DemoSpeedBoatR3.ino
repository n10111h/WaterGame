/////////////////////////////////////////////////////////////////
// Water Game Demo Speed Boat Example Sketch
//
// This sketch is provided as a sample to help get your started
// See our Demo Speed Boat profile on the website for more
//
/////////////////////////////////////////////////////////////////

// Include Libraries
#include <SoftwareSerial.h> // To communicate with the HM-10 we use the Arduino SoftwareSerial library
                            // Note: There are other libraries that may peform better but we have not
                            // done extensive testing with them.  Search for "SoftwareSerial replacement"


/************************************************************/
// TEAM CODE

// Pin values (e.g. const int motor1Forward = 1;)
int rx_pin = 7; // This should be wired to the TX pin on the HM-10
int tx_pin = 8; // This should be wired to the RX pin on the HM-10

const int propMotor1 = 5;
const int propMotor2 = 6;
const int winchMotor1 = 9;
const int winchMotor2 = 10;
const int rudderServo = 11;

// Other constants (e.g. const int maxForwardPower = 175;)
const int joyStickMid = 512;
const int joyStickMax = 1023;
const int motorPowerMax = 255;
const int rudderMin = 50;
const int rudderMax = 205;
const int winchPower = 100;

bool debug = false; // set to true to get debug info on Serial Monitor

/************************************************************/

//BLE Initialization
SoftwareSerial BLESerial(rx_pin, tx_pin);  // Initialize serial channel for bluetooth


/////////////////////////////////////////////////////////////////
// getBLEMessage()  This is a helper method that will attempt to
//                  retrieve a new communication from the MicroBlue
//                  app.  The result is returned in the BLEMessage struct
struct BLEMessage {
  String id = "";
  String value = "";
};
struct BLEMessage getBLEMessage() {
  BLEMessage msg;
  // Read id and value if detect signature code 1
  if (BLESerial.available()) {
    int v = BLESerial.read();
    if (v == 1) {
      unsigned long startTime = millis();
      while (millis() - startTime < 1000) {  // read the ID
        if (BLESerial.available()) {
          v = BLESerial.read();
          if (v == 2) break;       // Move to next while loop
          msg.id.concat((char)v);  // Append character to id string
        }
      }
      while (millis() - startTime < 1000) { // read the value
        if (BLESerial.available()) {
          v = BLESerial.read();
          if (v == 3) break;          // Finish appending to message strings
          msg.value.concat((char)v);  // Append character to value string
        }
      }
    }
    msg.value.toLowerCase();     // Convert all letters in value to lowercase
    msg.value.replace(" ", "");  // Filter out all spaces in message value
  }
  // Print id and value if Serial exist (for debugging and development)
  if (debug && Serial) {
    if (msg.id.length() > 0 && msg.value.length() > 0) {
      Serial.print("id:");
      Serial.print(msg.id);
      Serial.print(" ");
      Serial.print("value:");
      Serial.println(msg.value);
    }
  }
  return msg;
}


/////////////////////////////////////////////////////////////////
// setup()  This method is called once when your sketch first
//          starts to run.  Initialize things here
void setup() {
  // Begin Serial Communication
  Serial.begin(9600);

  // Connect BLE serial port
  BLESerial.begin(9600);

  /************************************************************/
  // TEAM CODE
  // Initialize your pins (e.g. pinMode(motor1Forward, OUTPUT);
  pinMode(propMotor1, OUTPUT);
  pinMode(propMotor2, OUTPUT);
  pinMode(winchMotor1, OUTPUT);
  pinMode(winchMotor1, OUTPUT);
  pinMode(rudderServo, OUTPUT);
  // Do any other one time initialization that needs to be done for your code

  /************************************************************/
}

/************************************************************/
// TEAM CODE
// runMotor()
// A helper function for driving a motor that has two pins connected.
// This helps ensure we don't apply a signal (and thus power)
// to both wires to the motor at the same time
void runMotor(int pin1, int pin2, int power) {
  if (power > 0) {
    analogWrite(pin1,power);
    analogWrite(pin2, 0);
  } else if (power < 0) { // Arduino pins don't deal with negative power
    analogWrite(pin1,0);
    analogWrite(pin2, -power); // so we reverse it and put it on the other pin
  } else {
    analogWrite(pin1,0);
    analogWrite(pin2, 0);
  }
}


void loop() {
  // Get BLE message
  BLEMessage msg = getBLEMessage();

  /************************************************************/
  // TEAM CODE
  // See what message and value was sent if any (msg.id and msg.value) 
  // and control the robot as needed
  if (msg.id == "d1") { // joystick and also d-pad
    // The joystick control sends back two values with a comma between them
    String xString = msg.value.substring(0, msg.value.indexOf(',')); // get the first value (before the comma)
    String yString = msg.value.substring(msg.value.indexOf(',')+1,msg.value.length()); // get the second value (after the comma)  
    int yValue = yString.toInt()-joyStickMid; // convert the string representation to an int and shift it so 0 is at the middle of the joystick
    int forwardPower = yValue * 255L / joyStickMid; // convert from -512 - 512 to -255 - 255 (see the Arduino map() function for another way to do this)
    int xValue = xString.toInt();
    int rudder = (long)xValue * (rudderMax-rudderMin) / 1023L + rudderMin; // convert from 0-1023 to rudderMin-rudderMax (see the Arduino map() function for another way to do this)

    if (debug) {
      Serial.print("F: "); Serial.print(forwardPower);
      Serial.print(", R: "); Serial.println(rudder);
    }
    runMotor(propMotor1, propMotor2, forwardPower); // Set prop power
    analogWrite(rudderServo, rudder); // set rudder Servo Position
  }
  if (msg.id == "bup") {  // button is called "bup" for Button Up
    if (msg.value == "1") { //if the button is down
      analogWrite(winchMotor2, winchPower); // pull in the fish!
      analogWrite(winchMotor1, 0);
    } else {
      analogWrite(winchMotor1, 0);    // stop motor when they let go
      analogWrite(winchMotor2, 0);      
    }
  }
    if (msg.id == "bdown") { 
    if (msg.value == "1") {
      analogWrite(winchMotor1, winchPower); // lower the hook
      analogWrite(winchMotor2, 0);
    } else {
      analogWrite(winchMotor1, 0);     // stop motor when they let go
      analogWrite(winchMotor2, 0);      
    }
  }

  /************************************************************/


}
