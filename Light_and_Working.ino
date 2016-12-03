/*
  

  Program Description: this program is used to program a robotic car. The Arduino Mega communicates with an Android Phone via bluetooth. 
  The blutooth module sends commands from the user to be preformed by the car. The car is equiped with a VCNL 4000 Proximity Sensor, Parallax Laser Range Finder, 
  bluetooth module, LED light, and 3 motors. Two motors for the wheels and one for the Parallax Laser Range Finder. The car is programmed to stop moving upon 
  receiving information for the proximity sensors that an object is too close. Also, the car has an LED light that turns on automatically in the dark but that can 
  also be programed manually from the app. 

  Last Modified: 11/23/2016 
  By: Julia Roscher and Matthew Reilly
    
  
*/

// include the SoftwareSerial library so we can use it to talk to the LRF (Laser Range Finder)
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>

 
// the i2c address
#define VCNL4000_ADDRESS 0x13
 
// commands and constants
#define VCNL4000_COMMAND 0x80
#define VCNL4000_PRODUCTID 0x81
#define VCNL4000_IRLED 0x83
#define VCNL4000_AMBIENTPARAMETER 0x84
#define VCNL4000_AMBIENTDATA 0x85
#define VCNL4000_PROXIMITYDATA 0x87
#define VCNL4000_SIGNALFREQ 0x89
#define VCNL4000_PROXINITYADJUST 0x8A
 
#define VCNL4000_3M125 0
#define VCNL4000_1M5625 1
#define VCNL4000_781K25 2
#define VCNL4000_390K625 3
 
#define VCNL4000_MEASUREAMBIENT 0x10
#define VCNL4000_MEASUREPROXIMITY 0x08
#define VCNL4000_AMBIENTREADY 0x40
#define VCNL4000_PROXIMITYREADY 0x20


#define BUFSIZE  16  // Size of buffer (in bytes) for incoming data from the LRF (this should be adjusted to be larger than the expected response)

// servos
Servo lrfServo;     // servo for the laser range finder
Servo servoRight;  // Declare right servo
Servo servoLeft;    //Declare left servo




  char lrfData[BUFSIZE];  // Buffer for incoming data

  //lights up in dark or when told too
  int lightbulb = 10;                  // LED connected to digital pin 10
  unsigned int ambient;
  unsigned int prox;

  //variabled used to track if LED is on, if "Light" button was pressed, and if autolight functionality is active
  boolean litAF = false;
  boolean autolight = true;
  boolean light = false;


void setup()  // Set up code called once on start-up
{
 
  pinMode(lightbulb, OUTPUT);        // sets the digital pin as output
  digitalWrite(lightbulb, LOW);      // sets the LED off
  
  
  // setup Arduino Serial Monitor
  Serial.begin(9600); //set baud rate for Serial port
  while (!Serial);   // Wait until ready
  
  Serial.println("\n\nParallax Laser Range Finder");
  
  // set the baud rate for the Serial1 port, the LRF runs off this port
  Serial1.begin(9600);

  //Set up for Proximity Sensor
  Serial.println("VCNL");
  Wire.begin();
 
  uint8_t rev = read8(VCNL4000_PRODUCTID);
 
  if ((rev & 0xF0) != 0x10) {
    Serial.println("Proximity Sensor not found :(");
    while (1);
  }
 /*
  write8(VCNL4000_IRLED, 20);        // set to 20 * 10mA = 200mA
  Serial.print("IR LED current = ");
  Serial.print(read8(VCNL4000_IRLED) * 10, DEC);
  Serial.println(" mA");
 
  Serial.print("Proximity measurement frequency = ");
  uint8_t freq = read8(VCNL4000_SIGNALFREQ);
  if (freq == VCNL4000_3M125) Serial.println("3.125 MHz");
  if (freq == VCNL4000_1M5625) Serial.println("1.5625 MHz");
  if (freq == VCNL4000_781K25) Serial.println("781.25 KHz");
  if (freq == VCNL4000_390K625) Serial.println("390.625 KHz");
 
  write8(VCNL4000_PROXINITYADJUST, 0x81);
  Serial.print("Proximity adjustment register = ");
  Serial.println(read8(VCNL4000_PROXINITYADJUST), HEX);
 
    
  /*
    When the LRF powers on, it launches an auto-baud routine to determine the
    host's baud rate. it will stay in this state until a "U" ($55) character
    is sent by the host. 
  */
  Serial.print("Waiting for the LRF...");
  delay(2000);                        // Delay to let LRF module start up
  Serial1.print('U');               // Send character
  while (Serial1.read() != ':');    // When the LRF has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  Serial1.flush();                  // Flush the receive buffer
  Serial.println("Ready!");
  Serial.flush();                     // Wait for all bytes to be transmitted to the Serial Monitor


  delay(1000);
  lrfServo.attach(11); //Servo for Laser Range Finder
  lrfServo.writeMicroseconds(1400); // move laser to center


  // start servos
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoRight.writeMicroseconds(1500);        // 1.5 ms stop


  servoLeft.attach(13);                     // Attach left signal to pin 13
  servoLeft.writeMicroseconds(1500);    

}

void loop() {
  // --------------------------
  // serial message validation
  // --------------------------
 // Serial.println("Getting to the loop");
 // Serial.println("Value of exitLoop");
  delay(50);


   String t; //create an empty string to store messages from Android
  while(Serial.available()) { //keep reading bytes while they are still more in the buffer
    t += (char)Serial.read(); //read byte, convert to char, and append it to string
    Serial.println(t);
  }

 // Serial.println("Sweep command received");

  

    //Move Motors
      //servoLEFT = 1000 = right
      //servoRIGHT = 1000 = left
      //seroLEFT = 2000 = backwards
      //servoRIGHT = 2000 = forwards
      //else = 0

   /*   
      servoLeft.write(0);
      servoRight.write(2000);
      //left wheeel above forward 1600
      //right wheel backwared below 1500
      servoLeft.read();
      servoRight.read();
      */
 
 // while(t != "s"){
      //Stop sent from Phone
      if (t == "#1500,1500$") {   
      Serial.write("Stop");
      servoLeft.write(1500);
      servoRight.write(1500);
      servoLeft.read();
      servoRight.read();
    } 

   if (t == "#1300,1700$") {   
      Serial.write("Backward");
      servoLeft.write(1700);
      servoRight.write(1300);
      servoLeft.read();
      servoRight.read();
      Serial.println("This is suppose to be forward");
    } 

      if (t == "#1700,1300$") {   
      Serial.write("Backward");
      servoLeft.write(1300);
      servoRight.write(1700);
      servoLeft.read();
      servoRight.read();
    } 

      if (t == "#1300,1500$") {   
      Serial.write("left");
      servoLeft.write(1300);
      servoRight.write(1500);
      servoLeft.read();
      servoRight.read();
    } 
    
     if (t == "#1500,1700$") {   
      Serial.write("Right");
      servoLeft.write(1500);
      servoRight.write(1700);
      servoLeft.read();
      servoRight.read();
    } 
    
     if(ambient <= 150 && autolight == true && light == false){
      digitalWrite(lightbulb, HIGH);
      light = true;
     }

     if (t == "l" && litAF == false && autolight == true && light == true) {   
       Serial.write("lightoff");
       digitalWrite(lightbulb, LOW);     // sets the LED on
       litAF = true;
       light = false;
       autolight = false; 
       t = "pie";
    } 

    if(t == "l" && litAF == true && autolight == false && light == false){
      digitalWrite(lightbulb, HIGH);
      light = true; 
      litAF = false;
      autolight = true; 
       t = "pie";
    }

    if(ambient >= 150 && autolight == true && light == true){
      digitalWrite(lightbulb, LOW);
      light = false;
    }

    if(t == "l" && litAF == false && autolight == true && light == false){
      Serial.write("LightON");
      digitalWrite(lightbulb, HIGH);
      light = true;
      litAF = true; 
      autolight = false;
      t = "pie";
    }

    if(t == "l" && litAF == true && autolight == false && light == true){
      digitalWrite(lightbulb, LOW);
      light = false;
      autolight = true; 
      litAF = false; 
      t = "pie";
    }    

    if (t == "s") {  
     // clickable = false;  
      Serial.write("laser sweep");
      sweep();
     // Serial.println("After sweeepppppppp");
    //  t = "pie";
     // clickable = true;
    } 
  

  // read ambient light!
  write8(VCNL4000_COMMAND, VCNL4000_MEASUREAMBIENT | VCNL4000_MEASUREPROXIMITY);
 
  while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    if ((result & VCNL4000_AMBIENTREADY)&&(result & VCNL4000_PROXIMITYREADY)) {
 
      Serial.print("Ambient = ");
      ambient = read16(VCNL4000_AMBIENTDATA);
      Serial.print(ambient);
      Serial.print("\t\tProximity = ");
      prox = read16(VCNL4000_PROXIMITYDATA);
      Serial.println(prox);
      

      //if the ambient light gets below a certian level the lightbulb turns on
      //the lightbulb then turns off if the ambient light is within the normal visibility spectrum

       if(prox >= 2200 && prox != 0 ){
   
           Serial.println("YOU GO HRERE");
 
            servoLeft.write(1500);
            servoRight.write(1500);
             servoLeft.read();
             servoRight.read();
       }
     
      break;
    }

    delay(50);
}   
}

// moves the laser sensor and takes readings three times
void sweep()
{
  Serial.println("Made it to sweep");
  //Already in Center
  readProx();
 // delay(1500);
  toStop();
  lrfServo.writeMicroseconds(1400);
 // Serial.println("Back to sweep");
  loop();
  
/*
  //left
  lrfServo.writeMicroseconds(2000);
  readProx();
 // delay(1500);
  toStop();

  
  //right
  lrfServo.writeMicroseconds(900);
  readProx();
//  delay(1500);
  toStop();


  //Center
  lrfServo.writeMicroseconds(1400);
  loop();

 // exit(0);
 */ 
}

void readProx()  
{

  /* 
   *  
   *  Range goes from about 8 ft to .5 ft 
   *  
    When a single range (R) command is sent, the LRF returns the distance to the target
    object in ASCII in millimeters. For example:
     
    D = 0123 mm
  */   

  Serial.println("Made it to Read Prox");
  Serial1.print('R');         // Send command
 
  char offset = 0;        // Offset into buffer
  lrfData[0] = 0;         // Clear the buffer    
 
  while(1)
  {
    if (Serial1.available() > 0) // If there are any bytes available to read, then the LRF must have responded
    {
      lrfData[offset] = Serial1.read();  // Get the byte and store it in our buffer
      if (lrfData[offset] == ':')          // If a ":" character is received, all data has been sent and the LRF is ready to accept the next command
      {
        lrfData[offset] = 0; // Null terminate the string of bytes we just received
        break;               // Break out of the loop
      }
          
      offset++;  // Increment offset into array
      if (offset >= BUFSIZE) offset = 0; // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bounds
    }
  }
  Serial.println(lrfData);    // The lrfData string should now contain the data returned by the LRF, so display it on the Serial Monitor
  Serial.flush();             // Wait for all bytes to be transmitted to the Serial Monitor

}

void toStop(){

  String SUBstr;
  String Value; 
  int intValue; 
  Serial.println("Print me");
  Serial.println(lrfData);
  String str(lrfData); 
  Serial.println(str.substring(5,9));
  Value = str.substring(5, 9);
  intValue = Value.toInt();

  //Something is in Robots Way
  if(intValue <= 430 && intValue != 0 ){
   
    Serial.println("YOU GO HRERE");
  //  delay(3000);
     servoLeft.write(1500);
      servoRight.write(1500);
      servoLeft.read();
      servoRight.read();
  //   delay(8000);
  }
  
  Serial.println("Converted Value ");
  Serial.println(intValue);  
  Serial.println("AFTER");
  Serial.flush();
  
 }
  
uint16_t readProximity() {
  write8(VCNL4000_COMMAND, VCNL4000_MEASUREPROXIMITY);
  while (1) {
    uint8_t result = read8(VCNL4000_COMMAND);
    //Serial.print("Ready = 0x"); Serial.println(result, HEX);
    if (result & VCNL4000_PROXIMITYREADY) {
      return read16(VCNL4000_PROXIMITYDATA);
    }
    delay(1);
  }
}


// Read 1 byte from the VCNL4000 at 'address'
uint8_t read8(uint8_t address)
{
  uint8_t data;
 
  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();
 
  delayMicroseconds(170);  // delay required
 
  Wire.requestFrom(VCNL4000_ADDRESS, 1);
  while(!Wire.available());
 
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}
 
// Read 2 byte from the VCNL4000 at 'address'
uint16_t read16(uint8_t address)
{
  uint16_t data;
 
  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();
 
  Wire.requestFrom(VCNL4000_ADDRESS, 2);
  while(!Wire.available());
#if ARDUINO >= 100
  data = Wire.read();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.read();
#else
  data = Wire.receive();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.receive();
#endif
 
  return data;
}
 
// write 1 byte
void write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(VCNL4000_ADDRESS);
#if ARDUINO >= 100
  Wire.write(address);
  Wire.write(data);  
#else
  Wire.send(address);
  Wire.send(data);  
#endif
  Wire.endTransmission();
}


