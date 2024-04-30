/*
 ******************************************************************
 * Find Servo Positions for R3 with an HM-10
 *
 * 1) Build your mechanism, wire up your servo, and detach the servo horn from the servo
 * 1.5) Edit line 17 below to match the pin your servo is getting a PWM signal from
 * 2) Download and run this sketch on your R3 Uno.  The Servo should immediately go to its center position
 * 3) Connect MicroBlue and make sure you have two buttons ("bup" and "bdown")
 * 4) reconnect your servo horn in the middle of the range of travel you want
 * 5) Use the up and down buttons to move the servo to various positions, note the values on the LED display
 *
 *
 ******************************************************************
*/

const int servoPin = 11; // Change this to the right pin for your build
int position = 255/2; // start in the middle

// Include Libraries
#include <SoftwareSerial.h> // To communicate with the HM-10 we use the Arduino SoftwareSerial library
                            // Note: There are other libraries that may peform better but we have not
                            // done extensive testing with them.  Search for "SoftwareSerial replacement"


/************************************************************/
// TEAM CODE

// Pin values (e.g. const int motor1Forward = 1;)
int rx_pin = 7; // This should be wired to the TX pin on the HM-10
int tx_pin = 8; // This should be wired to the RX pin on the HM-10

// Other constants (e.g. const int maxForwardPower = 175;)

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
  if (Serial) {
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
  pinMode(servoPin, OUTPUT);

  // Begin Serial Communication
  Serial.begin(9600);

  // Connect BLE serial port
  BLESerial.begin(9600);

  /************************************************************/
  // TEAM CODE
  // Initialize your pins (e.g. pinMode(motor1Forward, OUTPUT);

  // Do any other one time initialization that needs to be done for your code

  analogWrite(servoPin, position);
  Serial.println(position);

  /************************************************************/
}

void loop() {
  // Get BLE message
  BLEMessage msg = getBLEMessage();

  /************************************************************/
  // TEAM CODE
  // See what message and value was sent if any (msg.id and msg.value) 
  // and control the robot as needed
      if (msg.id == "bup") { 
        if (msg.value.toInt() == 1) {
          if (position < 254) position++;
          analogWrite(servoPin, position);
          Serial.println(position);
        }
      }
      if (msg.id == "bdown") { 
        if (msg.value.toInt() == 1) {
          if (position > 1) position--;
          analogWrite(servoPin, position);
          Serial.println(position); // also write it to the Serial Monitor
        }
      }

  /************************************************************/


}
