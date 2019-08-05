#define _light_first 5
#define _light_second 12
#define _switch_first 14
#define _switch_second 13

#define _on 1
#define _off 0

#include <ESPAsyncWiFiManager.h>
#include <AceButton.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

using namespace ace_button;

AsyncWebServer server(80);
DNSServer dns;

int state_light_first;
int state_light_second;

AceButton firstLightButton;
AceButton secondLightButton;

void buttonsAction(AceButton*, uint8_t, uint8_t);

void setup() {
  WiFi.disconnect();
  AsyncWiFiManager wifiManager(&server, &dns);

  pinMode(_light_first, OUTPUT);
  pinMode(_light_second, OUTPUT);

  pinMode(_switch_first, INPUT);
  pinMode(_switch_second, INPUT);

  digitalWrite(_light_first, LOW);
  digitalWrite(_light_second, LOW);

  digitalWrite(_switch_first, LOW);
  digitalWrite(_switch_second, LOW);

  wifiManager.autoConnect("Ligth switch");

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "Device will be reseted");
    delay(1000);
    ESP.restart();
  });

  server.on("/state_light_first", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(state_light_first));
  });

  server.on("/state_light_second", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(state_light_second));
  });


  server.on("/on_light_first", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(_light_first, HIGH);
    state_light_first = _on;
    request->send(204);
  });

  server.on("/on_light_second", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(_light_second, HIGH);
    state_light_second = _on;
    request->send(204);
  });

  server.on("/off_light_first", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(_light_first, LOW);
    state_light_first = _off;
    request->send(204);
  });

  server.on("/off_light_second", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(_light_second, LOW);
    state_light_second = _off;
    request->send(204);
  });

  server.begin();

  delay(2000);

  ButtonConfig* buttonsConfig = ButtonConfig::getSystemButtonConfig();
  buttonsConfig->setEventHandler(buttonsAction);
  buttonsConfig->setFeature(ButtonConfig::kFeatureClick);

  firstLightButton.init(_switch_first, LOW, 0 /* id */);
  secondLightButton.init(_switch_second, LOW, 1 /* id */);
}

void loop() {
  // put your main code here, to run repeatedly:
  firstLightButton.check();
  secondLightButton.check();
}


void buttonsAction(AceButton * button, uint8_t eventType,
                   uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventPressed:
      if (button->getId() == 0) {
        if (state_light_first == _on) {
          state_light_first = _off;
          digitalWrite(_light_first, LOW);
        } else if (state_light_first == _off) {
          state_light_first = _on;
          digitalWrite(_light_first, HIGH);
        }
        break;

      } else if (button->getId() == 1) {
        if (state_light_second == _on) {
          state_light_second = _off;
          digitalWrite(_light_second, LOW);
        } else if (state_light_second == _off) {
          state_light_second = _on;
          digitalWrite(_light_second, HIGH);
        }
      }
      break;

    case AceButton::kEventReleased:
      if (button->getId() == 0) {
        if (state_light_first == _on) {
          state_light_first = _off;
          digitalWrite(_light_first, LOW);
        } else if (state_light_first == _off) {
          state_light_first = _on;
          digitalWrite(_light_first, HIGH);
        }
      } else if (button->getId() == 1) {
        if (state_light_second == _on) {
          state_light_second = _off;
          digitalWrite(_light_second, LOW);
        } else if (state_light_second == _off) {
          state_light_second = _on;
          digitalWrite(_light_second, HIGH);
        }
      }
      break;
  }
}
