#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

// maximum received message length 
#define MAX_MSG_LEN (128)

// Wifi configuration
const char* ssid = "Viciphone";
const char* password = "23232323";

const uint16_t port = 8000;
const char * host = "172.20.10.13";

// MQTT Configuration
// if you have a hostname set for the MQTT server, you can use it here
// const char *serverHostname = "raspberrypi";fs
// otherwise you can use an IP address like this
 const IPAddress serverIPAddress(172, 20, 10, 13);
// the topic we want to use
const char *topic = "test/message";

int verifier = 0;


WiFiClient espClient;
PubSubClient client(espClient);
WiFiServer wifiServer(8000);

void setup() {
  Serial.begin(115200);
  connectWifi();
  if(verifier != 0){
  
      // connect to MQTT server  
    client.setServer(serverIPAddress, 1883);
    client.setCallback(callback);
  }
  else{
    sendFog();
  }
}

void loop() {
   
  if(verifier != 0){
    if (!client.connected()) {
        connectMQTT();
      }
      // this is ESSENTIAL!
      client.loop();
      // idle
      //delay(500);
    }
  else{
    recvFog();
  }
}

void sendFog(){

  wifiServer.begin();
  WiFiClient server;
  
  while (!server.connect(host, port)) {

      Serial.println("Connection to host failed");

      delay(1000);
      return;
  }


  Serial.println("Connected to server successful!");

  server.print("Hello from ESP32!");

  Serial.println("Disconnecting...");
  server.stop();
}

void recvFog(){
  
  WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        Serial.write(c);
      }
      //delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}


void connectWifi() {
  delay(10);
  // Connecting to a WiFi network
  Serial.printf("\nConnecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected on IP address ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  // Wait until we're connected
  while (!client.connected()) {
    // Create a random client ID
    String clientId = "ESP8266-";
    clientId += String(random(0xffff), HEX);
    Serial.printf("MQTT connecting as client %s...\n", clientId.c_str());
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
      // Once connected, publish an announcement...
      client.publish(topic, "hello from ESP8266");
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      Serial.printf("MQTT failed, state %s, retrying...\n", client.state());
      // Wait before retrying
      delay(2500);
    }
  }
}

void callback(char *msgTopic, byte *msgPayload, unsigned int msgLength) {
  // copy payload to a static string
  static char message[MAX_MSG_LEN+1];
  if (msgLength > MAX_MSG_LEN) {
    msgLength = MAX_MSG_LEN;
  }
  strncpy(message, (char *)msgPayload, msgLength);
  message[msgLength] = '\0';
  
  Serial.printf("topic %s, message received: %s\n", topic, message);
}
