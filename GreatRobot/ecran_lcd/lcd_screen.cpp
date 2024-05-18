#include "lcd_screen.h"

LCDscreen::LCDscreen(): ts(XP, YP, XM, YM, 300){}

void LCDscreen::setup(){
  //ts = TouchScreen(XP, YP, XM, YM, 300);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);            //PORTRAIT
  yellowTeam = true;
  stateScreen = OFF;
  nbrPlant = 0;
}

int LCDscreen::chooseTeamScreen(){
  int val = -1;
  if( stateScreen!= CHOOSE_TEAM_SCREEN){
    showChooseTeamScreen();
  }
  bool down = Touch_getXY();
  red_btn.press(down && red_btn.contains(pixel_x, pixel_y));
  blue_btn.press(down && blue_btn.contains(pixel_x, pixel_y));
  if (red_btn.justPressed()) {
    val = 1;
  }
  else if (blue_btn.justPressed()) {
    val = 2;
  }
  return val;
}

int LCDscreen::choosePlantsScreen(){
  int val = -1;
  if(stateScreen != CHOOSE_PLANT_SCREEN){
    showChoosePlantScreen();
  }
  bool down = Touch_getXY();
  pot_0_btn.press(down && pot_0_btn.contains(pixel_x, pixel_y));
  pot_1_btn.press(down && pot_1_btn.contains(pixel_x, pixel_y));
  pot_2_btn.press(down && pot_2_btn.contains(pixel_x, pixel_y));
  pot_3_btn.press(down && pot_3_btn.contains(pixel_x, pixel_y));
  pot_4_btn.press(down && pot_4_btn.contains(pixel_x, pixel_y));
  if (pot_0_btn.justPressed()) {
    val = 0;
  }
  else if (pot_1_btn.justPressed()) {
    val = 1;
  }
  else if (pot_2_btn.justPressed()) {
    val = 2;
  }
  else if (pot_3_btn.justPressed()) {
    val = 3;
  }
  else if (pot_4_btn.justPressed()) {
    val = 4;
  }
  return val;
}

int LCDscreen::scoreScreen(int Score){
  int val = -1;
  if(stateScreen != SCORE_SCREEN){
    showScoreScreen(Score);
  }
  bool down = Touch_getXY();
  back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
  if (back_btn.justPressed()) {
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
        val = 0;
      }
  return val;
}

void LCDscreen::setTeam(int team){
  if(team == 1){
    Log.trace("LCD : set team on YELLOW" CR);
    yellowTeam = true;
  }else{
    Log.trace("LCD : set team on BLUE" CR);
    yellowTeam = false;
  }
}
void LCDscreen::setNbrPlant(int newNbrPlant){
  nbrPlant = newNbrPlant;
  Log.trace("LCD : set plant number to %d" CR, nbrPlant);
}

bool LCDscreen::Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

void LCDscreen::showChoosePlantScreen(){
  Log.trace("LCD : show the plant number selection screen on LCD" CR);
  tft.fillScreen(BLACK);
  //print message
  tft.setTextSize(4);
  tft.setCursor(50, 100);
  tft.setTextColor(WHITE);
  tft.println("number of");
  tft.setCursor(100, 130);
  tft.println("plant");
  //button
  pot_1_btn.initButton(&tft,  100, 220, 100, 40, WHITE, CYAN, BLACK, "1", 2);
  pot_2_btn.initButton(&tft,  220, 220, 100, 40, WHITE, CYAN, BLACK, "2", 2);
  pot_3_btn.initButton(&tft,  100, 270, 100, 40, WHITE, CYAN, BLACK, "3", 2);
  pot_4_btn.initButton(&tft,  220, 270, 100, 40, WHITE, CYAN, BLACK, "4", 2);
  pot_0_btn.initButton(&tft,  160, 320, 100, 40, WHITE, CYAN, BLACK, "0", 2);
  pot_0_btn.drawButton(false);
  pot_1_btn.drawButton(false);
  pot_2_btn.drawButton(false);
  pot_3_btn.drawButton(false);
  pot_4_btn.drawButton(false);
  stateScreen = CHOOSE_PLANT_SCREEN;
}

void LCDscreen::showChooseTeamScreen(){
  Log.trace("LCD : show the team selection screen on LCD" CR);
  tft.fillScreen(BLACK);
  //print message
  tft.setTextSize(4);
  tft.setCursor(0, 200);
  tft.setTextColor(WHITE);
  tft.println("Choose a Team");
  tft.setTextSize(3);
  tft.setCursor(0, 400);
  tft.setTextColor(WHITE);
  tft.print("plant :");
  tft.println(nbrPlant);
   //button
  red_btn.initButton(&tft,  100, 260, 100, 40, WHITE, CYAN, BLACK, "yellow", 2);
  blue_btn.initButton(&tft, 220, 260, 100, 40, WHITE, CYAN, BLACK, "BLUE", 2);
  red_btn.drawButton(false);
  blue_btn.drawButton(false);
  stateScreen = CHOOSE_TEAM_SCREEN;
}

void LCDscreen::showScoreScreen(int Score){
  Log.trace("LCD : show the score screen on LCD" CR);
  if(yellowTeam){
    tft.fillScreen(YELLOW);
    tft.setTextColor(BLACK);
  }else{
    tft.fillScreen(BLUE);
    tft.setTextColor(WHITE);
  }
  //print message
  tft.setTextSize(4);
  tft.setCursor(50, 100);
  tft.println("Estimated");
  tft.setCursor(90, 130);
  tft.println("score");
  tft.setTextSize(20);
  if(Score <10){
    tft.setCursor(110, 220);
  }else{
    tft.setCursor(50, 220);
  }
  tft.println(Score);
  digitalWrite(11, HIGH);
  back_btn.initButton(&tft,  160, 420, 100, 40, WHITE, CYAN, BLACK, "BACK", 2);
  back_btn.drawButton(false);
  stateScreen = SCORE_SCREEN;
}