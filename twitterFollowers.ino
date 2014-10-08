/*
  TwitterFollowerBox by Marc Cobler
  
  This skecth connects to a server running some php code which gets the Twitter Followers 
  out of a public account. Then we display the number to a 7 segment display. Like the good
  old days.
  It's important to read how I set up my server in order to decode the data
  to learn more go to http://github.com/kitusmark/twitterFollowerBox
  I'm using an Arduino UNO + Ethernet Shield + some 7 segments LEDs
  
  
  created 8 oct 2014
  Marc Cobler
*/
#include <SPI.h>
#include <Ethernet.h>

//Assign a MAC address and a IP. Check this if system not working properly
byte macAddress[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01};
IPAddress ip(192,168,1,22);

//Create an EthernetClient object
EthernetClient server;
//delay between server requests, X seconds.
const unsigned long requestInterval = 20000; 
//Set the server name and store it in flash, char array. URL
const char serverName[] = "http://jfmc.synology.me";

unsigned long lastAttemptTime = 0; //Last time connected to the server in milliseconds
String buffer = "";                //String to read from the html info from the server
String followers = "";            //String to hold the important data
boolean readingFollowers = false; //If we are currently reading the twitter followers data
String serverDirection = "";
//======================================================================================================
void setup() {
  //Convert the char Array of serverName to a String
  unsigned int i;
  for(i=0; i<= 22; i++)
    {
      serverDirection += serverName[i];
    }
 
  // we reserve some space in memory in order to manipulate the strings
  buffer.reserve(100);
  followers.reserve(10); 
  
  //Open a Serial port and wait to be ready. Only for Leonardo
  Serial.begin(9600);
  while (!Serial) {
   ; //Simply do nothing 
  }
  Serial.println(serverDirection);
  //We try some DHCP connection
  Serial.println("trying to get an IP using DHCP:");
  if (!Ethernet.begin(macAddress)) {
      Serial.println("the DHCP connection failed, trying manually..."); 
      Ethernet.begin(macAddress, ip);
  }
  Serial.println("Good! we got a valid address: ");
  Serial.println(Ethernet.localIP());
  
  //Connect to the server
  connectToServer();

}
//=======================================================================================================
void loop() {
  // 
    if (server.connected()) {
    if (server.available()) {
      // read incoming bytes:
      char inChar = server.read();

      // add incoming byte to end of line:
      buffer += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        buffer = "";
      } 
      // if the current line ends with '<', it will
      // be followed by the magic number we're waiting for:
      if ( buffer.endsWith("<")) {
        //useful info is beginning. Clear the followers string:
        readingFollowers = true; 
        followers = "";
      }
      // Once we know we're reading the followers, we save the number
      if (readingFollowers) {
        if ((inChar != '<') | (inChar != '>')) {
          followers += inChar;
        } 
        else {
          // if you got a "<" character,
          // you've reached the end of the facebo:
          readingFollowers = false;
          Serial.println("You're followed by: ");
          Serial.println(followers + "people"); 
          // close the connection to the server:
          server.stop(); 
        }
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }

}
//==============================FUNCTIONS=====================================
void connectToServer() {
  //connecting to the server via Internet
  Serial.println("Connecting to the specified Server...");
  
  if(server.connect(serverName,80)) {
     Serial.println("Making the HTTP request...");
     //HTTP GET request to the specified server
     server.println("GET /arduinoFollowersBox/arduinoFollowersBox.php HTTP/1.1"); 
     server.println("HOST: " + serverDirection);
     server.println("Connection: close");
     server.println();
  }
  Serial.println("Success");
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}
