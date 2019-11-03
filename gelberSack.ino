/*
 *  Created by TheCircuit
*/

//192.168.9.124

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
   #include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

bool richtig = false;

const char *mqttServer = "192.168.9.124";
// const char *mqttServer = "192.168.9.64";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
int statuss = 0;
int out = 0;

String lastSent = "";

void wifi_connect()
{
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_connect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("muell"))
    {

      Serial.println("connected");
    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522

  WiFi.begin("verschwoerhaus-legacy", "mitcodedieweltverbessern");
  wifi_connect();
  client.setServer(mqttServer, mqttPort);
}

void loop()
{
  mqtt_connect();

  if (richtig){
    client.publish("escaperoom/muell/gelberSack", "ja");
  }else{
    client.publish("escaperoom/muell/gelberSack", "nein");
  }

  delay(50);

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    richtig = false;
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println(content);

  content.toLowerCase();
  if(content.indexOf("e3") > -1){
    richtig = true;
  }else{
    richtig = false;
  }
}
