#ifndef FIREBASEHANDLER_H
#define FIREBASEHANDLER_H

// Checks and conntects to Firebase database

#include <Arduino.h>
#include <list>

#include "BuildIndicator.h"
#include "configHandler.h"
#include "interpretation.h"
#include "ledHandler.h"
#include "displayHandler.h"

class FirebaseHandler {
public:
  
  explicit FirebaseHandler(ConfigHandler& configHandler, LedHandler& ledHandler, DisplayHandler& displayHandler);
  ~FirebaseHandler();

  void begin(String host, String auth, String path);
  
  void updateListOfBuilds();
  std::list<String> listBuilds();
  void saveBuildKeys();
  void saveInterpretations();
  String* getBuildKeys();
  Color* getColors();
  std::list<Interpretation>& getInterpretations();
  void handleEvents();

private:

  void refreshAllBuildKeys();
  void refreshBuildKey(String key, String result);
  void startStreaming();
  Color getColorOfResult(String result);

  String m_firebaseHost;
  String m_firebaseAuth;
  String m_firebasePath;
  String m_buildKeys[NUM_OF_PIXELS];
  Color m_colors[NUM_OF_PIXELS];
  ConfigHandler& m_configHandler;
  LedHandler& m_ledHandler;
  DisplayHandler& m_displayHandler;
  std::list<String> m_listOfBuilds;
  std::list<Interpretation> m_interpretations;
};

#endif // FIREBASEHANDLER_H


