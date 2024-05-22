
/*
 * Datalogueur avec 4 thermocouple MAX31856 Afficheur LED 10-04-2024 pour boitier bois 4 TC
 * 
 * 
 * 
 * 

* branchement MAX31856 SPI pour les 4 thermocouples
* // Use software SPI: CS, DI, DO, CLK
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(13, 10, 11, 12);
Adafruit_MAX31856 maxthermoA = Adafruit_MAX31856(5, 2, 3, 4);
Adafruit_MAX31856 maxthermoB = Adafruit_MAX31856(16, 19, 18, 17);
Adafruit_MAX31856 maxthermoC = Adafruit_MAX31856(42, 48, 46, 44);

fil rouge VCC
fil noir GND

TC 1
CS brun 13
SCK violet 12
DO bleu 11
DI vert 10

TC 2
CS jaune 5
SCK vert 4
DO bleu 3
DI violet 2

TC 3
CS jaune 16
SCK vert 17
DO bleu 18
DI violet 19

TC 4
CS jaune 42
SCK vert 44
DO bleu 46
DI violet 48

 * 
 *  TM1637 affichage LED :
 * connect TM1637 N°1 pin CLK to Arduino fil 26 violet
 * connect TM1637 N°1 pin DIO to Arduino fil 27 bleue
 * 
 * connect TM1637 N°2 pin CLK to Arduino fil jaune 28
 * connect TM1637 N°2 pin DIO to Arduino fil orange 29
 * 
 * connect TM1637 N°3 pin CLK to Arduino fil jaune 30
 * connect TM1637 N°3 pin DIO to Arduino fil orange 31
 * 
 * connect TM1637 N°3 pin CLK to Arduino fil jaune 32
 * connect TM1637 N°3 pin DIO to Arduino fil orange 33
 
TM1637Display display1 = TM1637Display(CLK, DIO)
TM1637Display display1 = TM1637Display(26,27 );
TM1637Display display2 = TM1637Display(28,29 );
TM1637Display display3 = TM1637Display(30,31 );
TM1637Display display4 = TM1637Display(32,33 );
 * 
 * 
 * 
 */


// *** Thermo MAX31856 ****
#include <Adafruit_MAX31856.h>

// décalaration de l'obget mathermo et envoie du brochage SPI dans l'ordre CS, DI, DO, CLK
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// Use software SPI: CS, DI, DO, CLK
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(13, 11, 12, 12);
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(13, 10, 11, 12);
Adafruit_MAX31856 maxthermoA = Adafruit_MAX31856(5, 2, 3, 4);
Adafruit_MAX31856 maxthermoB = Adafruit_MAX31856(16, 19, 18, 17);
Adafruit_MAX31856 maxthermoC = Adafruit_MAX31856(42, 48, 46, 44);
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(CS, DI, DO, CLK);



// Affichage Led
#include <TM1637Display.h>
// Create a display object of type TM1637Display
//TM1637Display display1 = TM1637Display(CLK, DIO); clk orange 24 dio jaune 22
TM1637Display display1 = TM1637Display(26,27 );
TM1637Display display2 = TM1637Display(28,29 );
TM1637Display display3 = TM1637Display(30,31 );
TM1637Display display4 = TM1637Display(32,33 );

// variable température temps réel et avant
int templu;  
int templuA; 
int templuB;  
int templuC;
  
int templuamb;
int templuambA;
int templuambB;
int templuambC;

int deltatps;

int tchs1 ;
int tchs2 ;
int tchs3 ;
int tchs4 ;

int tcerr ;
int sderr ;

// initialisation 

void setup() {
       
    // début de la communication du port série 
    Serial.begin(9600);

// DEBUT des 4 thermocouple
// TC1
while (!Serial) {delay(10);}
  delay(10);
  //Serial.println("TC1 Begin");
  //delay(10);
  maxthermo.begin();


// TC2
while (!Serial) {delay(10);}
  delay(10);
  //Serial.println("TC2 Begin");
  //delay(10);
  maxthermoA.begin();  
 
// TC3
while (!Serial) {delay(10);}
  delay(10);
  //Serial.println("TC3 Begin");
  //delay(10);
  maxthermoB.begin();  

// TC4
while (!Serial) {delay(10);}
  delay(10);
  //Serial.println("TC4 Begin");
  //delay(10);
  maxthermoC.begin();  


delay(10);
maxthermo.setThermocoupleType(MAX31856_TCTYPE_S);
delay(10);
maxthermoA.setThermocoupleType(MAX31856_TCTYPE_S);
delay(10);
maxthermoB.setThermocoupleType(MAX31856_TCTYPE_S);
delay(10);
maxthermoC.setThermocoupleType(MAX31856_TCTYPE_S);


delay(10);
maxthermo.setConversionMode(MAX31856_CONTINUOUS);
delay(10);
maxthermoA.setConversionMode(MAX31856_CONTINUOUS);
delay(10);
maxthermoB.setConversionMode(MAX31856_CONTINUOUS);
delay(10);
maxthermoC.setConversionMode(MAX31856_CONTINUOUS);

delay(10);
maxthermo.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 
delay(10);
maxthermoA.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 
delay(10);
maxthermoB.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 
delay(10);
maxthermoC.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 


// FIN des 4 thermocouple


/* traitement des led */ 
//pinMode(42,OUTPUT);
//pinMode(44,OUTPUT);


tchs1 = 0 ;
tchs2 = 0;
tchs3 = 0;
tchs4 = 0;
deltatps = 45;

} // fin boucle initialisation /** fin void setup **/


// ************** boucle principal ARDUINO ************************

void loop() {

// Test thermocouple 

 
  // Check and print any faults TC1
  uint8_t fault = maxthermo.readFault();
    if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) /*  Serial.println("Cold Junction Range Fault TC1")*/ ; 
    if (fault & MAX31856_FAULT_TCRANGE) /*  Serial.println("Thermocouple Range Fault TC1") */;
    if (fault & MAX31856_FAULT_CJHIGH)  /* Serial.println("Cold Junction High Fault TC1")*/;
    if (fault & MAX31856_FAULT_CJLOW)   /* Serial.println("Cold Junction Low Fault TC1") */;
    if (fault & MAX31856_FAULT_TCHIGH)  /*  Serial.println("Thermocouple High Fault TC1") */;
    if (fault & MAX31856_FAULT_TCLOW)  /*  Serial.println("Thermocouple Low Fault TC1") */;
    if (fault & MAX31856_FAULT_OVUV)   /*  Serial.println("Over/Under Voltage Fault TC1")*/;
    if (fault & MAX31856_FAULT_OPEN)   /*  Serial.println("Thermocouple Open Fault TC1") */;
    tchs1 = 1; }
    if (not fault) { 
    tchs1 = 0;     }


  // Check and print any faults TC2
  uint8_t faultA = maxthermoA.readFault();
    if (faultA) {
    if (faultA & MAX31856_FAULT_CJRANGE)  /* Serial.println("Cold Junction Range Fault TC2")*/; 
    if (faultA & MAX31856_FAULT_TCRANGE) /*  Serial.println("Thermocouple Range Fault TC2")*/ ;
    if (faultA & MAX31856_FAULT_CJHIGH)  /* Serial.println("Cold Junction High Fault TC2")*/;
    if (faultA & MAX31856_FAULT_CJLOW)  /*  Serial.println("Cold Junction Low Fault TC2")*/ ;
    if (faultA & MAX31856_FAULT_TCHIGH)  /*  Serial.println("Thermocouple High Fault TC2")*/ ;
    if (faultA & MAX31856_FAULT_TCLOW)   /* Serial.println("Thermocouple Low Fault TC2")*/ ;
    if (faultA & MAX31856_FAULT_OVUV)   /*  Serial.println("Over/Under Voltage Fault TC2")*/;
    if (faultA & MAX31856_FAULT_OPEN)  /*   Serial.println("Thermocouple Open Fault TC2")*/ ;
    tchs2 = 1;  }
if (not faultA) { 
  tchs2 = 0; }


  // Check and print any faults TC3
  uint8_t faultB = maxthermoB.readFault();
    if (faultB) {
    if (faultB & MAX31856_FAULT_CJRANGE)/*   Serial.println("Cold Junction Range Fault TC3")*/; 
    if (faultB & MAX31856_FAULT_TCRANGE)/*   Serial.println("Thermocouple Range Fault TC3")*/ ;
    if (faultB & MAX31856_FAULT_CJHIGH)/*   Serial.println("Cold Junction High Fault TC3")*/;
    if (faultB & MAX31856_FAULT_CJLOW) /*   Serial.println("Cold Junction Low Fault TC3") */;
    if (faultB & MAX31856_FAULT_TCHIGH) /*   Serial.println("Thermocouple High Fault TC3")*/ ;
    if (faultB & MAX31856_FAULT_TCLOW) /*   Serial.println("Thermocouple Low Fault TC3")*/ ;
    if (faultB & MAX31856_FAULT_OVUV)  /*   Serial.println("Over/Under Voltage Fault TC3")*/;
    if (faultB & MAX31856_FAULT_OPEN)  /*   Serial.println("Thermocouple Open Fault TC3")*/ ;
    tchs3 = 1; }
if (not faultB) { 
  tchs3 = 0;
}

  // Check and print any faults TC4
  uint8_t faultC = maxthermoC.readFault();
    if (faultC) {
    if (faultC & MAX31856_FAULT_CJRANGE) /*  Serial.println("Cold Junction Range Fault TC4")*/; 
    if (faultC & MAX31856_FAULT_TCRANGE)/*   Serial.println("Thermocouple Range Fault TC4")*/;
    if (faultC & MAX31856_FAULT_CJHIGH) /*  Serial.println("Cold Junction High Fault TC4")*/;
    if (faultC & MAX31856_FAULT_CJLOW) /*   Serial.println("Cold Junction Low Fault TC4")*/ ;
    if (faultC & MAX31856_FAULT_TCHIGH)/*    Serial.println("Thermocouple High Fault TC4")*/ ;
    if (faultC & MAX31856_FAULT_TCLOW)  /*  Serial.println("Thermocouple Low Fault TC4") */;
    if (faultC & MAX31856_FAULT_OVUV)  /*   Serial.println("Over/Under Voltage Fault TC4")*/;
    if (faultC & MAX31856_FAULT_OPEN)  /*   Serial.println("Thermocouple Open Fault TC4")*/ ;
    tchs4 = 1; }
if (not faultC) { 
  tchs4 = 0;
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

delay(10);

  // vérification conversion compléte et lire la température thermocouple 2
   if (maxthermoA.conversionComplete()) {
      // sauvegarde de la température
   if (tchs2 == 0) {
       templuA= maxthermoA.readThermocoupleTemperature();
    } else {
    templuA = 0;
    }
    templuambA = maxthermoA.readCJTemperature() ;
    } else {
    Serial.println("Conversion incomplète! thermo 2");
    templuA = 0;
    tchs2 = 1;
    }

delay(10);

  // vérification conversion compléte et lire la température thermocouple 3
   if (maxthermoB.conversionComplete()) {
      // sauvegarde de la température
   if (tchs3 == 0) {
       templuB= maxthermoB.readThermocoupleTemperature();
    } else {
    templuB = 0;
    }
    templuambB = maxthermoB.readCJTemperature() ;
    } else {
    Serial.println("Conversion incomplète! thermo 3");
    templuB = 0;
    tchs3 = 1;
    }

delay(10);

  // vérification conversion compléte et lire la température thermocouple 4
   if (maxthermoC.conversionComplete()) {
      // sauvegarde de la température
   if (tchs4 == 0) {
       templuC= maxthermoC.readThermocoupleTemperature();
    } else {
    templuC = 0;
    }
    templuambC = maxthermoC.readCJTemperature() ;
    } else {
    Serial.println("Conversion incomplète! thermo 4");
    templuC = 0;
    tchs4 = 1;
    }

  //if (deltatps == 50) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte prod
  if (deltatps == 50) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte  test
    
/* Sortie vers la pi */
    Serial.print(templu); 
    Serial.print(" "); 
    //Serial.print(templuamb);
    //Serial.print(" "); 
    Serial.print(templuA); 
    Serial.print(" "); 
    //Serial.print(templuambA);
    //Serial.print(" "); 
    Serial.print(templuB); 
    Serial.print(" "); 
    //Serial.print(templuambB);
    //Serial.print(" "); 
    Serial.print(templuC); 
    Serial.print(" "); 
    //Serial.print(templuambC);
    Serial.print(" "); 
    Serial.print(tchs1);
    Serial.print(" "); 
    Serial.print(tchs2);
    Serial.print(" "); 
    Serial.print(tchs3);
    Serial.print(" "); 
    Serial.print(tchs4);
   Serial.println(); //End of Row move to next row
/* Sortie vers la pi */
   
    deltatps = 0;
    }

    deltatps = deltatps +1 ;
    
 // Set the brightness to 5 (0=dimmest 7=brightest)
  display1.setBrightness(5);
  display1.showNumberDec(templu); 

// Set the brightness to 5 (0=dimmest 7=brightest)
 display2.setBrightness(5);
 display2.showNumberDec(templuA); 

// Set the brightness to 5 (0=dimmest 7=brightest)
 display3.setBrightness(5);
 display3.showNumberDec(templuB); 


// Set the brightness to 5 (0=dimmest 7=brightest)
 display4.setBrightness(5);
 display4.showNumberDec(templuC); 

 

  delay(2000); // délais de la boucle = une seconde 
  

// sortie

 
} // fin loop

// **********  Fin LOOP *****************

// objets
