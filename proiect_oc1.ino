// C++ code

#include <Adafruit_LiquidCrystal.h>

#define MOD_ACCESORII 30
#define MOD_PORNIT 60
#define FARURI_OPRITE 50
#define USA_DESCHISA 50

#define AFISAJ_USA_DESCHISA '-'
#define AFISAJ_USA_INCHISA_STANGA '/'
#define AFISAJ_USA_INCHISA_DREAPTA '\\'

#define AFISAJ_MOD_OPRIT "OFF"
#define AFISAJ_MOD_ACCESORII "ACC"
#define AFISAJ_MOD_PORNIT "ON "

#define AFISAJ_FRANA_NEACTIONATA '.'
#define AFISAJ_FRANA_ACTIONATA '#'

#define AFISAJ_FARURI_OPRITE "OFF"
#define AFISAJ_FARURI_PORNITE "ON "

#define MESAJ_MOTOR "IGNITION-"
#define MESAJ_FARURI "FARURI-"


/**********************
*	"masina_lcd.h"	  *
**********************/
#ifndef MASINA_LCD_H
#define MASINA_LCD_H

//structura descrie la ce pozitie apare un afisaj pe ecranul LCD 16x2
typedef struct coordonate_lcd {
	int index1;
  	int index2;
}CoordonateLCD;

//definirea functiilor din biblioteca "masina_lcd.h"

void afisare_mesaj_motor(CoordonateLCD coord_mesaj_motor);
void afisare_mesaj_faruri(CoordonateLCD coord_mesaj_faruri);
void afisare_usi_lcd(int procentaj_usa, CoordonateLCD c1, CoordonateLCD c2);
void afisare_mod_motor_lcd(int procentajMotor, CoordonateLCD coord_motor);
void afisare_frana_lcd(int procentajMotor, CoordonateLCD coord_frana);
void afisare_faruri_lcd(int procentajMotor, int procentajFaruri, CoordonateLCD coord_faruri);

#endif


/****************************
*	DEFINIRE PINI ARDUINO	*
****************************/
Adafruit_LiquidCrystal lcd_1(0);			//referinta la ecranul lcd
const int pin_photo = 9;					//pin arduino ce conecteaza ecranul lcd
const int pinButonFrana = 2;				//pin arduino buton frana 
const int pinPotentiometruMotor = A0;		//pin arduino potentiometru motor
const int ledPinStopuri = 3;				//pin arduino stopuri
const int ledPinFaruri = 4;					//pin arduino faruri
const int pinFotorezistentaFaruri = A3;		//pin arduino fotorezistenta faruri
const int pinUsiFata = A1;					//pin arduino usi fata (stanga + dreapta)
const int pinUsiSpate = A2;					//pin arduino usi spate (stanga + dreapta)


/***************************
*	IMPLEMENTARE FUNCTII   *
***************************/
void afisare_mesaj_motor(CoordonateLCD coord_mesaj_motor) {
	lcd_1.setCursor(coord_mesaj_motor.index1, coord_mesaj_motor.index2);	//setam cursorul pe pozitia (coord_mesaj_motor.index1, coord_mesaj_motor.index2)
  	lcd_1.print(MESAJ_MOTOR);												//afisare indicator motor
}

void afisare_mesaj_faruri(CoordonateLCD coord_mesaj_faruri) {
	lcd_1.setCursor(coord_mesaj_faruri.index1, coord_mesaj_faruri.index2);	//setam cursorul pe pozitia (coord_mesaj_faruri.index1, coord_mesaj_faruri.index2)
    lcd_1.print(MESAJ_FARURI);												//afisare indicator faruri

}

void afisare_usi_lcd(int procentaj_usa, CoordonateLCD c1, CoordonateLCD c2) {
    if(procentaj_usa > USA_DESCHISA){			  
      lcd_1.setCursor(c1.index1, c1.index2);	//setam cursorul pe pozitia (c1.index1, c1.index2)
      lcd_1.print(AFISAJ_USA_DESCHISA);			//usa deschisa
      lcd_1.setCursor(c2.index1, c2.index2);	//setam cursorul pe pozitia (c2.ndex1, c2index2)
      lcd_1.print(AFISAJ_USA_DESCHISA);		    //usa deschisa
    } else {
      lcd_1.setCursor(c1.index1, c1.index2);	//setam cursorul pe pozitia (c1.index1, c1.index2)
      lcd_1.print(AFISAJ_USA_INCHISA_STANGA);	//usa inchisa
      lcd_1.setCursor(c2.index1, c2.index2); 	//setam cursorul pe pozitia (c2.index1, c2.index2)
      lcd_1.print(AFISAJ_USA_INCHISA_DREAPTA);	//usa inchisa
    }
}

void afisare_mod_motor_lcd(int procentajMotor, CoordonateLCD coord_motor) {
	
  	lcd_1.setCursor(coord_motor.index1, coord_motor.index2);	//setam cursorul pe pozitia 11.0
  	
  	//daca valoarea potentiometrului este mai mica decat valoarea de atingere a modului accesorii
    if(procentajMotor <= MOD_ACCESORII) {
     	lcd_1.print(AFISAJ_MOD_OPRIT);			//mod oprit
    } else if(procentajMotor <= MOD_PORNIT) {	//valoarea este mai mica decat cea a motorului pornit
      	lcd_1.print(AFISAJ_MOD_ACCESORII);		//mod accesorii
    } else {
      	lcd_1.print(AFISAJ_MOD_PORNIT);			//mod pornit
    }
}

void afisare_frana_lcd(int procentajMotor, CoordonateLCD coord_frana) {
	lcd_1.setCursor(coord_frana.index1, coord_frana.index2);		 //setam cursorul pe pozitia 13.1
  	lcd_1.print(AFISAJ_FRANA_NEACTIONATA);							 //by default frana nu este actionata si afisam "."
    if(procentajMotor > MOD_ACCESORII) {							   	 //daca motorul este pornit
        if(digitalRead(pinButonFrana) == LOW){						 //daca frana nu este actionata, afisajul nu se schimba
          	digitalWrite(ledPinStopuri, LOW);
        } else {
          	digitalWrite(ledPinStopuri, HIGH);
          	lcd_1.setCursor(coord_frana.index1, coord_frana.index2); //setam cursorul pe pozitia 13.1
          	lcd_1.print(AFISAJ_FRANA_ACTIONATA);				 	 //cand frana este actionatata si motorul este pornit afisam "#"
        }
    }
}

void afisare_faruri_lcd(int procentajMotor, int procentajFaruri, CoordonateLCD coord_faruri) {
	if(procentajMotor > MOD_ACCESORII){				//daca motorul este cel putin in modul accesorii
        if(procentajFaruri < FARURI_OPRITE){		//daca fororezistenta faruri este sub valoarea setata pentru faruri oprite
          	digitalWrite(ledPinFaruri, HIGH);		//atunci pornim farurile
          	lcd_1.print(AFISAJ_FARURI_PORNITE);	    //pe lcd vom afisare mesajul "ON " dupa indicator faruri
        } else {									//in cazul in care valoarea este peste cea setata
          	digitalWrite(ledPinFaruri, LOW);		//farurile sunt stinse
          	lcd_1.print(AFISAJ_FARURI_OPRITE);	    //pe lcd vom afisare mesajul "OFF" dupa indicator faruri
        }
    } else {										//in cazul in care motorul este oprit
      	digitalWrite(ledPinFaruri, LOW);			//farurile sunt stinse
      	lcd_1.print(AFISAJ_FARURI_OPRITE);			//pe lcd vom afisare mesajul "OFF" dupa indicator faruri
    }
}

void setup() {
  	pinMode(pin_photo, INPUT);
  	Serial.begin(9600);
  	lcd_1.begin(16, 2);
    pinMode(ledPinStopuri, OUTPUT);
    pinMode(ledPinFaruri, OUTPUT);
    pinMode(pinButonFrana, INPUT);
}

void loop() {

  	/*******************
    *	CITIRE DATE	   *
	*******************/
  	int valoarePotentiometruMotor = analogRead(pinPotentiometruMotor);					//extragere date motor
    int procentajPotentiometruMotor = map(valoarePotentiometruMotor, 0, 1023, 0, 100);	//calculam procentajul
    
  	int dataFaruri = analogRead(pinFotorezistentaFaruri);		//extragere date faruri
  	int procentajFaruri = map(dataFaruri, 0, 1023, 0, 100);		//calculam procentajul fotorezistenta faruri
  	
  	int dataUsiFata = analogRead(pinUsiFata);					//extragere date usi fata
  	int procentajUsiFata = map(dataUsiFata, 0, 1023, 0, 100);	//calculam procentajul fotorezistenta usi fata
  
  	int dataUsiSpate = analogRead(pinUsiSpate);					//extragere date usi spate
  	int procentajUsiSpate = map(dataUsiSpate, 0, 1023, 0, 100);	//calculam procentajul fotorezistenta usi spate
 
	/*******************************************
	*	TRANSMITEM INFORMATIA PE ECRANUL LCD   *	
	*******************************************/
  	CoordonateLCD coord_mesaj_motor = {2, 0};
  	afisare_mesaj_motor(coord_mesaj_motor);
  
    CoordonateLCD coord_motor = {11, 0};
  	afisare_mod_motor_lcd(procentajPotentiometruMotor, coord_motor);	
  	
  	CoordonateLCD coord_mesaj_faruri = {2, 1};
  	afisare_mesaj_faruri(coord_mesaj_faruri);

  	CoordonateLCD coord_faruri = {9, 1};
  	afisare_faruri_lcd(procentajPotentiometruMotor, procentajFaruri, coord_faruri);
  
  	CoordonateLCD coord_frana = {13, 1};
  	afisare_frana_lcd(procentajPotentiometruMotor, coord_frana); 
  
    CoordonateLCD coord_usa_stg_fata = {0, 0};
	CoordonateLCD coord_usa_dr_fata = {15, 0};
	afisare_usi_lcd(procentajUsiFata, coord_usa_stg_fata, coord_usa_dr_fata);  	

  	CoordonateLCD coord_usa_stg_spate = {0, 1};
	CoordonateLCD coord_usa_dr_spate = {15, 1};
  	afisare_usi_lcd(procentajUsiSpate, coord_usa_stg_spate, coord_usa_dr_spate);
  
}