// Includes
#include <AcceleroMMA7361.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <MsTimer2.h>


// Macros
#define MAG_ADDR  0x0E
// PIN 2 HAS TROUBLE READING GM COUNTER PULSE

// Initializers
AcceleroMMA7361 accelero;
int xa;
int ya;
int za;
int finalCount = 0;
int tempCount = 0;
int countReset = LOW;



// Sets up device to be able to transmit data
void setup()
{

  // Set up geiger counter interrupt
  pinMode(2, INPUT);
  attachInterrupt(0, inc_handler, RISING);
  MsTimer2::set(1000, timer_handler);
  
  Wire.begin();
  Serial.begin(9600);
  accelero.begin(13, 12, 11, 10, A0, A1, A2);
  accelero.setARefVoltage(3.3);                    //sets the AREF voltage to 3.3V
  accelero.setSensitivity(HIGH);                   //sets the sensitivity to +/-1.5G
  accelero.calibrate();
  config();
  
  MsTimer2::start();

  
}


// Main loop of code
void loop()
{
  // Get acceleration values
  xa = accelero.getXAccel();
  ya = accelero.getYAccel();
  za = accelero.getZAccel();
  // Accelerometer values
  Serial.print(xa);
  Serial.print(",");
  Serial.print(ya);
  Serial.print(",");
  Serial.print(za);
  Serial.print(",");
  // Magnetometer values
  Serial.print(readx()); 
  Serial.print(",");    
  Serial.print(ready());     
  Serial.print(",");    
  Serial.print(readz()); 
  // Geiger counter values
  Serial.print(",");
  Serial.println(finalCount);
    
  // If finalCount previously set then resets counting process of geiger counter
  if(countReset == HIGH)
  {

    // Resets count to zero
    tempCount = 0;
  
    // Sets countState to prepare for geiger counter measurement
    countReset = LOW;
    
     // Starts back up timer
     MsTimer2::start();
    
    // Re-enable interrupts
    interrupts();
  }
  
  
  delay(50);                                     //make it readable

}


// Configures device to read from magnetometer
void config(void)
{
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x11);              // cntrl register2
  Wire.write(0x80);              // send 0x80, enable auto resets
  Wire.endTransmission();       // stop transmitting
  
  delay(15);
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x10);              // cntrl register1
  Wire.write(0x01);              // send 0x01, active mode
  Wire.endTransmission();       // stop transmitting
}



// Read magnetometer X value
int readx(void)
{
  int xl, xh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x01);              // x MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x02);              // x LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xl = Wire.read(); // receive the byte
  }
  
  int xout = (xl|(xh << 8)); //concatenate the MSB and LSB
  return xout;
}


// Read magnetometer Y value
int ready(void)
{
  int yl, yh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x03);              // y MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x04);              // y LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yl = Wire.read(); // receive the byte
  }
  
  int yout = (yl|(yh << 8)); //concatenate the MSB and LSB
  return yout;
}


// Read magnetometer Z value
int readz(void)
{
  int zl, zh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x05);              // z MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x06);              // z LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zl = Wire.read(); // receive the byte
  }
  
  int zout = (zl|(zh << 8)); //concatenate the MSB and LSB
  return zout;


// the setup routine runs once when you press reset:

  // initialize serial communication at 9600 bits per second:


// the loop routine runs over and over again forever:

  // read the input on analog pin 0:
  int sensorValue = analogRead(A4);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(20);        // delay in between reads for stability
}

// Interrupt handler to increment counts per second variable
void inc_handler(void)
{
   tempCount = tempCount + 1;
}

// Interrupt handler to wirelessly send
void timer_handler(void)
{
  // Temporarily disable interrupts to prepare for transmission of geiger data
  noInterrupts();
  
  // Stop Timer  
  MsTimer2::stop();

  // Sets final count equal to number of clicks in past second
  finalCount = tempCount;
  
  // sets countReset so that count resets after being printed
  countReset = HIGH;
}
