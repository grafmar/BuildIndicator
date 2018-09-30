#ifndef HTMLHANDLER_H
#define HTMLHANDLER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "firebaseHandler.h"

void htmlPrintSetting(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler);
void htmlEditKeys(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler);
void htmlEditInterpret(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler);
void htmlSaveSetting(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler);
void htmlSaveInterpret(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler);
void htmlPrintDirectory(ESP8266WebServer &server);

#endif // HTMLHANDLER_H


