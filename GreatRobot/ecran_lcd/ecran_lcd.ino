#include "lcd_screen.h"

int Score = 0;
enum{NBR_PLANT, CHOOSE_TEAM,SCORE} state;
LCDscreen screen = LCDscreen();
void setup(void)
{
  Serial.begin(9600);
  Serial.println("setup in ...");
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  screen.setup();
  state =NBR_PLANT;
  Serial.println("setup out..");
}

void loop(void)
{
  int btn_val = -1;
  switch(state){
    case NBR_PLANT :
    {
      btn_val = screen.choosePlantsScreen();
      if(btn_val != -1){
        Serial.print("btn");
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
      btn_val = screen.scoreScreen(0);
      if(btn_val != -1){
        state = NBR_PLANT;
      }    
    }
  } 
}
