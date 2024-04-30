/*
 ******************************************************************
 * Find Servo Positions for R4 Wifi
 *
 * 1) Build your mechanism, wire up your servo, and detach the servo horn from the servo
 * 1.5) Edit line 17 below to match the pin your servo is getting a PWM signal from
 * 2) Download and run this sketch on your R4 Uno.  The Servo should immediately go to its center position
 * 3) Connect MicroBlue and make sure you have two buttons ("bup" and "bdown")
 * 4) reconnect your servo horn in the middle of the range of travel you want
 * 5) Use the up and down buttons to move the servo to various positions, note the values on the LED display
 *
 * https://snappyxo.io/
 *
 ******************************************************************
*/

const int servoPin = 3; // Change this to the right pin for your build
int position = 255/2; // start in the middle

// Include libraries
#include <ArduinoBLE.h>
//#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;
const uint8_t digits[10][5][3] = { // set up a matrix of all 10 digits at 3x5 resolution
 {{ 0, 1, 0 }, // 0
  { 1, 0, 1 },
  { 1, 0, 1 },
  { 1, 0, 1 },
  { 0, 1, 0 }},
 {{ 0, 1, 0 }, // 1
  { 0, 1, 0 },
  { 0, 1, 0 },
  { 0, 1, 0 },
  { 0, 1, 0 }},
 {{ 0, 1, 0 }, // 2
  { 1, 0, 1 },
  { 0, 1, 0 },
  { 1, 0, 0 },
  { 1, 1, 1 }},  
 {{ 0, 1, 0 }, //3
  { 1, 0, 1 },
  { 0, 1, 1 },
  { 1, 0, 1 },
  { 0, 1, 0 }},
 {{ 0, 0, 1 }, // 4
  { 0, 1, 1 },
  { 1, 1, 1 },
  { 0, 0, 1 },
  { 0, 0, 1 }},
 {{ 1, 1, 1 }, // 5
  { 1, 0, 0 },
  { 1, 1, 0 },
  { 0, 0, 1 },
  { 1, 1, 0 }},
 {{ 0, 1, 1 }, // 6
  { 1, 0, 0 },
  { 1, 1, 0 },
  { 1, 0, 1 },
  { 0, 1, 0 }},
 {{ 1, 1, 1 }, // 7
  { 0, 0, 1 },
  { 0, 1, 0 },
  { 0, 1, 0 },
  { 0, 1, 0 }},
 {{ 0, 1, 0 }, // 8
  { 1, 0, 1 },
  { 0, 1, 0 },
  { 1, 0, 1 },
  { 0, 1, 0 }},
 {{ 0, 1, 0 }, // 9
  { 1, 0, 1 },
  { 0, 1, 1 },
  { 0, 0, 1 },
  { 1, 1, 0 }},
};

uint8_t frame[8][12] = { // This is our frame buffer for the LED matrix
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Object Initialization
BLEService CustomService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic CustomCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214",
                                       BLERead | BLENotify | BLEWrite, 100);

struct BLEMessage {
  String id = "";
  String value = "";
};

struct BLEMessage getBLEMessage() {
  BLEMessage msg;
  // Read id and value if detect signature code 1
  if (CustomCharacteristic.written()) {
    uint8_t charCodes[100];
    int valueLength = CustomCharacteristic.valueLength();
    CustomCharacteristic.readValue(charCodes, valueLength);
    if (charCodes[0] == 1) {
      int i = 1;
      while (i < valueLength - 1 && charCodes[i] != 2) {
        msg.id.concat((char)charCodes[i]);
        i++;
      }
      i++;
      while (i < valueLength - 1 && charCodes[i] != 3) {
        msg.value.concat((char)charCodes[i]);
        i++;
      }
    }
  }
  // Print id and value if Serial exist
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

void setup() {
  //set up the motors and pwms to run
  pinMode(servoPin, OUTPUT);

  // Begin Serial Communication
  Serial.begin(9600);
  // Begin Initialization
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy failed!");
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("FindServoPos");
  BLE.setDeviceName("FindServoPos");
  BLE.setAdvertisedService(CustomService);
  // add the characteristic to the service
  CustomService.addCharacteristic(CustomCharacteristic);
  // add service
  BLE.addService(CustomService);
  // start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral, waiting for connections....");
  matrix.begin();

  analogWrite(servoPin, position);
  setFrame(position);
  matrix.renderBitmap(frame, 8, 12);
}

void clearFrame() {
    for (int i = 0;i<8;i++)
      for (int j = 0;j<12;j++)
        frame[i][j] = 0;
}
// stamp the specified digit onto the frame buffer at the specified location
void addDigit(int x, int y, int digit) {
    for (int r = 0;r<05;r++) {
      for (int c = 0;c<3;c++){
        frame[r+y][c+x] = digits[digit][r][c];  
      }
    }
}

//Write a 3 digit number onto the frame
void setFrame(int num) {
  clearFrame();
  int hundreds = (num / 100);
  int tens = (num / 10) % 10 ;
  int ones = num % 10 ;
  addDigit(0,1,hundreds);
  addDigit(4,1,tens);
  addDigit(8,1,ones);
}
void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // Get BLE message
      BLEMessage msg = getBLEMessage();
    
      if (msg.id == "bup") { 
        if (msg.value.toInt() == 1) {
          if (position < 254) position++;
          analogWrite(servoPin, position);
          setFrame(position);
          matrix.renderBitmap(frame, 8, 12);
          Serial.println(position);
        }
      }
      if (msg.id == "bdown") { 
        if (msg.value.toInt() == 1) {
          if (position > 1) position--;
          analogWrite(servoPin, position);
          setFrame(position);
          matrix.renderBitmap(frame, 8, 12); // write it to the led matrix
          Serial.println(position); // also write it to the Serial Monitor
        }
      }
    }
  }
}
