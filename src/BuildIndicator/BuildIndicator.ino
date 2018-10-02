#include "color.h"
#include "color.h"
#include "interpretation.h"
#include "BuildIndicator.h"
#include "configHandler.h"
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Adafruit_GFX.h>
#include <FS.h>


#include "displayHandler.h"
#include "htmlHandler.h"
#include "spiffsFile.h"
#include "firebaseHandler.h"
#include "configHandler.h"
#include "ledHandler.h"


// Set these to run example.
#define FIREBASE_HOST "yourHost.firebaseio.com"
#define FIREBASE_AUTH "TheVeryLongAuthenticationKeyForFirebase1"
#define FIREBASE_PATH "/yourPathToBuilds/"


ESP8266WebServer server(80);
ConfigHandler configHandler;
LedHandler ledHandler;
DisplayHandler displayHandler;
FirebaseHandler firebaseHandler(configHandler, ledHandler, displayHandler);


void setup() {
  Serial.begin(115200);
  
  // delay to sattle all voltages (for display)
  delay(2000);

  // Display Handler
  displayHandler.begin();

  // LED-Handler
  ledHandler.begin();

  // doit
  displayHandler.getDisplay().clearDisplay();
  displayHandler.getDisplay().setCursor(0,0);
  displayHandler.getDisplay().setTextSize(1);
  displayHandler.getDisplay().setTextColor(WHITE);
  
  setupSPIFFS();
  setupWifi();
  firebaseHandler.begin(FIREBASE_HOST, FIREBASE_AUTH, FIREBASE_PATH);
  //setupWebserver();
}


void loop() {
  //server.handleClient();
  firebaseHandler.handleEvents();
  displayHandler.handle();
}

void setupSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }
}

void setupWifi() {
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  displayHandler.getDisplay().clearDisplay();
  displayHandler.getDisplay().setCursor(0,0);
  displayHandler.getDisplay().println("Connect to");
  displayHandler.getDisplay().println("\"BuildIndicatorAP\" to config WiFi");
  displayHandler.getDisplay().display();

  if (!wifiManager.autoConnect("BuildIndicatorAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  displayHandler.getDisplay().clearDisplay();
  displayHandler.getDisplay().setCursor(0,0);
  displayHandler.getDisplay().println("my IP: ");
  displayHandler.getDisplay().println();
  displayHandler.getDisplay().setTextSize(2);
  displayHandler.getDisplay().println(WiFi.localIP());
  displayHandler.getDisplay().display();
}

void setupWebserver() {
  // handle requests
  server.on("/", HTTP_GET, []() {
    htmlPrintSetting(server, NUM_OF_PIXEL_ROWS, NUM_OF_PIXEL_COLS, firebaseHandler);
  });
  server.on("/editKeys", HTTP_GET, []() {
    htmlEditKeys(server, NUM_OF_PIXEL_ROWS, NUM_OF_PIXEL_COLS, firebaseHandler);
  });
  server.on("/editInterpret", HTTP_GET, []() {
    htmlEditInterpret(server, NUM_OF_PIXEL_ROWS, NUM_OF_PIXEL_COLS, firebaseHandler);
  });
  server.on("/save", HTTP_POST, []() {
    htmlSaveSetting(server, NUM_OF_PIXEL_ROWS, NUM_OF_PIXEL_COLS, firebaseHandler);
  });
  server.on("/saveInterpret", HTTP_POST, []() {
    htmlSaveInterpret(server, NUM_OF_PIXEL_ROWS, NUM_OF_PIXEL_COLS, firebaseHandler);
  });
  server.on("/list", HTTP_GET, []() {
    htmlPrintDirectory(server);
  });

  server.onNotFound([]() {
    Serial.printf("request: %s\n", server.uri().c_str());
    if(!handleFileRead(server.uri(), server))
      server.send(404, "text/plain", "FileNotFound");
  });
  
  server.begin();
}


