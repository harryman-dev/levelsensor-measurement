extern "C" {
#include "user_interface.h"                     // for the RTC memory read/write functions
}
#include <ESP8266WiFi.h>                        // WiFi library
#include <PubSubClient.h>                       // MQTT library
#include <Adafruit_ADS1X15.h>                   // ADS1115 library

const char* SSID = "<<SSID>>";                        // WiFi SID
const char* PASSWORD = "<<Password>>";      // WiFi Password
const char* MQTT_BROKER = "xx.xx.xx.xx";        // IP address MQTT broker

const char* clientId = "<<Name>>";                  // name of this device = unique client identifier

typedef struct {                                // prepare rtc memory
  int16_t Voltage;
} rtcStore;

rtcStore rtcMem;

int16_t adc0;
int16_t volts0;

                                                 // Set your Static IP address, gateway, subnet
IPAddress ip(xx, xx, xx, xx);                    // IP address of this device (Wemos D1 mini)
IPAddress gateway(xx, xx, xx, xx);              // IP address gateway (e.g. router)
IPAddress subnet(255, 255, 255, 0);

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_ADS1115 ads;

void setup_wifi() {                               // try to start WiFi

  Serial.println("Connecting to WiFi");

  WiFi.mode(WIFI_STA);                            // station mode = connect to existing access point (e.g. router)
  WiFi.begin(SSID, PASSWORD);

                                                  // Configures static IP address
  if (!WiFi.config(ip, gateway, subnet)) {
    Serial.println("STA failed to configure");
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt() {                               // try to connect to an existing MQTT broker
  client.setServer(MQTT_BROKER, 1883);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect(clientId)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);                                // Wait 5 seconds before retrying
    }
  }
}

void setup_ads1115() {                            // setup ADS1115 (set input range to GAIN_ONE)
  ads.setGain(GAIN_ONE);                          // 1x gain   +/- 4.096V  1 bit = 0.125mV
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    delay(1000);
    while (1);
  }
  Serial.println("ADS connected");
}

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);                   // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  Serial.println("Start");
 
  system_rtc_mem_read(64, &rtcMem, 2);            // read last value from RTC
  Serial.print("lastValue: ");
  Serial.println(rtcMem.Voltage);

  setup_ads1115();

  adc0 = ads.readADC_SingleEnded(0);              // get current value via ADS1115
  volts0 = ads.computeVolts(adc0)*10000;          // prepare current value for comparison 
  
  if (abs(rtcMem.Voltage-volts0)>100) {           // if delta > 100 (0,01V) => start wifi + mqtt and publish new value
    setup_wifi();
    setup_mqtt();
    delay(1000);
                                                  // prepare payload to publish (payload = JSON array)
    String payload = "{\"Voltage\":" + String(float(volts0)/10000,4) + ",";
    payload += "\"RSSI\":" + String(WiFi.RSSI()) + "}";
  
                                                  // publish value (topic, payload)
    client.publish("/aussen/levelsensor", payload.c_str());
    delay(1000);

    rtcMem.Voltage = volts0;                      // write new value to RTC
    system_rtc_mem_write(64, &rtcMem, 2);
    Serial.print("newValue: ");
    Serial.println(rtcMem.Voltage);
 }

  Serial.println("go to sleep");

//  ESP.deepSleep(5 * 1000000, WAKE_NO_RFCAL);      // go back to deep sleep for 5 sec (test)
  ESP.deepSleep(15 * 60 * 1000000, WAKE_NO_RFCAL);  // go back to deep sleep for 15 min (prod)
}

void loop() {}                                      // no loop needed in this sketch
