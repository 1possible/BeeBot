#ifndef SCORE_CALCULATOR_H
#define SCORE_CALCULATOR_H

class ScoreCalculator {
  public:
    ScoreCalculator();
    static const int score_default = 20;
    void setup();
    int getScore();
    void addPlantToScore();
    void addOneSonarPanelToScore();
    void addPAMIToScore();
    void addFinalZone();
    void setNbrPlant(int nbrPlant);
  private:
    static const int pts_per_plants = 3; 
    static const int pts_per_SolarPanel = 5; 
    static const int pts_per_PAMI = 5; 
    static const int pts_per_PAMI_PLANTS = 5; //add 5 pts when pami touch plants
    static const int pts_final_zone = 10;
    int score;
    int nbrPlant;
};
#endif