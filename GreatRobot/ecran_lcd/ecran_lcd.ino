#include "lcd_screen.h"
#include "ScoreCalculator.h"

int Score = 0;
enum{NBR_PLANT, CHOOSE_TEAM,SCORE} state;
LCDscreen screen = LCDscreen();
ScoreCalculator scoreCal = ScoreCalculator();
void setup(void)
{
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  screen.setup();
  state =NBR_PLANT;
}

void loop(void)
{
  int btn_val = -1;
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
        state = SCORE;
      }
      break;   
    }
    case SCORE:
    {
      btn_val = screen.scoreScreen(scoreCal.getScore());
      if(btn_val != -1){
        scoreCal.addOneSonarPanelToScore();
        screen.showScoreScreen(scoreCal.getScore());
        //state = NBR_PLANT;
      }    
    }
  } 
}