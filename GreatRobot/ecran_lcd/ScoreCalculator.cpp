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
  Log.trace("SCORE : set the number of plant to  %d " CR, nbrPlant);
}
void ScoreCalculator::addOneSonarPanelToScore(){
  score += pts_per_SolarPanel;
  Log.trace("SCORE : add solar panel points (new score: %d )" CR, score);
}
void ScoreCalculator::addPlantToScore(){
  score += nbrPlant * pts_per_plants + pts_per_PAMI_PLANTS;
  Log.trace("SCORE : add solar plant points to score (new score: %d )" CR, score);
}
void ScoreCalculator::addPAMIToScore(){
  score += 2 * pts_per_PAMI;
  Log.trace("SCORE : add PAMI points to score (new score: %d )" CR, score);
}
void ScoreCalculator::addFinalZone(){
  score += pts_final_zone;
  Log.trace("SCORE : add final zone points to score (new score: %d )" CR, score);
}