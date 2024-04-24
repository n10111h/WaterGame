/////////////////////////////////////////////////////////////////
// Water Game HM-10 Bluetooth Module Renaming Sketch
// 1) Wire your HM-10 module to your Arduino board (see Wiring Resources)
// 2) Connect your Arduino board to your PC with a USB cable
// 3) Choose your board on the drop down list
// 4) Copy this entire sketch into the IDE
// 5) Edit the pin numbers (rx_pin & tx_pin) if needed (lines 20,21)
// 6) Edit the bluetooth name string (line 24) to something unique to your team, add something else if you have more than one board
// 7) Click the "Upload" button (right arrow) on the toolbar
// 
//  Use your Bluetooth App on your phone to confirm that the name has been changed.
//  You may need to power cycle your Arduino and restart your app a few times to get this update to occur
//
/////////////////////////////////////////////////////////////////

// Include Libraries
#include <SoftwareSerial.h> // To communicate with the HM-10 we use the Arduino SoftwareSerial library
                            // Note: There are other libraries that may peform better but we have not
                            // done extensive testing with them.  Search for "SoftwareSerial replacement"

// Declare pins
int rx_pin = 7; // This should be wired to the TX pin on the HM-10
int tx_pin = 8; // This should be wired to the RX pin on the HM-10

// Bluetooth name (Max 12 characters)
const char *name = "19743A"; // Change this to something unique to your team

// Initialize Object
SoftwareSerial BTSerial(rx_pin, tx_pin); // set up the serial communication with the HM-10

/////////////////////////////////////////////////////////////////
// setup()  This method is called once when your sketch first
//          starts to run.  Initialize things here
void setup() {
  // Start bluetooth serial at 9600 bps
  BTSerial.begin(9600);
  while (!BTSerial) {} // wait for it to get started

  // Rename device to xxxxxx (The maximum length for a name is 12 characters)
  BTSerial.write("AT+NAME"); // send rename command
  BTSerial.write(name);     // send new name
}



/////////////////////////////////////////////////////////////////
// loop()  This method is called over and over while your program
//         continues to run
void loop() {
  // Nothing to do, renaming handled by setup()
}
