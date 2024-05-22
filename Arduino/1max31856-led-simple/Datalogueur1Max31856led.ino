
/*
 * Datalogueur avec 1 thermocouple MAX31856 Afficheur LED simple 16-04-2024
 * 
 * 
 * 

TC 1
CS brun 13
SCK violet 12
DO bleu 11
DI vert 10


Use software SPI: CS, DI, DO, CLK pour le brochage de la carte
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13) port utilisé pour le SPI;
use hardware SPI, just pass in the CS pin n'est obligatoire que CS posistion 10
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);
 
 * 
 *  The circuit:
 * connect TM1637 N°1 pin CLK to Arduino fil blanc 6
 * connect TM1637 N°1 pin DIO to Arduino fil gris 7
 * 
 * TM1637Display display1 = TM1637Display(CLK, DIO); clk orange 24 dio jaune 22
TM1637Display display1 = TM1637Display(6, 7);
 * 
 * 
 */


// Thermo
#include <Adafruit_MAX31856.h>

// décalaration de l'obget mathermo et envoie du brochage SPI dans l'ordre CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(13, 10, 11, 12);


// décalaration de l'obget mathermo et envoie du brochage SPI dans l'ordre CS, DI, DO, CLK
/*Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(3, 8, 7, 5);
Adafruit_MAX31856 maxthermoA = Adafruit_MAX31856(6, 8, 7, 5);*/


// Affichage Led
#include <TM1637Display.h>
// Create a display object of type TM1637Display
//TM1637Display display1 = TM1637Display(CLK, DIO); clk orange 24 dio jaune 22
TM1637Display display1 = TM1637Display(6,7 );

// variable température temps réel et avant
int templu;  
int templuamb;

int templuavant ;
int number ;
int delta; // delta température entre 2 séquence
int pente ; // delta sur une heure

int deltatps;
int tchs1 ;
int tcerr ;
int sderr ;

// initialisation 

void setup() {
       
    // début de la communication du port série 
    Serial.begin(9600);


// DEBUT du thermocouple
while (!Serial) {delay(10);}
  delay(100);
  maxthermo.begin();
    
  delay(100);
  if (!maxthermo.begin()) {
    delay(1000);
 }
  

delay(500);
maxthermo.setThermocoupleType(MAX31856_TCTYPE_S);
delay(500);
maxthermo.setConversionMode(MAX31856_CONTINUOUS);
delay(500);
maxthermo.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// FIN du thermocouple


/* traitement des led */ 
pinMode(42,OUTPUT);
pinMode(44,OUTPUT);


tchs1 = 0 ;
deltatps = 45;
templuavant= 0 ;
delta = 0 ;
pente = 0 ;

} // fin boucle initialisation /** fin void setup **/


// ************** boucle principal ARDUINO ************************

void loop() {

// Test thermocouple 

 
  // Check and print any faults
  uint8_t fault = maxthermo.readFault();
  if (fault) {
  
    if (fault & MAX31856_FAULT_CJRANGE) /* Serial.println("Cold Junction Range Fault")*/; 
    if (fault & MAX31856_FAULT_TCRANGE) /*  Serial.println("Thermocouple Range Fault")*/ ;
    if (fault & MAX31856_FAULT_CJHIGH)  /* Serial.println("Cold Junction High Fault")*/;
    if (fault & MAX31856_FAULT_CJLOW)  /*  Serial.println("Cold Junction Low Fault")*/ ;
    if (fault & MAX31856_FAULT_TCHIGH) /*   Serial.println("Thermocouple High Fault")*/;
    if (fault & MAX31856_FAULT_TCLOW)  /*  Serial.println("Thermocouple Low Fault")*/ ;
    if (fault & MAX31856_FAULT_OVUV)  /*   Serial.println("Over/Under Voltage Fault")*/;
    if (fault & MAX31856_FAULT_OPEN)  /*   Serial.println("Thermocouple Open Fault")*/;
    tchs1 = 1;
    
  }

if (not fault) { 
  tchs1 = 0;
}

  // vérification conversion compléte et lire la température thermocouple 1
 if (maxthermo.conversionComplete()) {
      // sauvegarde de la température
   if (tchs1 == 0) {
        templu= maxthermo.readThermocoupleTemperature();
   } else {
   templu = 0;
  }
        templuamb = maxthermo.readCJTemperature() ;
   } else {
   Serial.println("Conversion incomplète! thermo 1");
   templu = 0;
   tchs1 = 1;
 }



 
  //if (deltatps == 50) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte prod
  if (deltatps == 50) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte  test

      if (templuavant == 0) { templuavant = templu; }
      delta= templu - templuavant  ;
      pente = delta * 60;

    
/* Sortie vers la pi */
    
    Serial.print(templu); 
    Serial.print(" "); 
    Serial.print(templuamb);
    Serial.print(" ");
    Serial.print(templuavant);
    Serial.print(" ");
    Serial.print(delta);
    Serial.print(" ");
    Serial.print(pente);
    Serial.print(" ");
    Serial.print(tchs1);
    Serial.println(); //End of Row move to next row
/* Sortie vers la pi */
    templuavant = templu;
    deltatps = 0;
    
    }

    deltatps = deltatps +1 ;
//Serial.print(deltatps); 
    
 // Set the brightness to 5 (0=dimmest 7=brightest)
  display1.setBrightness(5);
  display1.showNumberDec(templu); 



tcerr = tchs1  ;
  if ( tcerr > 0 ) { 
  
   digitalWrite(44, HIGH); 
  }
else {
   digitalWrite(44, LOW); 
  }
 

  delay(2000); // délais de la boucle = une seconde 
  

// sortie

 
} // fin loop

// **********  Fin LOOP *****************

// objets
