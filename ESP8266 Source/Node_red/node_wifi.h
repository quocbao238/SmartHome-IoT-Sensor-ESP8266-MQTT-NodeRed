/*  Confix WiFi*/

const char* ssid = "Hshop.vn"; //Nhập tên WiFi 
const char* password = "03050709"; // Nhập Mật khẩu WiFi 
const char* mqttServer = "192.168.10.130"; // Nhập địa chỉ của server MQTT
String clientId = "hshopClient";            // Client ID của mạch

/* DHT11 */
const char* dht11_humi_topic = "Maker_sensor_dht11/humidity";      // Topic của Sensor đọc độ ẩm
const char* dht11_temp_topic = "Maker_sensor_dht11/temperature";      // Topic của Sensor đọc nhiet do
const char* dht11_topic = "Maker_sensor_dht11";  

/* DHT22 */
const char* dht22_humi_topic = "Maker_sensor_dht22/humidity";      // Topic của Sensor đọc độ ẩm
const char* dht22_temp_topic = "Maker_sensor_dht22/temperature";      // Topic của Sensor đọc nhiet do
const char* dht22_topic = "Maker_sensor_dht22";  

/* Relay */
const char* relay_topic = "Maker_relay";      // Topic của relay

/* MQ_2*/  
const char* mq2_topic_lpg = "Maker_MQ2_Lpg";
const char* mq2_topic_co= "Maker_MQ2_CO";
const char* mq2_topic_smoke = "Maker_MQ2_Smoke";

/* PIR*/  
const char* pir_topic = "Maker_PIR";
/* VC*/  
const char* vc_topic = "Maker_VC";
