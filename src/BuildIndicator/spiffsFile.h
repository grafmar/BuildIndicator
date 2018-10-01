#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String getContentType(String filename);
bool handleFileRead(String path, ESP8266WebServer &server);

