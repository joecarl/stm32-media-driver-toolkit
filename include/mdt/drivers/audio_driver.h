#ifndef MDT_AUDIO_DRIVER_H
#define MDT_AUDIO_DRIVER_H

#include <stdint.h>

#define C3  130.81
#define Cs3 138.59
#define Db3 138.59
#define D3  146.83
#define Ds3 155.56
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Fs3 185.00
#define Gb3 185.00
#define G3  196.00
#define Gs3 207.65
#define Ab3 207.65
#define A3  220.00
#define As3 233.08
#define Bb3 233.08
#define B3  246.94

#define C4  261.63
#define Cs4 277.18
#define Db4 277.18
#define D4  293.66
#define Ds4 311.13
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Fs4 369.99
#define Gb4 369.99
#define G4  392.00
#define Gs4 415.30
#define Ab4 415.30
#define A4  440.00
#define As4 466.16
#define Bb4 466.16
#define B4  493.88

#define C5  523.25
#define Cs5 554.37
#define Db5 554.37
#define D5  587.33
#define Ds5 622.25
#define Eb5 622.25
#define E5  659.25
#define F5  698.46
#define Fs5 739.99
#define Gb5 739.99
#define G5  783.99
#define Gs5 830.61
#define Ab5 830.61
#define A5  880.00
#define As5 932.33
#define Bb5 932.33
#define B5  987.77

#define C6  1046.50
#define Cs6 1108.73
#define Db6 1108.73
#define D6  1174.66
#define Ds6 1244.51
#define Eb6 1244.51


typedef struct {
	uint16_t time;
	uint16_t frec;
} MDT_NOTE;


/**
 * Inicializa el driver de audio
 */
void MDT_AUDIO_Init();


/**
 * Da comienzo a la reproduccion de la cola de notas introducidas empezando por
 * la primera que se introdujo.
 */
void MDT_AUDIO_Play();


/**
 * Añade una nota a la cola de reproduccion
 * @param frec frecuencia de la nota en Hz. Pueden usarse las constantes
 * 			   definidas en "audio_driver.h"
 * @param time duracion de la nota en milisegundos
 */
void MDT_AUDIO_AddNote(uint16_t frec, uint16_t time);


/**
 * Devuelve TRUE si hay algo reproduciendose, de lo contrario devuelve FALSE. En 
 * realidad devuelve el numero de notas en espera, pero a efectos prácticos es 
 * lo mismo.
 */
uint8_t MDT_AUDIO_IsPlaying();


#endif
