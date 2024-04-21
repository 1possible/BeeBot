#include "ScoreCalculator.h"

ScoreCalculator::ScoreCalculator(){
  score = 0;
  nbrPlant = 0;
}
void ScoreCalculator::setup(){
}
int ScoreCalculator::getScore(){
  return score;
}
void ScoreCalculator::setNbrPlant(int newNbrPlant){
  nbrPlant = newNbrPlant;
}
void ScoreCalculator::addOneSonarPanelToScore(){
  score += pts_per_SolarPanel;
}
void ScoreCalculator::addPlantToScore(){
  score += nbrPlant * pts_per_plants + pts_per_PAMI_PLANTS;
}
void ScoreCalculator::addPAMIToScore(){
  score += 2 * pts_per_PAMI;
}
void ScoreCalculator::addFinalZone(){
  score += pts_final_zone;
}