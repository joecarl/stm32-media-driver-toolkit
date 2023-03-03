#ifndef MDT_TEXT_INPUT_H
#define MDT_TEXT_INPUT_H

#include <stdint.h>


typedef struct {
	
	char value[128];
	
	uint8_t cursor_pos;

} MDT_TEXT_INPUT;


void MDT_TEXT_INPUT_ShiftCursor(MDT_TEXT_INPUT* inp, int shift);


void MDT_TEXT_INPUT_InsertAtPos(MDT_TEXT_INPUT* inp, const char* txt, size_t pos);


void MDT_TEXT_INPUT_Insert(MDT_TEXT_INPUT* inp, const char* txt);


void MDT_TEXT_INPUT_Delete(MDT_TEXT_INPUT* inp, size_t count);


void MDT_TEXT_INPUT_Clear(MDT_TEXT_INPUT* inp);


void MDT_TEXT_INPUT_Init(MDT_TEXT_INPUT* inp);


#endif
