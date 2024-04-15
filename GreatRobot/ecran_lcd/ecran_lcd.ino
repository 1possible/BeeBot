#include "lcd_screen.h"
#include "ScoreCalculator.h"
#include "CommunicationArduino.h"

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
        scoreCal.setNbrPlant(btn_val);
        state = CHOOSE_TEAM;
      }
      break;   
    }
    case CHOOSE_TEAM :
    {
      btn_val = screen.chooseTeamScreen();
      if(btn_val != -1){
        screen.setTeam(btn_val);
        commArd.sendTeam(String(btn_val));
        state = SCORE;
      }
      break;   
    }
    case SCORE:
    {
      btn_val = screen.scoreScreen(scoreCal.getScore());
      if(resData){
        screen.showScoreScreen(scoreCal.getScore());
      }
      if(btn_val != -1){
        state = NBR_PLANT;
      } 
    }
  } 
}