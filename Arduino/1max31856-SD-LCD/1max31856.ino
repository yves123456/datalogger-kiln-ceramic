/* Datalogger 1 MAX1856 Horloge DS3231 SD card affichage LCD I2C et TM1637 LED
*  YLG le 21-12-2023 Suite mesure température four 
*
* 
* Horloge temps réel : 
* DS3231:  SDA pin   -> dedicated SDA pin GRIS
*          SCL pin   -> dedicated SCL pin Blanc
* violet VCC
* bleue GND

* Lecteur de carte SD :
*   SD card attached to SPI bus as follows:
*   MOSI - pin 51 gris
*   MISO - pin 50 blanc
*   CLK - pin 52 violet
*   CS -  4 noir 
     Pin 4 used here for consistency with other Arduino examples
* GND bleue
* Vcc violet

* 
* Thermocouple MAX 31856 SPI: CS, DI, DO, CLK
* CS 6 Violet
* DI 8 brun
* DO 7 blanc
* CLK 5 gris
* VCC rouge
* GND noir

 * Affichage LCD :
 *  Gnd->Gnd Haut fil brun 
 * Vcc->5V  milieu fil rouge
 * SDA A4 fil orange 20
 * SCL A5 fil jaune 21

* TM 1637 afichage LED:
 * connect TM1637 N°1 pin CLK to Arduino fil violet 24
 * connect TM1637 N°1 pin DIO to Arduino fil gris 22 
 * GND bleue 
 * VCC vert

*/

// Affichage Led
#include <TM1637Display.h>
// Create a display object of type TM1637Display
//TM1637Display display1 = TM1637Display(CLK, DIO); DIO jaune clk orange

TM1637Display display1 = TM1637Display(24, 22);


// Thermocouple 
#include <Adafruit_MAX31856.h>
// déclaration de l'obget mathermo et envoie du brochage SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(6, 8, 7, 5);


// include the SD library:
#include <SPI.h>
#include <SD.h>
// set up variables using the SD utility library functions:
Sd2Card card;
//SdVolume volume;
//SdFile root;
const int chipSelect = 4; // pour la carte SD

// DS3231 Horloge 
#include <DS3231.h>
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

// LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // pour télécharger la librairie: croquis - include library - manage libraries - LiquidCrystal_I2C.h de Frank de Brabander
LiquidCrystal_I2C lcd(0x27,16,2); //écran LCD 0x27 avec 16 colonnes et 2 lignes (ligne 0 et ligne 1) 

// VARIABLE 
// variable température temps réel et avant
int templu;  
int templua;  
int templuavant ;
int number ;
int deltatps;
int delta; // delta température entre 2 séquence
int pente ; // delta sur une heure
int tchs1 ;
int sderr ;


// initialisation 


// SETUP

void setup()
{

  // Ouverture port série :
  Serial.begin(9600);

// DS3231 Horloge Initialize the rtc object
  rtc.begin();

// Mise à jour de l'heure et de la date pour l'horloge  
  // The following lines can be uncommented to set the date and time
//rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
//rtc.setTime(18, 57, 0);     // Set the time to 12:00:00 (24hr format)
//rtc.setDate(22, 12, 2023);   // Set the date to January 1st, 2014

// initialisation LCD
  lcd.begin();
  lcd.cursor_on();
  lcd.backlight(); // noBacklight() pour désactiver le retro éclairage
  lcd.setCursor(0,0); // écriture du texte sur la ligne 1 colonne 5 (centré)
  lcd.print("DEBUT !"); // texte "hello" sur ligne 1 colonne 5 
   
// initialitsion Premier thermocouple PREMIER ******************************
  while (!Serial) delay(10);
//  Serial.println("MAX31856 test thermocouple");

// test de fonctionnement du thermocouple 
  if (!maxthermo.begin()) {
    Serial.println("Ne peux pas initiliser le thermocouple.");
   while (1) delay(10);
  }

  // définir le type de thermcouple dans mon cas c'est S
maxthermo.setThermocoupleType(MAX31856_TCTYPE_S); 
  
// définittion de type de conversion 
 maxthermo.setConversionMode(MAX31856_CONTINUOUS);

// filtre du 50hz sur le thermocouple
  maxthermo.setNoiseFilter(MAX31856_NOISE_FILTER_50HZ); 

// initialise transfert vers Excel
//  Initialize_PlxDaq();
// Initialisation SD Card

  templuavant= 0 ;
  delta = 0 ;
  pente = 0 ;
  Initialize_SDcard();
  deltatps = 55;
  tchs1 = 0 ;
  templuavant= maxthermo.readThermocoupleTemperature();

}


// BOUCLE PRINCIPAL

void loop()
{

/*
// Test Horloge
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

// Test Thermocouple  
  Serial.print("Cold Junction Temp: ");
  Serial.println(maxthermo.readCJTemperature());

  Serial.print("hot Junction Temp: ");
  Serial.println(maxthermo.readThermocoupleTemperature()); */


 // Test si Thermocouple est ok 
  uint8_t fault = maxthermo.readFault();
  if (fault) {
    
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");  
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
     tchs1 = 1;
  }

if (not fault) { 
  tchs1 = 0;
}

   // vérification conversion compléte et lire la température
 if (maxthermo.conversionComplete()) {

   if (tchs1 == 0) {
  templua = maxthermo.readCJTemperature();
  templu= maxthermo.readThermocoupleTemperature();
   }  else {
   templu = 0;
   templua = 0;}
        
   

    // affichage LCD de la température et heure
    lcd.setCursor(0, 0); 
    lcd.print(rtc.getDateStr());  
    lcd.print(" ");
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(0, 1); 
    lcd.print("TC");
    lcd.print(templu); 
    lcd.print(" A");
    lcd.print(templua);
    lcd.print(" V");
    lcd.print(templuavant);
    lcd.print(" E:");
    lcd.print(tchs1);
  
  // Envoie sur carte SD et Excel
  if (deltatps == 55) { // on ecrit toute les 3 = 180 x1000 minutes sur la carte 

if (templuavant == 0) { templuavant = templu; }
      
      delta= templu - templuavant  ;
      pente = delta * 60;
            
  //  Write_PlxDaq();
      Write_SDcard();
      Serial.print(rtc.getDateStr());
      Serial.print(" ");
      Serial.print(rtc.getTimeStr());
      Serial.print(" ");
      Serial.print(templu); 
      Serial.print(" ");
      Serial.print(templua);
      Serial.print(" ");
      Serial.print(templuavant);
      Serial.print(" ");
      Serial.print(delta);
      Serial.print(" ");
      Serial.print(pente);
      Serial.print(" ");
      Serial.print(tchs1);
      Serial.println(); //End of Row move to next row 
            
      templuavant = templu;
      deltatps = 0;
    }

    deltatps = deltatps +1 ;

   delay (10);
     
 } else {
   lcd.print("Err thermo 1");
 }

 // Set the brightness to 5 (0=dimmest 7=brightest)
  display1.setBrightness(5);
  display1.showNumberDec(templu); 

  // Wait one second before repeating :)
  delay (1000);
 
}

// FIN DE LOOP

// Début module 


// transferts vers  Excel
void Write_PlxDaq()
  {
    Serial.print("DATA"); //always write "DATA" to Indicate the following as Data
    Serial.print(","); //Move to next column using a ","
    Serial.print("DATE"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","
    Serial.print("TIME"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","
    Serial.print(templu); 
    Serial.print(","); //Move to next column using a ","
    Serial.print(templua);
    Serial.print(","); //Move to next column using a ","
    Serial.print(templuavant);
    Serial.print(",");
    Serial.print(delta);
    Serial.print(",");
    Serial.print(pente);
    Serial.print(","); //Move to next column using a ","
    Serial.print(tchs1); //Move to next column using a ","
    Serial.println(); //End of Row move to next row
  }

// initailisation vers Excel
void Initialize_PlxDaq()
{
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,date, Heure,TCouple,TAmb,TAvant,Delta,Pente,ER"); //always write LABEL, to indicate it as first line
}



// initialisation SD Card
void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Date;Time;TCouple;TAmb;TAvant;Delta;Pente;ER"); //Write the first row of the excel file
    dataFile.close();
  }

  
} // fin initialise SD

// écrire sur carte SD
void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDateStr()); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(rtc.getTimeStr()); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templu); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templua); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(templuavant); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(delta); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(pente); //Store data on SD card
    dataFile.print(";"); //Move to next column using a ","
    dataFile.print(tchs1); //Store data on SD card
    dataFile.println(); //End of Row move to next row
    
    dataFile.close(); //Close the file
  }
  else
  Serial.println("Attention SD invalide");
}
