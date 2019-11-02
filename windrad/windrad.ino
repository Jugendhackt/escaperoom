int analogPin = A0;
int maxW = 0;
int digitalPin = D0;
int zeit = 0;

void setup() {
 Serial.begin(9600);
 pinMode(digitalPin, OUTPUT);
}

void loop() {
  int wert = analogRead(analogPin);
  if(wert>maxW){
    maxW = wert;
    Serial.println(wert);
  }
  if(125>maxW){
    if (millis()-zeit>1000){
      if (millis()-zeit>2000){
        digitalWrite(digitalPin,HIGH);
        zeit = millis();
      }else{
       digitalWrite(digitalPin,LOW);
    }
  }
  }else{
    digitalWrite(digitalPin,LOW);
    delay(2000);
  }
}
