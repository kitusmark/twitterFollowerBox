
#include <SPI.h>
#include <Ethernet.h>

byte macAddress[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01};
IPAddress ip(192,168,1,111);

//Inicializamos una instancia de la libreria
EthernetClient client;

const unsigned long intervaloPeticiones = 30000; //delay entre peticiones al servidor

char nombreServidor[]= ""; //URL
unsigned long tiempoUltimoIntento = 0;
String lineaActual = "";
String followers = "";
boolean leyendoFollowers = false; //Si actualmente estamos leyendo el tweet
//======================================================================================================
void setup() {
  // primero reservamos espacio para las Strings
  lineaActual.reserve(256);
  followers.reserve(100);
  
  //Abrimos el puerto serie y esperamos a que se abra correctamente
  Serial.begin(9600);
  while (!Serial) {
   ; //Simplemente esperamos 
  }

  //Intentamos conectar mediante DHCP
  Serial.println("Intentando obtener una dirección IP mediante DHCP:");
  if (!Ethernet.begin(macAddress)) {
      Serial.println("La conexión mediante DHCP ha fallado. Intentandolo manualmente..."); 
      Ethernet.begin(macAddress, ip);
  }
  Serial.println("La dirección asignada es:");
  Serial.println(Ethernet.localIP());
  
  //Conectar al servidor de Twitter
  conectarServidor();

}
//=======================================================================================================
void loop() {
  // 
    if (client.connected()) {
    if (client.available()) {
      // read incoming bytes:
      char inChar = client.read();

      // add incoming byte to end of line:
      lineaActual += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        lineaActual = "";
      } 
      // if the current line ends with <fan_count>, it will
      // be followed by the tweet:
      if ( lineaActual.endsWith("<fan_count>")) {
        // tweet is beginning. Clear the tweet string:
        readingFbcount = true; 
        fbcount = "";
      }
      // if you're currently reading the bytes of the facebook count,
      // add them to the tweet String:
      if (readingFbcount) {
        if (inChar != '<') {
          fbcount += inChar;
        } 
        else {
          // if you got a "<" character,
          // you've reached the end of the facebo:
          readingFbcount = false;
          Serial.println(fbcount); 
          lcd.begin(16, 2);
          lcd.print("Boing Boing likes");
          lcd.setCursor(0,7);
          lcd.print(fbcount);
  
          // close the connection to the server:
          client.stop(); 
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

void conectarServidor() {
  //intentar conectar
  Serial.println("Conectando al servidor...");
  String contenido = "";
  
  if(client.connect(nombreServidor,80)) {
     Serial.println("Haciendo la petición HTTP");
     //HTTP GET petición a twitter
     client.println("GET HTTP/1.1"); 
     //Declaramos el servidor correcto
     client.println("HOST: " + contenido.concat(nombreServidor));
     client.println();
  }
  
  // note the time of this connect attempt:
  tiempoUltimoIntento = millis();
}
