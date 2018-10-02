#include <FirebaseArduino.h>
#include "firebaseHandler.h"


FirebaseHandler::FirebaseHandler(ConfigHandler& configHandler, LedHandler& ledHandler, DisplayHandler& displayHandler) :
  m_firebaseHost(""),
  m_firebaseAuth(""),
  m_firebasePath(""),
  m_configHandler(configHandler),
  m_ledHandler(ledHandler),
  m_displayHandler(displayHandler),
  m_listOfBuilds(),
  m_interpretations()
{
}

FirebaseHandler::~FirebaseHandler() {
}

void FirebaseHandler::begin(String host, String auth, String path) {
  m_firebaseHost = host;
  m_firebaseAuth = auth;
  m_firebasePath = path;

  m_configHandler.readBuildKeys(m_buildKeys);
  m_configHandler.readInterpretations(m_interpretations);
  
  Firebase.begin(m_firebaseHost, m_firebaseAuth);

  FirebaseObject allBuilds = Firebase.get(m_firebasePath);
  if (allBuilds.success()) {
    JsonVariant allBuildsJson = allBuilds.getJsonVariant();
    allBuildsJson.prettyPrintTo(Serial);
    Serial.println("");
    Serial.println("");
    Serial.println("Keys:");
    for( const auto& kv : allBuildsJson.as<JsonObject>() ) {
      if (kv.value.is<char*>()) {
        Serial.println(kv.key);
      }
    }
  } else {
    Serial.println("couldn't parse!!!");
    Serial.println("");
  }

  refreshAllBuildKeys();
}

void FirebaseHandler::updateListOfBuilds() {
	Serial.println("updateListOfBuilds()");
	Serial.println("clear list");
	m_listOfBuilds.clear();

	Serial.println("request firebase");
	FirebaseObject allBuilds = Firebase.get(m_firebasePath);
	if (allBuilds.success()) {
		JsonVariant allBuildsJson = allBuilds.getJsonVariant();
		Serial.println("fill in list from json");
		for (const auto& kv : allBuildsJson.as<JsonObject>()) {
			if (kv.value.is<char*>()) {
				m_listOfBuilds.push_back(kv.key);
			}
		}
	}
	Serial.println("End of updateListOfBuilds()");
  // restart streaming
  startStreaming();
}

std::list<String> FirebaseHandler::listBuilds() {
  return m_listOfBuilds;
}

void FirebaseHandler::saveBuildKeys() {
	m_configHandler.saveBuildKeys(m_buildKeys);
  refreshAllBuildKeys();
}

void FirebaseHandler::saveInterpretations() {
  m_configHandler.saveInterpretations(m_interpretations);
  refreshAllBuildKeys();
}

String* FirebaseHandler::getBuildKeys() {
	return m_buildKeys;
}

Color * FirebaseHandler::getColors() {
  return m_colors;
}

std::list<Interpretation>& FirebaseHandler::getInterpretations() {
  return m_interpretations;
}

void FirebaseHandler::handleEvents() {
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }
  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    
    if (event.getString("path") != "") {
      String key = event.getString("path").substring(1);  // remove leading "/"
      String result = event.getString("data");
      if (event.getString("path") == "/") {
        JsonVariant eventJson = event.getJsonVariant("data");
        for (const auto& kv : eventJson.as<JsonObject>()) {
          key = kv.key;
          result = kv.value.as<char*>();
        }
      }
      else {
        //Serial.print("got event (");
        //Serial.print(event.getString("type"));
        //Serial.print("): ");
        //Serial.print(event.getString("path"));
        //Serial.print(" = ");
        //Serial.println(event.getString("data"));
      }
      Serial.print("-> key: ");
      Serial.print(key);
      Serial.print(", result: ");
      Serial.println(result);
      refreshBuildKey(key, result);
      m_ledHandler.show();
    }
  }
//  if (millis() - lastMillis > 1000) {
//    lastMillis = millis();
//    // Get all entries.
//    // TODO: Replace with streaming
//    FirebaseObject builds = Firebase.get(FIREBASE_PATH);
//    if (Firebase.failed()) {
//        Serial.println("Firebase get failed");
//        Serial.println(Firebase.error());
//        return;
//    }
//    else {
//      for (uint8_t i=0; i<NUM_OF_PIXELS ; i++) {
//        if (m_buildKeys[i] != "-") {
//          String buildResult = builds.getString(m_buildKeys[i]);
//          // Serial.println(m_buildKeys[i] + ": " + buildResult);
//          setBuildToPixel(buildResult, i);
//        }
//      }
//      strip.show();
//    }
//  }
}

void FirebaseHandler::refreshAllBuildKeys() {
  for (uint8_t i = 0; i < NUM_OF_PIXELS; i++) {
    bool alreadyUpdated = false;
    for (uint8_t j = 0; j < i; j++) {
      if (m_buildKeys[i] == m_buildKeys[j]) {
        alreadyUpdated = true;
      }
    }
    if (!alreadyUpdated) {
      if (m_buildKeys[i] != "-") {
        String buildResult = Firebase.getString(m_firebasePath + m_buildKeys[i]);
        refreshBuildKey(m_buildKeys[i], buildResult);
      }
      else {
        // set empty fields dark
        refreshBuildKey(m_buildKeys[i], "RGB:0,0,0");
      }
      String buildResult;
    }
  }
  m_ledHandler.show();

  // restart streaming
  startStreaming();
}

void FirebaseHandler::refreshBuildKey(String key, String result) {
  Color color = getColorOfResult(result);
  Serial.printf("refreshBuildKey(): %s, %s (%d,%d,%d)", key.c_str(), result.c_str(), color.r, color.g, color.b);
  Serial.println();
  for (uint8_t i = 0; i < NUM_OF_PIXELS; i++) {
    if (m_buildKeys[i] == key) {
      m_ledHandler.setLedColor(i, color);
      m_colors[i] = color;
      String message = key + "\n" + result;
      m_displayHandler.setMessage(message);
    }
  }
}

void FirebaseHandler::startStreaming() {
  uint8_t tries=0;
  do {
    Serial.println("Start Firebase stream");
    Firebase.stream(m_firebasePath);
    tries++;
    Serial.println("check on success");
  } while (!Firebase.success() && (tries < 10));
  Serial.println("done");
}

Color FirebaseHandler::getColorOfResult(String result) {
  // result is encoded color
  if ((result.startsWith("rgb:")) || (result.startsWith("RGB:"))) {
    uint8_t firstPos = result.indexOf(",");
    uint8_t secondPos = result.indexOf(",", firstPos+1);
    uint8_t red   = constrain((result.substring(4, firstPos)).toInt(), 0, 255);
    uint8_t green = constrain((result.substring(firstPos+1, secondPos)).toInt(), 0, 255);
    uint8_t blue  = constrain((result.substring(secondPos+1)).toInt(), 0, 255);
    //Serial.printf("getColorOfResult(): direct (%i,%i,%i)\n", red, green, blue);
    return Color(red, green, blue);
  }

  // search result in interpretations
  for (Interpretation interpretation : m_interpretations) {
    //Serial.printf("getColorOfResult(): check %s == %s?\n", interpretation.interpretName.c_str(), result.c_str());
    if (interpretation.interpretName == result) {
      //Serial.printf("getColorOfResult(): from interpretation (%d,%d,%d)\n", interpretation.color.r, interpretation.color.g, interpretation.color.b);
      return interpretation.color;
    }
  }

  // return default color (dark)
  //Serial.println("getColorOfResult(): nothing found (0,0,0)");
  return Color(0,0,0);
}

