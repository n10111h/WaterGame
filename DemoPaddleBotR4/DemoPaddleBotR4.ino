/////////////////////////////////////////////////////////////////
// Water Game Demo Paddle Bot Example Sketch
//
// This sketch is provided as a sample to help get your started
// See our Demo Paddle Bot profile on the website for more
//
/////////////////////////////////////////////////////////////////

// Include Libraries
#include <ArduinoBLE.h>  // To communicate with the Built in BLE chip we use the ArduinoBLE library

// Set up BLE using MicroBlue UUID
BLEService CustomService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic CustomCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214",
                                       BLERead | BLENotify | BLEWrite, 100);


/************************************************************/
// TEAM CODE

// Bluetooth name (Max 12 characters)
const char *name = "19743B"; // Change this to something unique to your team

// Pin values (e.g. const int motor1Forward = 1;)
const int motorA1 = 9;
const int motorA2 = 10;
const int motorB1 = 5;
const int motorB2 = 6;

// Other constants (e.g. const int maxForwardPower = 175;)
const int joyStickMid = 512;
const int joyStickMax = 1023;
const int motorPowerMax = 255;

bool debug = false;
/************************************************************/

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
  // Read id and value if we detect signature code 1
  if (CustomCharacteristic.written()) {
    uint8_t charCodes[100];
    int valueLength = CustomCharacteristic.valueLength();
    CustomCharacteristic.readValue(charCodes, valueLength);
    if (charCodes[0] == 1) {
      int i = 1;
      while (i < valueLength - 1 && charCodes[i] != 2) { // read the msg ID
        msg.id.concat((char)charCodes[i]);
        i++;
      }
      i++;
      while (i < valueLength - 1 && charCodes[i] != 3) { // read the msg value
        msg.value.concat((char)charCodes[i]);
        i++;
      }
    }
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

  // Begin BLE Initialization
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy failed!");
  }
  // set advertised local name and service UUID:
  BLE.setLocalName(name);
  BLE.setDeviceName(name);
  BLE.setAdvertisedService(CustomService);
  // add the characteristic to the service
  CustomService.addCharacteristic(CustomCharacteristic);
  // add service
  BLE.addService(CustomService);
  // start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral, waiting for connections....");

  /************************************************************/
  // TEAM CODE
  // Initialize your pins (e.g. pinMode(motor1Forward, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
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


/////////////////////////////////////////////////////////////////
// loop()  This method is called over and over while your program
//         continues to run
void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  if (central) { // if a central is connected to peripheral:
    Serial.print("Connected to central: ");
    Serial.println(central.address()); // print the central's MAC address:
  
    while (central.connected()) { // while the central is still connected to peripheral
      BLEMessage msg = getBLEMessage(); // Try to get the next message

      /************************************************************/
      // TEAM CODE
      // See what message and value was sent if any (msg.id and msg.value) 
      // and control the robot as needed
      if (msg.id == "d1") { // joystick and also d-pad
        // The joystick control sends back two values with a comma between them
        String xString = msg.value.substring(0, msg.value.indexOf(',')); // get the first value (before the comma)
        String yString = msg.value.substring(msg.value.indexOf(',')+1,msg.value.length()); // get the second value (after the comma)
        int xValue = xString.toInt()-joyStickMid; // convert the string representation to an int and shift it so 0 is at the middle of the joystick
        int yValue = yString.toInt()-joyStickMid;
        int forwardPower = yValue * 255L / joyStickMid; // convert from -512 - 512 to -255 - 255 (see the Arduino map() function for another way to do this)
        int rotationAdjust = xValue * 255L / joyStickMid; // convert from -512 - 512 to -128 - 128
        int leftMotor = forwardPower - rotationAdjust; // adjust paddle speeds so that the bot can rotate
        int rightMotor = forwardPower + rotationAdjust;
        leftMotor = constrain(leftMotor, -254, 254); // Make sure our adjustment didn't take it out of the Arduino range
        rightMotor = constrain(rightMotor, -254, 254);
        if (debug) { // Set debug to true where it is declared above to see output of the above math.
          Serial.print("Fwd: "); Serial.print(forwardPower);
          Serial.print(", Rot: "); Serial.print(rotationAdjust);
          Serial.print(", Lft: "); Serial.print(leftMotor);
          Serial.print(", Rgt: "); Serial.print(rightMotor);
          Serial.println(" ");
        }

        runMotor(motorA1, motorA2, leftMotor);
        runMotor(motorB1, motorB2, rightMotor);


      /************************************************************/
      }
    }
  }
}
