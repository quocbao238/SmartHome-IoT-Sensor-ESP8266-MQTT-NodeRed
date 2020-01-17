#include "IO.h"
#include "library.h"
#include "node_wifi.h"

/*  Value   */

unsigned long last_time;
unsigned long send_time;
unsigned int count = 0;
int pirvalue,vcvalue;
float mq2analog;
int vcstate = 0;
long lastMsg = 0;
char msg[50];
float dht11_temp,dht11_humi,dht22_temp,dht22_humi;
float b_dht11_temp,b_dht11_humi,b_dht22_temp,b_dht22_humi;
int readcounter = 0;
unsigned long blinktime;
char dht11_temperatureTemp[7];
char dht11_humidityTemp[7];
char dht22_temperatureTemp[7];
char dht22_humidityTemp[7];
char pir_send[7];
char mq2lgp_send[7];
char mq2co_send[7];
char mq2smoke_send[7];
char vc_send[7];
boolean run = true;
int lpg, co, smoke;

MQ2 mq2(A0);
OneButton vcsensor(VC_Sensor, true);



// void readVC();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Start");
  pin_Mode();
  setupWiFi();
  dht11.begin();
  dht22.begin();
  // mq2.begin();
  delay(2000);
  read_DHT11();
  delay(2000);
  read_DHT22();
}

void loop() {
  if (!client.connected()) 
      {
      reconnect();
      }
    client.loop();
    readSensor();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message read [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if (strcmp(topic, relay_topic) == 0)
  {
    handlingData_Relay(payload);
  }
  Serial.println();
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");      
      client.publish(relay_topic, "Reconnect");      // Gửi dữ liệu
      client.subscribe(relay_topic);                 // Theo dõi dữ liệu
    } else {
      Serial.println("failed, rc=");
      Serial.print(client.state());
      // Doi 1s
      delay(1000);
    }
  }
}


void setupWiFi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void pin_Mode(){
  pinMode(RL_1,OUTPUT);
  pinMode(RL_2,OUTPUT);
  pinMode(RL_3,OUTPUT);
  pinMode(RL_4,OUTPUT);
  pinMode(PIR_Sensor,INPUT);
  digitalWrite(RL_1,HIGH);
  digitalWrite(RL_2,HIGH);
  digitalWrite(RL_3,HIGH);
  digitalWrite(RL_4,HIGH);
  vcsensor.attachClick(readVC);
}


void read_DHT11(){
  dht11_humi = dht11.readHumidity();
  dht11_temp = dht11.readTemperature();
  float x = random(0,100);
  float y = random(0,100);
  x = x/100;
  y = y/100;
  if (isnan(dht11_humi) || isnan(dht11_temp)) {
    Serial.print(F("---"));
    // return;
  }else
  { 
    b_dht11_humi = dht11_humi + y;
    b_dht11_temp = dht11_temp + x;
  }
  Serial.println("DHT11 Temp: " + String(b_dht11_temp) + " - Humi: " + String(b_dht11_humi));

  dtostrf(b_dht11_temp, 6, 2, dht11_temperatureTemp);
  dtostrf(b_dht11_humi, 6, 2, dht11_humidityTemp);
}


void read_DHT22(){
  dht22_humi = dht22.readHumidity();
  dht22_temp = dht22.readTemperature();
  float x = random(0,100);
  float y = random(0,100);
  x = x/100;
  y = y/100;
  if (isnan(dht22_humi) || isnan(dht22_temp)) {
    Serial.print(F("---"));
    // return;
  }else
  { 
    b_dht22_humi = dht22_humi +x;
    b_dht22_temp = dht22_temp +y;
  }
  Serial.println("DHT22 Temp: " + String(b_dht22_temp) + " - Humi: " + String(b_dht22_humi));

  dtostrf(b_dht22_temp, 6, 2, dht22_temperatureTemp);
  dtostrf(b_dht22_humi, 6, 2, dht22_humidityTemp);
}


void read_PIR(){
  pirvalue =  digitalRead(PIR_Sensor);
  // pirvalue == 1 ? digitalWrite(RL_2,LOW) : digitalWrite(RL_2,HIGH);
  // Serial.print("PIR: " + String(pirvalue) + " --- ");
}

void read_MQ2(){
  mq2analog = analogRead(A0);
  smoke = mq2analog;
  dtostrf(smoke, 6, 2, mq2smoke_send);
}

void readVC() {
  digitalWrite(RL_1,!digitalRead(RL_1));
  Serial.println("Relay change.");
  // dtostrf(digitalRead(RL_1), 6, 2, pir_send);
} 

void handlingData_Relay(byte* data)
{
  switch ((char)data[0]) {
    case '0':
      digitalWrite(RL_1, LOW);
      // client.publish(relay_topic, "Relay 1 is ON");
      break;
    case '1':
      digitalWrite(RL_1, HIGH);
      // client.publish(relay_topic, "Relay 1 is OFF");
      break;
    case '2':
      digitalWrite(RL_2, LOW);
      // client.publish(relay_topic, "Relay 2 is ON");
      break;
    case '3':
      digitalWrite(RL_2, HIGH);
      // client.publish(relay_topic, "Relay 2 is OFF");
      break;
    case '4':
      digitalWrite(RL_3, LOW);
      // client.publish(relay_topic, "Relay 3 is ON");
      break;
    case '5':
      digitalWrite(RL_3, HIGH);
      // client.publish(relay_topic, "Relay 3 is OFF");
      break;
    case '6':
      digitalWrite(RL_4, LOW);
      // client.publish(relay_topic, "Relay 4 is ON");
      break;
    case '7':
      digitalWrite(RL_4, HIGH);
      // client.publish(relay_topic, "Relay 4 is OFF");
      break;
  }
}


void readSensor()
{ 
  /* Vc Sensor */
  vcsensor.tick();
  /* PIR Sensor*/
  read_PIR();
  /* MQ2 Sensor*/
  read_MQ2();

  if(millis() - last_time > 2000 ){
    readcounter  == 0 ? read_DHT11() : read_DHT22();
    readcounter  < 1 ? readcounter++ : readcounter = 0;
    last_time = millis();
  }

  /* VC Value  - PIR  - MQ2 */
  if(millis() - send_time > 100){
    switch (count)
    {
    case 0:
      if(pirvalue == 0){
      // client.publish(pir_topic, "There is some one in my shop");
      client.publish(pir_topic, "ON");
      }else{
      // client.publish(pir_topic, "There is no one in my shop ");
      client.publish(pir_topic, "OFF");
      }
      // Serial.println("Send 2");
      break;
    case 1:
      client.publish(mq2_topic_smoke, mq2smoke_send);
      // Serial.println("Send 5");
      break;
    case 2:
      client.publish(dht11_temp_topic, dht11_temperatureTemp);
      // Serial.println("Send 6");
      break;
    case 3:
      client.publish(dht11_humi_topic, dht11_humidityTemp);
      // Serial.println("Send 7");
      break;    
    case 4:
      client.publish(dht22_temp_topic, dht22_temperatureTemp);
      // Serial.println("Send 8");
      break;
    case 5:
      client.publish(dht22_humi_topic, dht22_humidityTemp);
      // Serial.println("Send 9");
      break;    
    case 6:
      if(digitalRead(RL_1) == LOW){
        client.publish(vc_topic, "ON");
      }else{
        client.publish(vc_topic, "OFF");
      }
      break;
    default:
      break;
    }
    count < 6 ? count ++ : count = 0;
    send_time =  millis();
  }
  yield();
}