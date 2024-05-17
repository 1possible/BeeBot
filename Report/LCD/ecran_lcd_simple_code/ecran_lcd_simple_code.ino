#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino

const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
const int TS_LEFT=915,TS_RT=195,TS_TOP=949,TS_BOT=196;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//define button
Adafruit_GFX_Button red_btn, blue_btn, back_btn;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//state
enum{INIT, CHOOSE_TEAM , WAIT_BACK} state;

//variable for score
int Score = 0;


int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
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
void drawn_team_score(team_color)
{
	tft.fillScreen(team_color);//fill the screen with the team_color
	//print message
	tft.setTextSize(4); //set the text size
	tft.setCursor(50, 100); //set the position of the cursor (x,y)
	tft.setTextColor(WHITE); //set the color of the text
	tft.println("Estimated"); //print the text
	tft.setCursor(90, 130);
	tft.println("score");
	tft.setTextSize(20);
	tft.setCursor(110, 220);
	tft.println(Score); //print the score
	//init and draw "BACK" button
	back_btn.initButton(&tft,  160, 420, 100, 40, WHITE, CYAN, BLACK, "BACK", 2);
	back_btn.drawButton(false);
}

void setup(void)
{
    
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT

}


void loop(void)
{
  switch(state){
		  case INIT:
      {
        tft.fillScreen(BLACK); //fill the screen with the black color
        //print message "Choose a Team" in white
        tft.setTextSize(4);
        tft.setCursor(0, 200);
        tft.setTextColor(WHITE);
        tft.println("Choose a Team");
        //button
        red_btn.initButton(&tft,  100, 260, 100, 40, WHITE, CYAN, BLACK, "RED", 2);
        blue_btn.initButton(&tft, 220, 260, 100, 40, WHITE, CYAN, BLACK, "BLUE", 2);
        red_btn.drawButton(false);
        blue_btn.drawButton(false);
        state = CHOOSE_TEAM ;
        break;
      }
      case CHOOSE_TEAM :
      {
	      //initialise the touch zone for red and blue button
        bool down = Touch_getXY();
        red_btn.press(down && red_btn.contains(pixel_x, pixel_y));
        blue_btn.press(down && blue_btn.contains(pixel_x, pixel_y));
				//if press on red button
        if (red_btn.justPressed()) {
          drawn_team_score(RED);
          state = WAIT_BACK;
        }
        //if press on blue button
        if (blue_btn.justPressed()) {
          drawn_team_score(BLUE);
          state = WAIT_BACK;
        }
      break;    
      }
      case WAIT_BACK :
      {
	      //check if the back button was pressed return to INIT (team selection render screen)
        bool down = Touch_getXY();
        back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
        if (back_btn.justPressed()) {
          state = INIT;
        }
      break;    
      }    
  } 
}
