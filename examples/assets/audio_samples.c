/**
 * En este archivo se implementan varias funciones para reproducir algunas 
 * melodías conocidas.
 */

#include "drivers/audio_driver.h"


void PlayStrangerThings() {

	const uint16_t t = 200;
	/*
	for (uint8_t i = 0; i < 4; i++) {

		MDT_AUDIO_AddNote(A3, t);
		MDT_AUDIO_AddNote(C4, t);
		MDT_AUDIO_AddNote(E4, t);
		MDT_AUDIO_AddNote(G4, t);
		MDT_AUDIO_AddNote(A4, t);
		MDT_AUDIO_AddNote(G4, t);
		MDT_AUDIO_AddNote(E4, t);
		MDT_AUDIO_AddNote(C4, t);

		MDT_AUDIO_AddNote(0, 10);// delay(10);
	
	}
	*/	
	
	for (uint8_t i = 0; i < 4; i++) {

		MDT_AUDIO_AddNote(C3, t);
		MDT_AUDIO_AddNote(E3, t);
		MDT_AUDIO_AddNote(G3, t);
		MDT_AUDIO_AddNote(B3, t);
		MDT_AUDIO_AddNote(C4, t);
		MDT_AUDIO_AddNote(B3, t);
		MDT_AUDIO_AddNote(G3, t);
		MDT_AUDIO_AddNote(E3, t);
		
		MDT_AUDIO_AddNote(0, 10);// delay(10);

	}
	
	
	MDT_AUDIO_Play();

}

void PlayAquaticAmbience() {

	MDT_AUDIO_AddNote(D4, 250);
	MDT_AUDIO_AddNote(F4, 250);
	MDT_AUDIO_AddNote(E4, 250);
	MDT_AUDIO_AddNote(B4, 250);
	MDT_AUDIO_AddNote(G4, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(E5, 250);
	MDT_AUDIO_AddNote(B5, 250);
	MDT_AUDIO_AddNote(G5, 250);
	
	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(E5, 250);
	MDT_AUDIO_AddNote(B5, 250);
	MDT_AUDIO_AddNote(G5, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(E5, 250);
	MDT_AUDIO_AddNote(B5, 250);
	MDT_AUDIO_AddNote(G5, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(C6, 250);
	MDT_AUDIO_AddNote(D5, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(C6, 250);
	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(C6, 250);
	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(C6, 250);

	MDT_AUDIO_AddNote(0, 10);// delay(10);

	MDT_AUDIO_AddNote(D5, 250);
	MDT_AUDIO_AddNote(F5, 250);
	MDT_AUDIO_AddNote(C6, 250);
	MDT_AUDIO_AddNote(D5, 250);

	MDT_AUDIO_Play();

}

void PlayMarchaImperial() {

	MDT_AUDIO_AddNote( A4, 500);
	MDT_AUDIO_AddNote( 0, 10);//	delay(10);
	MDT_AUDIO_AddNote( A4, 500);
	MDT_AUDIO_AddNote( 0, 10);//	delay(10);
	MDT_AUDIO_AddNote( A4, 500);
	MDT_AUDIO_AddNote( F4, 350);
	MDT_AUDIO_AddNote( C5, 150);
	MDT_AUDIO_AddNote( A4, 500);
	MDT_AUDIO_AddNote( F4, 350);
	MDT_AUDIO_AddNote( C5, 150);
	MDT_AUDIO_AddNote( A4, 1000);

	MDT_AUDIO_AddNote( E5, 500);
	MDT_AUDIO_AddNote( 0, 10);//	delay(10);
	MDT_AUDIO_AddNote( E5, 500);
	MDT_AUDIO_AddNote( 0, 10);//	delay(10);
	MDT_AUDIO_AddNote( E5, 500);
	MDT_AUDIO_AddNote( F5, 350);
	MDT_AUDIO_AddNote( C5, 150);
	MDT_AUDIO_AddNote( Gs4, 500);
	MDT_AUDIO_AddNote( F4, 350);
	MDT_AUDIO_AddNote( C5, 150);
	MDT_AUDIO_AddNote( A4, 1000);

	MDT_AUDIO_AddNote( A5, 500);
	MDT_AUDIO_AddNote( A4, 350);
	MDT_AUDIO_AddNote( A4, 150);
	MDT_AUDIO_AddNote( A5, 500);
	MDT_AUDIO_AddNote( Gs5, 250);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( F5, 125);
	MDT_AUDIO_AddNote( Fs5, 250);

	MDT_AUDIO_AddNote(0, 250);//	delay(250);

	MDT_AUDIO_AddNote( As4, 250);
	MDT_AUDIO_AddNote( Ds5, 500);
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( Cs5, 250);


	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( C5, 250);

	MDT_AUDIO_AddNote( 0, 250);//	delay(250);

	MDT_AUDIO_AddNote( F4, 250);
	MDT_AUDIO_AddNote( Gs4, 500);
	MDT_AUDIO_AddNote( F4, 375);
	MDT_AUDIO_AddNote( A4, 125);
	MDT_AUDIO_AddNote( C5, 500);
	MDT_AUDIO_AddNote( A4, 375);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( E5, 1000);
	MDT_AUDIO_AddNote( A5, 500);
	MDT_AUDIO_AddNote( A4, 350);
	MDT_AUDIO_AddNote( A4, 150);
	MDT_AUDIO_AddNote( A5, 500);
	MDT_AUDIO_AddNote( Gs5, 250);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( F5, 125);
	MDT_AUDIO_AddNote( Fs5, 250);

	MDT_AUDIO_AddNote( 0, 250);//	delay(250);

	MDT_AUDIO_AddNote( As4, 250);
	MDT_AUDIO_AddNote( Ds5, 500);
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( Cs5, 250);


	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( C5, 250);

	MDT_AUDIO_AddNote( 0, 250);//	delay(250);

	MDT_AUDIO_AddNote( F4, 250);
	MDT_AUDIO_AddNote( Gs4, 500);
	MDT_AUDIO_AddNote( F4, 375);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( A4, 500);
	MDT_AUDIO_AddNote( F4, 375);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( A4, 1000);

	MDT_AUDIO_Play();
}


void PlayStarWarsTheme() {

	MDT_AUDIO_AddNote( G4, 500);
	MDT_AUDIO_AddNote( D5, 500);
	MDT_AUDIO_AddNote( 0, 10);//DELAY
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( A4, 125);
	MDT_AUDIO_AddNote( G5, 500);

	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( 0, 10);//DELAY
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( A4, 125);
	MDT_AUDIO_AddNote( G5, 500);

	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( 0, 10);//DELAY
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( A4, 500);

	MDT_AUDIO_Play();
}


void PlayMonkeyIslandTheme() {

	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( E5, 750);
	MDT_AUDIO_AddNote( D5, 250);

	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( D5, 125);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( D5, 125);
	MDT_AUDIO_AddNote( C5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( C5, 250);
	MDT_AUDIO_AddNote( B4, 500);
	MDT_AUDIO_AddNote( 20, 250);
	MDT_AUDIO_AddNote( E5, 250);

	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( E5, 750);
	MDT_AUDIO_AddNote( 0, 125);
	MDT_AUDIO_AddNote( Fs5, 125);

	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( A5, 500);
	MDT_AUDIO_AddNote( Fs5, 375);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);

	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( Fs5, 500);
	MDT_AUDIO_AddNote( E5, 375);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);

	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( Fs5, 500);
	MDT_AUDIO_AddNote( E5, 375);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 250);

	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 500);
	MDT_AUDIO_AddNote( 0, 125);//DELAY
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( D5, 125);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( D5, 125);

	MDT_AUDIO_AddNote( C5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( C5, 250);
	MDT_AUDIO_AddNote( B4, 500);
	MDT_AUDIO_AddNote( 0, 1000);//DELAY

	MDT_AUDIO_AddNote( C6, 250);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( G5, 500);
	MDT_AUDIO_AddNote( C6, 125);
	MDT_AUDIO_AddNote( B5, 125);
	MDT_AUDIO_AddNote( A5, 125);
	MDT_AUDIO_AddNote( C6, 125);

	MDT_AUDIO_AddNote( B5, 250);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( G5, 500);
	MDT_AUDIO_AddNote( 0, 250);//DELAY
	MDT_AUDIO_AddNote( G5, 250);

	MDT_AUDIO_AddNote( A5, 250);
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( 0, 125);//DELAY
	MDT_AUDIO_AddNote( A5, 125);
	MDT_AUDIO_AddNote( B5, 125);
	MDT_AUDIO_AddNote( A5, 125);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( A5, 125);

	MDT_AUDIO_AddNote( B5, 250);
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( G5, 250);
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 500);

	//MDT_AUDIO_AddNote( 20, 1000);//DELAY
	/*
	MDT_AUDIO_AddNote( F4, 250);
	MDT_AUDIO_AddNote( G4, 250);
	MDT_AUDIO_AddNote( A4, 2000);
	MDT_AUDIO_AddNote( G4, 1000);
	MDT_AUDIO_AddNote( 20000, 20);//DELAY
	MDT_AUDIO_AddNote( G4, 750);
	MDT_AUDIO_AddNote( F4, 250);
	MDT_AUDIO_AddNote( G4, 1000);
	MDT_AUDIO_AddNote( C5, 1000);
	MDT_AUDIO_AddNote( D5, 2000);

	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 20000, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 20000, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 500);
	*/
	MDT_AUDIO_AddNote( 0, 250);//DELAY
	MDT_AUDIO_AddNote( D5, 250);
	MDT_AUDIO_AddNote( C5, 125);
	MDT_AUDIO_AddNote( B4, 125);
	MDT_AUDIO_AddNote( C5, 250);

	MDT_AUDIO_AddNote( B4, 500);
	MDT_AUDIO_AddNote( 0, 250);//DELAY
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( D5, 250);

	MDT_AUDIO_AddNote( E5, 250);
	MDT_AUDIO_AddNote( 0, 20);//DELAY
	MDT_AUDIO_AddNote( E5, 500);
	MDT_AUDIO_AddNote( 20, 125);//DELAY
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( A5, 125);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( Fs5, 125);
	MDT_AUDIO_AddNote( A5, 125);
	MDT_AUDIO_AddNote( G5, 125);
	MDT_AUDIO_AddNote( E5, 125);
	MDT_AUDIO_AddNote( Fs5, 250);
	MDT_AUDIO_AddNote( E5, 500);

	MDT_AUDIO_Play();
}


void PlaySuperMarioTheme() {

	int i;
	MDT_AUDIO_AddNote(E5,125);
	MDT_AUDIO_AddNote( 0, 10);//DELAY
	MDT_AUDIO_AddNote(E5,250);
	MDT_AUDIO_AddNote( 0, 10);//DELAY
	MDT_AUDIO_AddNote(E5,125);
	MDT_AUDIO_AddNote( 0, 100);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(E5,250);
	MDT_AUDIO_AddNote(G5,600);
	MDT_AUDIO_AddNote(G4,600);

	for (i = 0; i < 2; i++) {

		MDT_AUDIO_AddNote(C5,250);
		MDT_AUDIO_AddNote( 0, 100);//DELAY
		MDT_AUDIO_AddNote(G4,125);
		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote(E4,250);
		MDT_AUDIO_AddNote( 0, 125);//DELAY
		MDT_AUDIO_AddNote(A4,125);
		MDT_AUDIO_AddNote( 0, 125);//DELAY
		MDT_AUDIO_AddNote(B4,125);
		MDT_AUDIO_AddNote( 0, 125);//DELAY
		MDT_AUDIO_AddNote(Bb4,125);
		MDT_AUDIO_AddNote(A4,250);

		MDT_AUDIO_AddNote(G4,250);
		MDT_AUDIO_AddNote(E5,250);
		MDT_AUDIO_AddNote(G5,250);
		MDT_AUDIO_AddNote(A5,250);
		MDT_AUDIO_AddNote(F5,125);
		MDT_AUDIO_AddNote(G5,125);
		MDT_AUDIO_AddNote( 0, 125);//DELAY
		MDT_AUDIO_AddNote(E5,125);
		MDT_AUDIO_AddNote( 0, 125);//DELAY
		MDT_AUDIO_AddNote(C5,125);
		MDT_AUDIO_AddNote(D5,125);
		MDT_AUDIO_AddNote(B4,125);
		MDT_AUDIO_AddNote( 0, 250);//DELAY
	}

	for (i = 0; i < 2; i++) {

		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote(G5,125);
		MDT_AUDIO_AddNote(Gb5,125);
		MDT_AUDIO_AddNote(F5,125);
		MDT_AUDIO_AddNote(Ds5,250);
		MDT_AUDIO_AddNote(E5,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(G4,125);
		MDT_AUDIO_AddNote(A4,125);
		MDT_AUDIO_AddNote(C5,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(A4,125);
		MDT_AUDIO_AddNote(C5,125);
		MDT_AUDIO_AddNote(Ds5,125);

		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote(G5,125);
		MDT_AUDIO_AddNote(Gb5,125);
		MDT_AUDIO_AddNote(F5,125);
		MDT_AUDIO_AddNote(Ds5,250);
		MDT_AUDIO_AddNote(E5,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(C6,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(C6,125);
		MDT_AUDIO_AddNote(0,10);//DELAY
		MDT_AUDIO_AddNote(C6,375);

		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote(G5,125);
		MDT_AUDIO_AddNote(Gb5,125);
		MDT_AUDIO_AddNote(F5,125);
		MDT_AUDIO_AddNote(Ds5,250);
		MDT_AUDIO_AddNote(E5,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(G4,125);
		MDT_AUDIO_AddNote(A4,125);
		MDT_AUDIO_AddNote(C5,125);
		MDT_AUDIO_AddNote(0,125);//DELAY
		MDT_AUDIO_AddNote(A4,125);
		MDT_AUDIO_AddNote(C5,125);
		MDT_AUDIO_AddNote(Ds5,125);

		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote(Eb5,250);
		MDT_AUDIO_AddNote( 0,125);//DELAY
		MDT_AUDIO_AddNote(D5,125);
		MDT_AUDIO_AddNote( 0, 250);//DELAY
		MDT_AUDIO_AddNote( C5, 500);
		MDT_AUDIO_AddNote( 0, 500);//DELAY
	}

	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,250);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,125);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(D5,250);
	MDT_AUDIO_AddNote(E5,125);
	MDT_AUDIO_AddNote(C5,250);
	MDT_AUDIO_AddNote(A4,125);
	MDT_AUDIO_AddNote(G4,500);

	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,250);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,125);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(D5,125);
	MDT_AUDIO_AddNote(E5,125);

	MDT_AUDIO_AddNote(0,600);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,250);
	MDT_AUDIO_AddNote(0,10);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(0,125);//DELAY
	MDT_AUDIO_AddNote(C5,125);
	MDT_AUDIO_AddNote(D5,250);
	MDT_AUDIO_AddNote(E5,125);
	MDT_AUDIO_AddNote(C5,250);
	MDT_AUDIO_AddNote(A4,125);
	MDT_AUDIO_AddNote(G4,500);

	MDT_AUDIO_Play();
}

