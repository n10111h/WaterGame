/////////////////////////////////////////////////////////////////
// Water Game HM-10 Hello Bluetooth Sketch
// 1) Wire your HM-10 module to your Arduino board (see Wiring Resources)
// 2) Connect your Arduino board to your PC with a USB cable
// 3) Choose your board on the drop down list
// 4) Copy this entire sketch into the IDE
// 5) Edit the HM-10 pin numbers (rx_pin & tx_pin) if needed (lines 26,27)
// 6) Click the "Upload" button (right arrow) on the toolbar
// 7) Open the Serial Monitor (tools menu) if it is not already open
// 
//  Use your MicroBlue App on your phone to connect to your board and send commands.
//  You should see the message ID and Values in the Serial Monitor
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
  // Begin Serial Communication
  Serial.begin(9600);

  // Connect BLE serial port
  BLESerial.begin(9600);

  /************************************************************/
  // TEAM CODE
  // Initialize your pins (e.g. pinMode(motor1Forward, OUTPUT);

  // Do any other one time initialization that needs to be done for your code

  /************************************************************/
}

void loop() {
  // Get BLE message
  BLEMessage msg = getBLEMessage();

  /************************************************************/
  // TEAM CODE
  // See what message and value was sent if any (msg.id and msg.value) 
  // and control the robot as needed


  /************************************************************/


}
