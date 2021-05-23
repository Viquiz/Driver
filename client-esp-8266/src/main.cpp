#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define CHANNEL 0

const char *serverSSID = "1234";
uint8_t serverMAC[6] = {0x7c, 0x9e, 0xbd, 0xf4, 0x04, 0x70};

bool scanForServer();

void setup()
{
  Serial.begin(115200);
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: ");
  Serial.println(WiFi.macAddress());
  WiFi.disconnect();
  if (!esp_now_init())
  {
    Serial.println("ESPNow Init Success");
  }
  else
  {
    Serial.println("ESPNow Init Failed");
    delay(1000);
    ESP.restart();
  }
  while (!scanForServer())
    delay(500);
  WiFi.begin(serverSSID, "123456789", CHANNEL, serverMAC, false);
  esp_now_add_peer(serverMAC, ESP_NOW_ROLE_MAX, CHANNEL, NULL, 0);
}

char *data = "hi from 8266";

void loop()
{
  Serial.println("Sending");
  esp_now_send(serverMAC, (uint8_t *)data, strlen(data));
  delay(2000);
}

bool scanForServer()
{
  uint8_t scanResults = WiFi.scanNetworks();
  if (scanResults)
  {
    Serial.print("Found ");
    Serial.print(scanResults);
    Serial.println(" devices ");
    for (int i = 0; i < scanResults; i++)
    {
      // Found server
      if (strcmp(WiFi.SSID(i).c_str(), serverSSID) == 0)
      {
        Serial.print("Server MAC: ");
        // memcpy(serverMAC, WiFi.SSID(i).c_str(), 6);
        char macStr[18];
        snprintf(macStr, sizeof(macStr), MACSTR, MAC2STR(serverMAC));
        Serial.println(macStr);
        return true;
      }
    }
  }
  Serial.println("No Server found");
  return false;
}