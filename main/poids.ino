#include <HX711.h> //CAPTEUR : capteur_20kg_B sur pins 13out et 15sck

float get_weigth()
{
  int tempo_local=000;//tempo des cycles d'acquisitions pilotée en local
  int nombre_point=10;                    //c'est le nombre d'acquisitions faites par le HX711, qui en fait une moyenne appelée mesure
  int pinDout =13;
  int pinSCK =15; 
  const float tare_20kg_B = 4000; // tare 20kg_1 : valeur ADC sans rien le 07/05/2019
  const float etalon_20kg_B = 1572; // etalonnage 20kg_1: poids de l'étalon en grammes le 07/05/2019
  const float mesure_20kg_B = 160000 -tare_20kg_B; // etalonnage 20kg_1: valeur ADC avec l'étalon sur la balance moins la tare le 07/05/2019

  delay (tempo_local);//tempo des cycles d'acquisitions sur HX711
  HX711  capteur;
  capteur.begin(  pinDout, pinSCK );//13,15    loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  capteur.power_up();
  delay (HX711_DELAY);  
  float  lecture_capteur = 1111;       
  lecture_capteur = capteur.get_value(nombre_point);//c'est un float moyenné sur x lectures, que l'on va envoyer sur le port serie ou sur eeprom 
  float poids_en_gr=(lecture_capteur-tare_20kg_B)/mesure_20kg_B*etalon_20kg_B;
  Serial.print("   valeur ADC *");Serial.print(lecture_capteur);
  Serial.print("  poids en grammes  ");Serial.print(poids_en_gr);Serial.print(" g \n");
  Serial.println();
  capteur.power_down();// put the ADC in sleep mode
  return poids_en_gr;        
}
