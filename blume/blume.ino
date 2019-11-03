
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// Update these with values suitable for your network.
int leds = 71;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(leds, 2, NEO_GRB + NEO_KHZ800);
const char* ssid = "verschwoerhaus-legacy";
const char* password = "mitcodedieweltverbessern";
const char* mqtt_server = "192.168.9.124";
int analogPin = A0;
int maxW = 0;
int digitalPin = D1;
int zeit = 0;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("escaperoom/windrad", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(digitalPin, OUTPUT);
  pixels.begin();
}

void loop() {

  if (!client.connected()) {
   /////////////////////////////////////////////// reconnect();
  }
  client.loop();

int wert = analogRead(analogPin);
  if(wert>maxW){
    maxW = wert;
    Serial.println(wert);
  }
  if(400>maxW){
    if (millis()-zeit>1000){
      if (millis()-zeit>2000){
        digitalWrite(digitalPin,HIGH);
        zeit = millis();
      }else{

        
       digitalWrite(digitalPin,LOW);
    }
  }
  client.publish("escaperoom/blume", "nein");
  }else{
    digitalWrite(digitalPin,LOW);
    client.publish("escaperoom/blume", "ja");
    delay(2000);
  }
  int mapMin = 500;
  int mapMax = 700;
//  int a = Math.round((wert - mapMin)*((mapMax - mapMin) / leds));
  int a = map(wert, mapMin, mapMax, 0, leds);
  Serial.print(wert);
  Serial.print(", ");
  Serial.println(a);
  for(int i = 0; i < 30; i++){
    if(i < a){
      pixels.setPixelColor(i,pixels.Color(0,255,0));
    }else{
      pixels.setPixelColor(i,pixels.Color(0,0,0));
    }
  }

pixels.show();
}
