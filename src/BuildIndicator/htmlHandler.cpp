#include <FS.h>

#include "htmlHandler.h"

uint16_t handleEditKeys(bool doSend, ESP8266WebServer &server, uint8_t rows, uint8_t cols, std::list<String> &buildsAvailable, String* buildKeys);
uint16_t handleEditInterpret(bool doSend, ESP8266WebServer &server, uint8_t rows, uint8_t cols, std::list<Interpretation>& interpretations);

uint8_t hexChar2byte(char input) {
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}

Color htmlColorToColor(const char* htmlColor) {
  uint8_t red   = hexChar2byte(htmlColor[1]) * 16 + hexChar2byte(htmlColor[2]);
  uint8_t green = hexChar2byte(htmlColor[3]) * 16 + hexChar2byte(htmlColor[4]);
  uint8_t blue  = hexChar2byte(htmlColor[5]) * 16 + hexChar2byte(htmlColor[6]);
  return Color(red, green, blue);
}

void htmlPrintSetting(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler) {
  String* buildKeys = fbHandler.getBuildKeys();
  Color* colors = fbHandler.getColors();
  String output = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n";
  output += "<title>IndicationLamps</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  output += "<link rel=\"stylesheet\" type=\"text/css\" href=\"indicationLamps.css\">\n";
  output += "<style>\n";
  output += "td.shadow{ color: #EEEEEE; text-shadow: 0px 0px 2px #000000; text-shadow: 0px 0px 5px #000000; padding:  0.7em 1em 0.7em 1em; }\n";
  output += "</style>\n";
  output += "</head>\n";
  output += "<body class=\"body\">\n";

  output += "<div class='table'>\n";
  output += "<table width='100%' border='1'>\n";
  for (uint8_t col=0; col<cols; col++) {
    output += "<col width='" + String(100/cols, DEC) + "%'>\n";
  }
  for (uint8_t row=0; row<rows; row++) {
    output += "<tr>\n";
    for (uint8_t col=0; col<cols; col++) {
      char colorString[10];
      sprintf(colorString, "%02x%02x%02x", colors[row + col*rows].r, colors[row + col*rows].g, colors[row + col*rows].b);
      output += "<td class='shadow' style='background: #"+String(colorString)+";'><b>";
      output += buildKeys[row + col*rows];
      output += "</b></td>\n";
    }
    output += "</tr>\n";
  }
  output += "</table><br>\n";
  output += "<a href='/editKeys'>Edit the shown builds/keys</a><br><br>\n";
  output += "<a href='/editInterpret'>Edit the interpreted values</a><br>\n";
  output += "</div>\n";
  
  output += "\n</body>\n</html>\n";
  server.send(200, "text/html", output);
  fbHandler.updateListOfBuilds();
}


void htmlEditKeys(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler) {
	String* buildKeys = fbHandler.getBuildKeys();
	std::list<String> buildsAvailable = fbHandler.listBuilds();
  
  uint16_t contentLength = handleEditKeys(false, server, rows, cols, buildsAvailable, buildKeys);
	//now send an empty string but with the header field Content-Length to let the browser know how much data it should expect
  server.setContentLength(contentLength);
  server.send(200, "text/html", "");
  static_cast<void>(handleEditKeys(true, server, rows, cols, buildsAvailable, buildKeys));
}

uint16_t handleEditKeys(bool doSend, ESP8266WebServer &server, uint8_t rows, uint8_t cols, std::list<String> &buildsAvailable, String* buildKeys) {
  uint16_t totalLength = 0;
	String output = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>IndicationLamps Configuration</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" type=\"text/css\" href=\"indicationLamps.css\">\n</head>\n<body class=\"body\">\n";
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }

  output = "<div class='table'>\n";
  output += "<form action='/save' method='post'>\n";
  output += "<table width='100%' border='1'>\n";
  for (uint8_t col=0; col<cols; col++) {
    output += "<col width='" + String(100/cols, DEC) + "%'>\n";
  }
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }
  for (uint8_t row=0; row<rows; row++) {
    output = "<tr>\n";
    for (uint8_t col=0; col<cols; col++) {
      output += "<td>";
      output += "<select id='" + String(row + col*rows, DEC) + "' name='" + String(row + col*rows, DEC) + "'>\n";
      for (String build : buildsAvailable) {
        output += "<option value='" + build + "'";
        if (build == buildKeys[row + col*rows]) {
          output += " selected";
        }
        output += ">" + build + "</option>\n";
      }
      output += "</select>";
      if (col < cols-1) {
        output += "<button type='button' onclick='document.getElementById(\""
                  + String(row + (col+1)*rows, DEC)
                  + "\").selectedIndex=document.getElementById(\""
                  + String(row + col*rows, DEC)
                  + "\").selectedIndex'>-&gt;</button>";
      }
      output += "</td>\n";
    }
    output += "</tr>\n";
    totalLength += output.length();
    if (doSend) { server.sendContent_P(output.c_str(), output.length()); }
  }
  output = "</table>\n";
  output += "<input type='submit' value='Save configuration'><br>";
  output += "</form>\n";
  output += "</div>\n";
  
  output += "\n</body>\n</html>\n";
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }

  return totalLength;
}

void htmlEditInterpret(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler) {
  std::list<Interpretation>& interpretations = fbHandler.getInterpretations();

  uint16_t contentLength = handleEditInterpret(false, server, rows, cols, interpretations);
  // now send an empty string but with the header field Content-Length to let the browser know how much data it should expect
  server.setContentLength(contentLength);
  server.send(200, "text/html", "");
  static_cast<void>(handleEditInterpret(true, server, rows, cols, interpretations));
}

uint16_t handleEditInterpret(bool doSend, ESP8266WebServer &server, uint8_t rows, uint8_t cols, std::list<Interpretation>& interpretations) {
  uint16_t totalLength = 0;
  String output = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n";
  output += "<title>IndicationLamps Interpretation Configuration</title>\n";
  output += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" type=\"text/css\" href=\"indicationLamps.css\">\n";
  output += "<script>\n";
  output += "  var addNum = 1;\n\n";
  output += "  function remove(id) {\n";
  output += "    var elem = document.getElementById(id);\n";
  output += "    return elem.parentNode.removeChild(elem);\n";
  output += "  }\n\n";
  output += "  function addLine() {\n";
  output += "    var addHtml = \"<div id ='divAdd_\"+addNum+\"'>\\n\";\n";
  output += "    addHtml = addHtml + \"<input type='text' id='interpretAdd_\" + addNum + \"' name='interpretAdd_\" + addNum + \"' pattern='[A-Za-z0-9._%+-]+' value='' style='width:30%'>\\n\";\n";
  output += "    addHtml = addHtml + \"<input type='color' id='colorAdd_\"+addNum+\"' name='colorAdd_\"+addNum+\"' value='#00ff00' style='width:30%;'>\\n\";\n";
  output += "    addHtml = addHtml + \"<input type='button' value='Del' onclick='remove(\\\"divAdd_\" + addNum + \"\\\")' style='width:20%;'><br>\\n\";\n";
  output += "    addHtml = addHtml + \"</div>\\n\";\n";
  output += "    document.getElementById('add').insertAdjacentHTML('beforebegin',addHtml);\n";
  output += "    addNum = addNum + 1;\n";
  output += "  }\n\n";
  output += "</script>";
  output += "</head>\n<body class=\"body\">\n";
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }

  output = "<div class='table'>\n";
  output += "<form action='/saveInterpret' method='post'>\n";
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }

  uint16_t inputFieldNumber = 1;
  for (Interpretation interpretation : interpretations) {
    char colorString[10];
    sprintf(colorString, "%02x%02x%02x", interpretation.color.r, interpretation.color.g, interpretation.color.b);
    output  = "<div id ='div_" + String(inputFieldNumber) + "'>\n";
    output += "  <input type='text' id='interpret_" + String(inputFieldNumber) + "' name='interpret_" + String(inputFieldNumber) + "' pattern='[A-Za-z0-9._%+-]+' value='"+interpretation.interpretName+"' style='width:30%'>\n";
    output += "  <input type='color' id='color_" + String(inputFieldNumber) + "' name='color_" + String(inputFieldNumber) + "' value='#"+String(colorString)+"' style='width:30%;'>\n";
    output += "  <input type='button' value='Del' onclick='remove(\"div_"+String(inputFieldNumber)+"\")' style='width:20%;'><br>\n";
    output += "</div>\n";
    totalLength += output.length();
    if (doSend) { server.sendContent_P(output.c_str(), output.length()); }
    inputFieldNumber++;
  }

  output = "<input type='button' id='add' value='Add' onclick='addLine()' style='width:20%;'><br><br>\n";
  output += "<input type='submit' value='Save configuration'><br>";
  output += "</form>\n";
  output += "</div>\n";
  output += "\n</body>\n</html>\n";
  totalLength += output.length();
  if (doSend) { server.sendContent_P(output.c_str(), output.length()); }

  return totalLength;
}


void htmlSaveSetting(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler) {
  String newProgramName;
  uint8_t numOfKeys = rows*cols;
  String* buildKeys = fbHandler.getBuildKeys();

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    Serial.printf("argument [%s] has data [%s]\n", server.argName( i ).c_str(), server.arg( i ).c_str());
  }
  for ( uint8_t i = 0; i < numOfKeys; i++ ) {
    String iString = String(i);
    if (server.arg(iString)){
      buildKeys[i] = server.arg(iString);
    }
  }
  
  server.sendHeader("Location","/");
  server.send(301);
  fbHandler.saveBuildKeys();
}

void htmlSaveInterpret(ESP8266WebServer &server, uint8_t rows, uint8_t cols, FirebaseHandler& fbHandler) {
  std::list<Interpretation>& interpretations = fbHandler.getInterpretations();
  interpretations.clear();

  Serial.println("htmlSaveInterpret...");
  for (uint8_t i = 0; i < server.args(); i++) {
    Serial.printf("argument [%s] has data [%s]\n", server.argName(i).c_str(), server.arg(i).c_str());
    if ((i + 1 < server.args()) && (server.argName(i).startsWith("interpret")) && (server.argName(i+1).startsWith("color"))) {
      Interpretation intp;
      intp.interpretName = server.arg(i);
      intp.color = htmlColorToColor(server.arg(i+1).c_str());
      interpretations.push_back(intp);
    }
  }

  server.sendHeader("Location", "/");
  server.send(301);
  fbHandler.saveInterpretations();
}

void htmlPrintDirectory(ESP8266WebServer &server) {
  String indent = "";
  String path = "/";
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>IndicationLamps filesystem content</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" type=\"text/css\" href=\"indicationLamps.css\">\n</head>\n<body class=\"body\">\n";
  output += "<div class='table'>\n";
  while(dir.next()){
    output += indent;
    output += "<a href=\"" + dir.fileName() + "\">";
    output += dir.fileName();
    output += "</a>";
    output += "<br>\n";
  }
  
  output += "</div>\n";
  output += "\n</body>\n</html>\n";
  server.send(200, "text/html", output);
}

