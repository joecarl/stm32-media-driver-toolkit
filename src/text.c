/**
 * ARCHIVO: "text.c"
 * NOMBRE:  Funciones de escritura
 * AUTOR:   José Carlos Hurtado
 *
 *		Este archivo contiene funciones para dibujar texto, con soporte 
 *		adicional para un reducido set de caracteres UTF-8
 *		
 */

#include "mdt/text.h"

#include <math.h>
#include <stdio.h>


void GetTextSize(const char* text, uint32_t* width, uint32_t* height) {
	
	*height = 1;
	*width = 0;

	uint32_t line_width = 0;

	#define NEW_LINE_WIDTH() \
		if (line_width > *width) {\
			*width = line_width;\
		}\
		line_width = 0;

	while (*text != '\0') {

		switch (*text) {

		case 0xC3:
			break;

		case '\r':
			NEW_LINE_WIDTH()
			break;

		case '\n':
			(*height)++;
			NEW_LINE_WIDTH()
			break;

		default:
			line_width++;
		
		}

		text++;
	}

	NEW_LINE_WIDTH()

}


void MDT_GRAPHICS_DrawText(MDT_BITMAP* bmp, const char* text, int x, int y, uint8_t color) {

	int iniX = x;
	int i = 0;

	#define PX(_x_, _y_) MDT_GRAPHICS_PutPixel(bmp, x + (_x_), y + (_y_), color)

	while (text[i] != '\0') {

		switch (text[i]) {

		case 0xC3:

			i++;

			switch (text[i]) {

			case '\0': 
				return; //no deberia, pero por si acaso
			
			case 0xA1://'á'
			
									PX(3, -2);
							PX(2, -1);

					PX(1, 1);PX(2, 1);
				PX(0, 2);			PX(3, 2);
				PX(0, 3);			PX(3, 3);
				PX(0, 4);			PX(3, 4);
					PX(1, 5);PX(2, 5);PX(4, 5);
				break;
			
			case 0xA9://'é'
			
									PX(3, -2);
							PX(2, -1);

						PX(1, 1);PX(2, 1);
				PX(0, 2);				PX(3, 2);
				PX(0, 3);PX(1, 3);PX(2, 3);
				PX(0, 4);
						PX(1, 5);PX(2, 5);
				break;
			
			case 0xAD://'í'
			
								PX(3, -1);
						PX(2, 0);

				PX(1, 2);PX(2, 2);
						PX(2, 3);
						PX(2, 4);
						PX(2, 5);
				break;
			
			case 0xB3://'ó'
			
									PX(3, -2);
							PX(2, -1);

					PX(1, 1);PX(2, 1);
				PX(0, 2);			PX(3, 2);
				PX(0, 3);			PX(3, 3);
				PX(0, 4);			PX(3, 4);
					PX(1, 5);PX(2, 5);
				break;
			
			case 0xBA://'ú'
			
									PX(3, -2);
							PX(2, -1);

				PX(0, 1);					PX(3, 1);
				PX(0, 2);					PX(3, 2);
				PX(0, 3);					PX(3, 3);
				PX(0, 4);					PX(3, 4);
					PX(1, 5);PX(2, 5); PX(4, 5);
				break;
			
			
			case 0xB1://ñ
			
				PX(0, 0);PX(1, 0);PX(2, 0);

				PX(0, 2);PX(1, 2);PX(2, 2);
				PX(0, 3);					PX(3, 3);
				PX(0, 4);					PX(3, 4);
				PX(0, 5);					PX(3, 5);
				break;

			case 0x91://Ñ
			
					PX(1, -1);PX(2, -1);PX(3, -1);

				PX(0, 1);						PX(4, 1);
				PX(0, 2);PX(1, 2);				PX(4, 2);
				PX(0, 3);		PX(2, 3);		PX(4, 3);
				PX(0, 4);				PX(3, 4);PX(4, 4);
				PX(0, 5);						PX(4, 5);
				
				break;
			}
			break;

		case 'a':
		
			    PX(1, 1);PX(2, 1);
			PX(0, 2);			PX(3, 2);
			PX(0, 3);			PX(3, 3);
			PX(0, 4);			PX(3, 4);
				PX(1, 5);PX(2, 5);  PX(4, 5);
			break;

		case 'b':
		
			PX(0, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			break;

		case 'c':
		
				PX(1, 1);PX(2, 1);
			PX(0, 2);
			PX(0, 3);
			PX(0, 4);
				PX(1, 5);PX(2, 5);
			break;

		case 'd':
		
									PX(3, 0);
									PX(3, 1);
				PX(1, 2);PX(2, 2);PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
				PX(1, 5);PX(2, 5);PX(3, 5);
			break;

		case 'e':
		

					PX(1, 1);PX(2, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);PX(1, 3);PX(2, 3);
			PX(0, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'f':
		
					 PX(2, 0);PX(3, 0);
			PX(1, 1);
			PX(1, 2);PX(2, 2);PX(3, 2);
			PX(1, 3);
			PX(1, 4);
			PX(1, 5);
			break;

		case 'g':
		

					PX(1, 1);PX(2, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
				PX(1, 5);PX(2, 5);PX(3, 5);
									PX(3, 6);
					PX(1, 7);PX(2, 7);
			break;

		case 'h':
		
			PX(0, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);				PX(3, 5);
			break;

		case 'i':
		
					PX(2, 0);

			PX(1, 2);PX(2, 2);
					PX(2, 3);
					PX(2, 4);
					PX(2, 5);
			break;

		case 'j':
		
				PX(2, 0);

				PX(2, 2);
				PX(2, 3);
				PX(2, 4);
				PX(2, 5);
			PX(1, 6);
			break;

		case 'k':
		
			PX(1, 0);
			PX(1, 1);		PX(3, 1);
			PX(1, 2);PX(2, 2);
			PX(1, 3);		PX(3, 3);
			PX(1, 4);				PX(4, 4);
			PX(1, 5);				PX(4, 5);
			break;

		case 'l':
		
			PX(1, 0);
			PX(1, 1);
			PX(1, 2);
			PX(1, 3);
			PX(1, 4);
				PX(2, 5);
			break;

		case 'm':
		

			PX(0, 1);PX(1, 1);     PX(3, 1);
			PX(0, 2);		PX(2, 2);		PX(4, 2);
			PX(0, 3);		PX(2, 3);		PX(4, 3);
			PX(0, 4);		PX(2, 4);		PX(4, 4);
			PX(0, 5);		PX(2, 5);		PX(4, 5);
			break;

		case 'n':
		

			PX(0, 1);PX(1, 1);PX(2, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);				PX(3, 5);
			break;

		case 'o':
		
				PX(1, 1);PX(2, 1);
			PX(0, 2);			PX(3, 2);
			PX(0, 3);			PX(3, 3);
			PX(0, 4);			PX(3, 4);
				PX(1, 5);PX(2, 5);
			break;

		case 'p':
		
					PX(1, 1);PX(2, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			PX(0, 6);
			PX(0, 7);
			break;

		case 'q':
		
					PX(1, 1);PX(2, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			  PX(1, 5);PX(2, 5);PX(3, 5);
									PX(3, 6);
									PX(3, 7);
			break;

		case 'r':
		
			PX(0, 1);		PX(2, 1);PX(3, 1);
			PX(0, 2);PX(1, 2);
			PX(0, 3);
			PX(0, 4);
			PX(0, 5);
			break;

		case 's':
		
					PX(1, 1);PX(2, 1); PX(3, 1);
			PX(0, 2);
					PX(1, 3);PX(2, 3);
									PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			break;

		case 't':
		
			PX(1, 0);
			PX(1, 1);PX(2, 1);
			PX(1, 2);
			PX(1, 3);
			PX(1, 4);
					PX(2, 5);PX(3, 5);
			break;

		case 'u':
		
			PX(0, 1);					PX(3, 1);
			PX(0, 2);					PX(3, 2);
			PX(0, 3);					PX(3, 3);
			PX(0, 4);					PX(3, 4);
				PX(1, 5);PX(2, 5);PX(4, 5);
			break;

		case 'v':
		
			PX(0, 1);			PX(3, 1);
			PX(0, 2);			PX(3, 2);
			PX(0, 3);			PX(3, 3);
			PX(0, 4);		PX(2, 4);
					PX(1, 5);
			break;

		case 'w':
		
			PX(0, 1);		PX(2, 1);    	PX(4, 1);
			PX(0, 2);		PX(2, 2);		PX(4, 2);
			PX(0, 3);		PX(2, 3);		PX(4, 3);
			PX(0, 4);		PX(2, 4);		PX(4, 4);
					PX(1, 5);PX(2, 5);PX(3, 5);
			break;

		case 'x':
		
			PX(0, 1);				PX(4, 1);
					PX(1, 2);	PX(3, 2);
						PX(2, 3);
					PX(1, 4);	PX(3, 4);
			PX(0, 5);				PX(4, 5);

			break;

		case 'y':
		
			PX(0, 1);			PX(3, 1);
			PX(0, 2);			PX(3, 2);
			PX(0, 3);			PX(3, 3);
			PX(1, 4);PX(2, 4);PX(3, 4);
								PX(3, 5);
				PX(1, 6);PX(2, 6);
			break;

		case 'z':
		
			PX(0, 1);PX(1, 1);PX(2, 1);PX(3, 1);PX(4, 1);
											PX(3, 2);
									PX(2, 3);
							PX(1, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);PX(4, 5);

			break;

		case '1':
		
						PX(2, 0);
				PX(1, 1);PX(2, 1);
						PX(2, 2);
						PX(2, 3);
						PX(2, 4);
						PX(2, 5);
			break;

		case '2':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
									PX(3, 2);
							PX(2, 3);
					PX(1, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);
			break;

		case '3':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
									PX(2, 2);
								PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case '4':
		
										PX(3, 0);
							PX(2, 1);PX(3, 1);
					PX(1, 2);			PX(3, 2);
			PX(0, 3);PX(1, 3);PX(2, 3);PX(3, 3);
										PX(3, 4);
										PX(3, 5);
			break;

		case '5':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
									PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case '6':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case '7':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);
									PX(3, 1);
									PX(3, 2);
							PX(2, 3);
							PX(2, 4);
							PX(2, 5);
			break;

		case '8':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
					PX(1, 2);PX(2, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case '9':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
				PX(1, 3);PX(2, 3);PX(3, 3);
									PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case '0':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'A':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);PX(1, 3);PX(2, 3);PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);				PX(3, 5);
			break;

		case 'B':
		
			PX(0, 0);PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			break;

		case 'C':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);
			PX(0, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'D':
		
			PX(0, 0);PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			break;

		case 'E':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);
			PX(0, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);
			break;

		case 'F':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);
			PX(0, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);
			PX(0, 3);
			PX(0, 4);
			PX(0, 5);
			break;

		case 'G':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);
			PX(0, 3);			PX(2, 3);PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'H':
		
			PX(0, 0);					PX(3, 0);
			PX(0, 1);					PX(3, 1);
			PX(0, 2);PX(1, 2);PX(2, 2);PX(3, 2);
			PX(0, 3);					PX(3, 3);
			PX(0, 4);					PX(3, 4);
			PX(0, 5);					PX(3, 5);
			break;

		case 'I':
		
			PX(1, 0);PX(2, 0);PX(3, 0);
					PX(2, 1);
					PX(2, 2);
					PX(2, 3);
					PX(2, 4);
			PX(1, 5);PX(2, 5);PX(3, 5);
			break;	

		case 'J':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);
								PX(2, 1);
								PX(2, 2);
								PX(2, 3);
								PX(2, 4);
				PX(0, 5);PX(1, 5);
			break;

		case 'K':
					
			PX(0, 0);				PX(3, 0);
			PX(0, 1);		PX(2, 1);
			PX(0, 2);PX(1, 2);
			PX(0, 3);		PX(2, 3);
			PX(0, 4);				PX(3, 4);
			PX(0, 5);					PX(4, 5);
			break;

		case 'L':
		
			PX(0, 0);
			PX(0, 1);
			PX(0, 2);
			PX(0, 3);
			PX(0, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);
			break;

		case 'M':
		
			PX(0, 0);					PX(4, 0);
			PX(0, 1);PX(1, 1);	PX(3, 1);PX(4, 1);
			PX(0, 2);		PX(2, 2);	PX(4, 2);
			PX(0, 3);					PX(4, 3);
			PX(0, 4);					PX(4, 4);
			PX(0, 5);					PX(4, 5);
			break;

		case 'N':
		
			PX(0, 0);						PX(4, 0);
			PX(0, 1);PX(1, 1);				PX(4, 1);
			PX(0, 2);		PX(2, 2);		PX(4, 2);
			PX(0, 3);				PX(3, 3);PX(4, 3);
			PX(0, 4);						PX(4, 4);
			PX(0, 5);						PX(4, 5);
			break;

		case 'O':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'P':
		
			PX(0, 0);PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);PX(1, 3);PX(2, 3);
			PX(0, 4);
			PX(0, 5);
			break;

		case 'Q':
		
					PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);		PX(2, 4);PX(3, 4);
					PX(1, 5);PX(2, 5);
									PX(3, 6);
			break;

		case 'R':
		
			PX(0, 0);PX(1, 0);PX(2, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);PX(1, 3);PX(2, 3);				
			PX(0, 4);				PX(3, 4);
			PX(0, 5);				PX(3, 5);
			break;
			
		case 'S':
		
					PX(1, 0);PX(2, 0);PX(3, 0);
			PX(0, 1);				
					PX(1, 2);PX(2, 2);
									PX(3, 3);
									PX(3, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);
			break;

		case 'T':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);PX(4, 0);
							PX(2, 1);
							PX(2, 2);
							PX(2, 3);
							PX(2, 4);
							PX(2, 5);
			break;
		
		case 'U':
		
			PX(0, 0);				PX(3, 0);
			PX(0, 1);				PX(3, 1);
			PX(0, 2);				PX(3, 2);
			PX(0, 3);				PX(3, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);
			break;

		case 'V':
		
			PX(0, 0);					PX(3, 0);
			PX(0, 1);					PX(3, 1);
			PX(0, 2);					PX(3, 2);
			PX(0, 3);				PX(2, 3);
			PX(0, 4);		PX(2, 4);
					PX(1, 5);
			break;

		case 'W':
		
			PX(0, 0);						PX(4, 0);
			PX(0, 1);						PX(4, 1);
			PX(0, 2);						PX(4, 2);
			PX(0, 3);						PX(4, 3);
			PX(0, 4);		PX(2, 4);		PX(4, 4);
					PX(1, 5);		PX(3, 5);
			break;

		case 'X':

			PX(0, 0);				PX(4, 0);
					PX(1, 1);	PX(3, 1);
						PX(2, 2);
					PX(1, 3);	PX(3, 3);
			PX(0, 4);				PX(4, 4);
			PX(0, 5);				PX(4, 5);
			break;

		case 'Y':

			PX(0, 0);				PX(4, 0);
			PX(0, 1);				PX(4, 1);
				PX(1, 2);		PX(3, 2);
						PX(2, 3);
						PX(2, 4);
						PX(2, 5);
			break;

		case 'Z':
		
			PX(0, 0);PX(1, 0);PX(2, 0);PX(3, 0);PX(4, 0);
									PX(3, 1);
							PX(2, 2);
					PX(1, 3);
			PX(0, 4);
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);PX(4, 5);

			break;
			
		case '!':
		
			PX(2, 0);
			PX(2, 1);
			PX(2, 2);
			PX(2, 3);

			PX(2, 5);
			break;
		
		case '"':
		
			PX(1, 0);	PX(3, 0);
			PX(1, 1);	PX(3, 1);
			break;

		case '\'':
		
			PX(2, 0);
			PX(2, 1);
			break;
		
		case '$':
							PX(2, -1);
					PX(1, 0);PX(2, 0);PX(3, 0);
			PX(0, 1);
					PX(1, 2);PX(2, 2);
									PX(3, 3);
			PX(0, 4);PX(1, 4);PX(2, 4);
					PX(1, 5);
			break;

		case '%':
		
			PX(0, 1);						;PX(4, 1);
									PX(3, 2);
							PX(2, 3);
					PX(1, 4);
			PX(0, 5);						PX(4, 5);
			break;
		
		case '&':
		
					PX(1, 0);
			PX(0, 1);		PX(2, 1);
					PX(1, 2);
			PX(0, 3);		PX(2, 3);
			PX(0, 4);				PX(3, 4);
					PX(1, 5);PX(2, 5);		PX(4, 5);
			break;

		case '/':
		
											;PX(4, 1);
									PX(3, 2);
							PX(2, 3);
					PX(1, 4);
			PX(0, 5);
			break;
			
		case '[':
		
			PX(1, 0);PX(2, 0);
			PX(1, 1);
			PX(1, 2);
			PX(1, 3);
			PX(1, 4);
			PX(1, 5);PX(2, 5);
			break;

		case ']':
		
			PX(2, 0);PX(3, 0);
					 PX(3, 1);
					 PX(3, 2);
					 PX(3, 3);
					 PX(3, 4);
			PX(2, 5);PX(3, 5);
			break;	

		case '{':
		
							PX(2, -1);
					PX(1, 0);
					PX(1, 1);
			PX(0, 2);
					PX(1, 3);
					PX(1, 4);
							PX(2, 5);
			break;
		
		case '}':
		
			PX(2, -1);
					 PX(3, 0);
					 PX(3, 1);
					 		PX(4, 2);
					 PX(3, 3);
					 PX(3, 4);
			PX(2, 5);
			break;

		case '(':
		
					PX(2, 0);
			PX(1, 1);
			PX(1, 2);
			PX(1, 3);
			PX(1, 4);
					PX(2, 5);
			break;

		case ')':
		
			PX(2, 0);
					 PX(3, 1);
					 PX(3, 2);
					 PX(3, 3);
					 PX(3, 4);
			PX(2, 5);
			break;

		case '=':
		
			PX(0, 2);PX(1, 2);PX(2, 2);PX(3, 2);

			PX(0, 4);PX(1, 4);PX(2, 4);PX(3, 4);
			break;

		case '*':
						PX(2, 0);
			PX(0, 1);	PX(2, 1);	PX(4, 1);
				PX(1, 2);PX(2, 2);PX(3, 2);
				PX(1, 3);PX(2, 3);PX(3, 3);
			PX(0, 4);	PX(2, 4);	PX(4, 4);
						PX(2, 5);
			break;

		case '+':

					PX(2, 2);
			PX(1, 3);PX(2, 3);PX(3, 3);
					PX(2, 4);
			break;

		case '-':
			
			PX(1, 3);PX(2, 3);PX(3, 3);
			break;

		case '?':
		
			PX(1, -1);PX(2, -1);
							PX(3, 0);
							PX(3, 1);
					PX(2, 2);
					PX(2, 3);
					
					PX(2, 5);
			break;
		
		case '_':
		
			PX(0, 5);PX(1, 5);PX(2, 5);PX(3, 5);PX(4, 5);
			break;

		case '>':

			PX(1, 1);
					PX(2, 2);
							PX(3, 3);
					PX(2, 4);
			PX(1, 5);	
			break;

		case '^':

							PX(2, 0);
					PX(1, 1);		PX(3, 1);
			PX(0, 2);						PX(4, 2);
			break;

		case '<':

							PX(2, 1);
					PX(1, 2);
			PX(0, 3);
					PX(1, 4);
							PX(2, 5);
			break;

		case ',':
		
				PX(2, 4);
			PX(1, 5);
			break;

		case ';':
		
				PX(2, 2);

				PX(2, 4);
			PX(1, 5);
			break;

		case '.':
		
			PX(1, 5);
			break;

		case ':':
		
			PX(2, 2);

			
			PX(2, 5);
			break;
		
		case '|':
		
			PX(2, 1);
			PX(2, 2);
			PX(2, 3);
			PX(2, 4);
			PX(2, 5);
			break;
		
		case '@':
						PX(2, -1);PX(3, -1);
				PX(1, 0);					PX(4, 0);
			PX(0, 1);				PX(3, 1);PX(4, 1);
			PX(0, 2);		PX(2, 2);		PX(4, 2);
			PX(0, 3);		PX(2, 3);		PX(4, 3);		
			PX(0, 4);				PX(3, 4);PX(4, 4);		
				PX(1, 5);
						PX(2, 6);PX(3, 6);
			break;

		case '#':	
					PX(1, 0);		PX(3, 0);	
			PX(0, 1);PX(1, 1);PX(2, 1);PX(3, 1);PX(4, 1);
					PX(1, 2);		PX(3, 2);
					PX(1, 3);		PX(3, 3);
			PX(0, 4);PX(1, 4);PX(2, 4);PX(3, 4);PX(4, 4);
					PX(1, 5);		PX(3, 5);
			break;
		
		case '\r':
		
			x = iniX - MDT_TEXT_CHAR_WIDTH;
			break;

		case '\n':
		
			x = iniX - MDT_TEXT_CHAR_WIDTH;
			y += MDT_TEXT_LINE_HEIGHT;
			break;

		}

		x += MDT_TEXT_CHAR_WIDTH;
		i++;
	}

}


void MDT_DrawText(const char* text, int x, int y, uint8_t color) {
	
	MDT_BITMAP ctx_bmp;
	MDT_GRAPHICS_GetBitmapFromContext(&ctx_bmp, &main_ctx);
	MDT_GRAPHICS_DrawText(&ctx_bmp, text, x, y, color);

}