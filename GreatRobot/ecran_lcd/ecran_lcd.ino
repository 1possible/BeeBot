#include "lcd_screen.h"
#include "ScoreCalculator.h"
#include "CommunicationArduino.h"
#include <ArduinoLog.h>

int Score = 0;
enum{NBR_PLANT, CHOOSE_TEAM,SCORE} state;
LCDscreen screen = LCDscreen();
ScoreCalculator scoreCal = ScoreCalculator();
CommunicationArduino commArd = CommunicationArduino();
void setup(void)
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  screen.setup();
  state =NBR_PLANT;
  commArd.setup();

  Log.begin(LOG_LEVEL_SILENT, &Serial,true);
  /*
  *** LOG level ***
  * 0 - LOG_LEVEL_SILENT     no output 
  * 1 - LOG_LEVEL_FATAL      fatal errors 
  * 2 - LOG_LEVEL_ERROR      all errors  
  * 3 - LOG_LEVEL_WARNING    errors, and warnings 
  * 4 - LOG_LEVEL_NOTICE     errors, warnings and notices 
  * 5 - LOG_LEVEL_TRACE      errors, warnings, notices & traces 
  * 6 - LOG_LEVEL_VERBOSE    all 
  */

  //Start logging

  Log.notice("******************************************" CR);                     
  Log.notice("***        Logging: LCD arduino        ***" CR);                      
  Log.notice("******************************************" CR);
}

void loop(void)
{
  int btn_val = -1;
  bool resData = commArd.receive(&scoreCal);
  switch(state){
    case NBR_PLANT :
    {
      btn_val = screen.choosePlantsScreen();
      if(btn_val != -1){
        Log.notice("MAIN : new state : CHOOSE_TEAM" CR);
        scoreCal.setNbrPlant(btn_val);
        screen.setNbrPlant(btn_val);
        state = CHOOSE_TEAM;
      }
      break;   
    }
    case CHOOSE_TEAM :
    {
      btn_val = screen.chooseTeamScreen();
      if(btn_val != -1){
        Log.notice("MAIN : new state : SCORE" CR);
        screen.setTeam(btn_val);
        scoreCal.addPAMIToScore();
        commArd.sendTeam(String(btn_val));
        state = SCORE;
      }
      break;   
    }
    case SCORE:
    {
      btn_val = screen.scoreScreen(scoreCal.getScore());
      if(resData){
        Log.notice("MAIN : refresh score screen" CR);
        screen.showScoreScreen(scoreCal.getScore());
      }
      if(btn_val != -1){
        Log.notice("MAIN : new state : NBR_PLANT" CR);
        state = NBR_PLANT;
      } 
    }
  } 
}