#define LCD_SCREEN_H
#ifdef LCD_SCREEN_H
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <ArduinoLog.h>
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
class LCDscreen {
  public:
    LCDscreen();
    void setup();
    int chooseTeamScreen();
    int choosePlantsScreen();
    int scoreScreen(int Score);
    void setTeam(int team);
    void showScoreScreen(int Score);
    void setNbrPlant(int newNbrPlant);
  private:
    static const int MINPRESSURE = 200;
    static const int MAXPRESSURE = 1000;
    
    /*static const byte BLACK = 0x0000;
    static const byte BLUE  = 0x001F;
    static const byte RED   = 0xF800;
    static const byte GREEN = 0x07E0;
    static const byte CYAN  = 0x07FF;
    static const byte YELLOW= 0xFFE0;
    static const byte WHITE = 0xFFFF;
    static const byte MAGENTA = 0xF81F;*/
        
    MCUFRIEND_kbv tft;
    bool Touch_getXY(void);
    static const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
    static const int TS_LEFT=915,TS_RT=195,TS_TOP=949,TS_BOT=196;
    TouchScreen ts;
    bool yellowTeam;
    int nbrPlant;
    Adafruit_GFX_Button red_btn, blue_btn, back_btn, pot_0_btn, pot_1_btn, pot_2_btn,pot_3_btn, pot_4_btn;
    int pixel_x, pixel_y;     //Touch_getXY() updates global vars
    enum {OFF, CHOOSE_PLANT_SCREEN, CHOOSE_TEAM_SCREEN , SCORE_SCREEN} stateScreen ;

    void showChooseTeamScreen();
    void showChoosePlantScreen();
    void showScoreYELLOWScreen(int Score);
};
#endif
