/////////////////////////////////////////////////////////////////
// Water Game Hello World Sketch
// 1) Install the Arduino IDE and launch it
// 2) Connect your Arduino board to your PC with a USB cable
// 3) Choose your board on the drop down list
// 4) Wait for the "core" Board libraries to install
// 5) Copy this entire sketch into the IDE
// 6) Click the "Upload" button (right arrow) on the toolbar
// 7) Open the Serial Monitor on the Tools menu
// 8) You should see output counting seconds
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
// setup()  This method is called once when your sketch first 
//          starts to run.  Initialize things here
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Set up the Serial communication over the USB cable at a baud rate that works everywhere
}

/////////////////////////////////////////////////////////////////
// loop()  This method is called over and over while your program
//         continues to run
void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0;i<1000;i++) { // a "for" loop that will execute 1000 times
    Serial.print("Hello World: "); // write the string of characters to the Serial Monitor
    Serial.println (i); // write the value of the local variable i to the Serial Monitor with a carrige return
    delay(1000); // wait for 1 second (1000 millisecs)
  }
}
