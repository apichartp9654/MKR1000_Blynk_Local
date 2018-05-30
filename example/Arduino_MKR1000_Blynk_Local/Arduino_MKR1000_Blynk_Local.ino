/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino MKR1000
  to connect your project to Blynk.

  Note: This requires WiFi101 library
    from http://librarymanager/all#WiFi101

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <WiFi101.h>
#include <BlynkSimpleMKR1000.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "yourtoken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "yourssid";
char pass[] = "yourpassword";

BlynkTimer timer; // Create a Timer object called "timer"! 
WidgetLED led0(V5);
WidgetLED led1(V6);
WidgetTerminal terminal(V3);
bool startflag = false;
float offset = 0;

void setup()
{
  // Debug console
  Serial.begin(115200);

  //Blynk.begin(auth, ssid, pass); // default online server
  
  Blynk.begin(auth, ssid, pass, IPAddress(xxx,xxx,xxx,xxx), 8080); // local server
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  delay(200);
  timer.setInterval(1000L, sendUptime); //  Here you set interval (1sec) and which function to call 
  terminal.flush();
}

void sendUptime()
{

  if(!startflag)
  {
    return;
  }
  // This function sends Arduino up time every 1 second to Virtual Pin (V5)
  // In the app, Widget's reading frequency should be set to PUSH
  // You can send anything with any interval using this construction
  // Don't send more that 10 values per second

  float v = random(0, 2000)/100.0;
  v = v + offset;
  Blynk.virtualWrite(V4, v);
  //Serial.println(v);

  String vdata = "val = " + String(v, 3);
  if(v < 0.0)
  {
    led0.on();
    //terminal.print ("val = ") ;
    //terminal.print (v) ;
    vdata = vdata + " X < LL";
    terminal.println(vdata) ;
  }
  else if(v > 15.0)
  {
    led1.on();
    //terminal.print ("val = ") ;
    //terminal.print (v) ;
    //terminal.println(" X > UL") ;
    vdata = vdata + " X > UL";
    terminal.println(vdata) ;
  }
  else
  {
    led0.off();
    led1.off();
    terminal.println(vdata) ;
    
   // terminal.print ("val = ") ;
   // terminal.println (v) ;
  }


  
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();

  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);

  // Let's write your hardware uptime to Virtual Pin 2
  Serial.println("Sync all state.");
   terminal.println ("") ;
   terminal.println("Sync all state.");
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asFloat(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  if(pinValue == 1)
  {
   terminal.println ("Start button is pressed.") ;
   startflag = true;
   Serial.println("Start button is pressed.");
  }
  else
  {
    terminal.println ("Start button is released.") ;
    startflag = false;
    Serial.println("Start button is released.");
  }
  
}

BLYNK_WRITE(V2)
{

  
  float pinValue = float(param.asDouble()); // assigning incoming value from pin V1 to a variable
  offset = pinValue;
  String str = "offset change to " + String(offset, 1) ;
  Serial.println(str);
  terminal.println (str) ;
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  //float b = float pinValue
}

bool firstTime = true;

void loop()
{
  Blynk.run();

  if(firstTime)
  {
    terminal.println ("") ;
    terminal.println("MK1000 connected to blynk local server.");
    Serial.println("MK1000 connected to blynk local server.");
    firstTime = false;
  }
  timer.run(); // BlynkTimer is working...
}

