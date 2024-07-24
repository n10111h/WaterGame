/////////////////////////////////////////////////////////////////
// Water Game Uno R4 Hello Bluetooth Sketch
// 1) Connect your Arduino board to your PC with a USB cable
// 2) Choose your board on the drop down list
// 3) Copy this entire sketch into the IDE
// 4) Edit the bluetooth name string (line 23) to something unique to your team, 
//    add something else if you have more than one board (e.g. A, B, etc)
// 5) Click the "Upload" button (right arrow) on the toolbar
// 6) Open the Serial Monitor (tools menu) if it is not already open
// 
//  Use your MicroBlue App on your phone to connect to your board and send commands.
//  You should see the message ID and Values in the Serial Monitor
//  If you just changed the name (step 4 above) you may need to power cycle 
//  your Arduino and restart your app a few times to get the name update to occur
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
const char *name = "3650"; // Change this to something unique to your team

// Pin values (e.g. const int motor1Forward = 1;)


// Other constants (e.g. const int maxForwardPower = 175;)

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

  // Do any other one time initialization that needs to be done for your code

  /************************************************************/
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


      /************************************************************/

    }
  }
}
