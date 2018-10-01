#include <FS.h>

#include "configHandler.h"
#include "BuildIndicator.h"

#define BUILD_KEY_PATH "/buildKeys.cfg"
#define INTERPRETATIONS_PATH "/interpretations.cfg"

ConfigHandler::ConfigHandler() {
}

ConfigHandler::~ConfigHandler() {
}

void ConfigHandler::readBuildKeys(String* buildKeys) {
	if (SPIFFS.exists(BUILD_KEY_PATH)) {
		File file = SPIFFS.open(BUILD_KEY_PATH, "r");
		for (uint8_t i=0; i<NUM_OF_PIXELS; i++) {
			buildKeys[i] = file.readStringUntil('\n');
		}
		file.close();
	}
	else {
		for (uint8_t i=0; i<NUM_OF_PIXELS; i++) {
			buildKeys[i] = "-";
		}
		buildKeys[0] = "SWAP_Main";
		buildKeys[1] = "SWAP_Main";
		buildKeys[2] = "SWAP_DevFT4";
		buildKeys[3] = "SWAP_DevFT3";
		buildKeys[4] = "MFH_Main";
		buildKeys[5] = "Economizer_Main";
		buildKeys[6] = "WC1";
		buildKeys[7] = "WC1";
		buildKeys[8] = "SWAP_Main";
		buildKeys[9] = "SWAP_Main";
		buildKeys[10] = "SWAP_DevFT4";
		buildKeys[11] = "SWAP_DevFT3";
		buildKeys[12] = "MFH_DevFT1";
		buildKeys[13] = "Economizer_DevFT1";
		buildKeys[14] = "WC2";
		buildKeys[15] = "WC2";
	}
}

void ConfigHandler::saveBuildKeys(String* buildKeys) {
	File file = SPIFFS.open(BUILD_KEY_PATH, "w");
	if (file) {
		for (uint8_t i=0; i<NUM_OF_PIXELS; i++) {
			file.print(buildKeys[i].c_str());
			file.print("\n");
		}
		file.close();
	}
}

void ConfigHandler::readInterpretations(std::list<Interpretation>& interpretations) {
  interpretations.clear();
  if (SPIFFS.exists(INTERPRETATIONS_PATH)) {
    File file = SPIFFS.open(INTERPRETATIONS_PATH, "r");
    while (file.available()) {
      String line = file.readStringUntil('\n');
      Interpretation intp;
      uint8_t firstPos = line.indexOf(",");
      uint8_t secondPos = line.indexOf(",", firstPos+1);
      uint8_t thirdPos = line.indexOf(",", secondPos+1);
      intp.interpretName = line.substring(0, firstPos);
      intp.color.r = constrain((line.substring(firstPos+1, secondPos)).toInt(), 0, 255);
      intp.color.g = constrain((line.substring(secondPos+1, thirdPos)).toInt(), 0, 255);
      intp.color.b = constrain((line.substring(thirdPos+1)).toInt(), 0, 255);
      interpretations.push_back(intp);
    }
    file.close();
  }
  else {
    //interpretations.clear();
    // create default interpretations
    Interpretation intp;
    intp.interpretName = "passed";
    intp.color = { 0,255,0 };
    interpretations.push_back(intp);
    intp.interpretName = "failed";
    intp.color = { 255,0,0 };
    interpretations.push_back(intp);
    saveInterpretations(interpretations);
  }
}

void ConfigHandler::saveInterpretations(std::list<Interpretation>& interpretations) {
  File file = SPIFFS.open(INTERPRETATIONS_PATH, "w");
  if (file) {
    for (Interpretation interpretation : interpretations) {
      file.printf("%s,%d,%d,%d\n",interpretation.interpretName.c_str(), interpretation.color.r, interpretation.color.g, interpretation.color.b);
    }
    file.close();
  }
}

