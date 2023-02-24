#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <stdint.h>


typedef struct {
	
	char value[128];
	
	uint8_t cursor_pos;

} TEXT_INPUT;


void TEXT_INPUT_ShiftCursor(TEXT_INPUT* inp, int shift);


void TEXT_INPUT_InsertAtPos(TEXT_INPUT* inp, const char* txt, size_t pos);


void TEXT_INPUT_Insert(TEXT_INPUT* inp, const char* txt);


void TEXT_INPUT_Delete(TEXT_INPUT* inp, size_t count);


void TEXT_INPUT_Clear(TEXT_INPUT* inp);


void TEXT_INPUT_Init(TEXT_INPUT* inp);


#endif
