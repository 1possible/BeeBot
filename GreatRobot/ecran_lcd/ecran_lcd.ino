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

Adafruit_GFX_Button red_btn, blue_btn, back_btn;

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

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


// typedef enum
// {
//   LCD_STATE_INIT = 0,
//   LCD_STATE_RED = 1,
//   LCD_STATE_RED =2,
// }LCD_STATE;

// typedef struct{
//     LCD_STATE state;
//     int ScoreRed;
//     int ScoreBlue; 
//     char Team;   
// }LCD_DATA;

// extern LCD_DATA LCDData;


int Score = 0;
int state = 0;

void setup(void)
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);            //PORTRAIT

}

/* two buttons are quite simple
 */
void loop(void)
{

  switch(state){
    case 0 :
    {
      tft.fillScreen(BLACK);
      //print message
      tft.setTextSize(4);
      tft.setCursor(0, 200);
      tft.setTextColor(WHITE);
      tft.println("Choose a Team");
      //button
      red_btn.initButton(&tft,  100, 260, 100, 40, WHITE, CYAN, BLACK, "RED", 2);
      blue_btn.initButton(&tft, 220, 260, 100, 40, WHITE, CYAN, BLACK, "BLUE", 2);
      red_btn.drawButton(false);
      blue_btn.drawButton(false);
      state = 1;
      break;   
    }
    case 1 :
    {
      bool down = Touch_getXY();
      red_btn.press(down && red_btn.contains(pixel_x, pixel_y));
      blue_btn.press(down && blue_btn.contains(pixel_x, pixel_y));

      if (red_btn.justPressed()) {
        tft.fillScreen(RED);
        //print message
        tft.setTextSize(4);
        tft.setCursor(50, 100);
        tft.setTextColor(WHITE);
        tft.println("Estimated");
        tft.setCursor(90, 130);
        tft.println("score");
        tft.setTextSize(20);
        tft.setCursor(110, 220);
        tft.println(Score);
        digitalWrite(12, HIGH);

        Serial.begin(9600);
        Serial.print("1");
        Serial.end();
        state = 2;
        break;
      }
      if (blue_btn.justPressed()) {
        tft.fillScreen(BLUE);
        //print message
        tft.setTextSize(4);
        tft.setCursor(50, 100);
        tft.setTextColor(WHITE);
        tft.println("Estimated");
        tft.setCursor(90, 130);
        tft.println("score");
        tft.setTextSize(20);
        tft.setCursor(110, 220);
        tft.println(Score);
        digitalWrite(11, HIGH);

        Serial.begin(9600);
        Serial.print("2");
        Serial.end();
        state = 2;
        break;
      }
      break;  
    }
    case 2 :
    {
      back_btn.initButton(&tft,  160, 420, 100, 40, WHITE, CYAN, BLACK, "BACK", 2);
      back_btn.drawButton(false);
      state = 3;
      break;   
    }

    case 3:
    {
      bool down = Touch_getXY();
      back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
      if (back_btn.justPressed()) {
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
        state = 0;
      }
      break;       
    }
  } 
}
