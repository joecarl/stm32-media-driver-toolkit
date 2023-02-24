
#include <string.h>

#include "text_input.h"


void strsplice(char* str, size_t pos, size_t count, const char* insert) {

	const size_t str_len = strlen(str);
	const size_t insert_len = insert != NULL ? strlen(insert) : 0;
	const int shift = insert_len - count;

	if (shift > 0) {

		for (int i = str_len; i >= (int) (pos + count); i--) {
			str[i + shift] = str[i];
		}

	} else if (shift < 0) {

		for (int i = pos + count; i <= str_len; i++) {
			str[i + shift] = str[i];
		}

	}

	if (insert != NULL) {

		for (int i = 0; i < insert_len; i++) {
			str[pos + i] = insert[i];
		}

	}

}


void TEXT_INPUT_InsertAtPos(TEXT_INPUT* inp, const char* txt, size_t pos) {

	strsplice(inp->value, pos, 0, txt);

}


void TEXT_INPUT_Insert(TEXT_INPUT* inp, const char* txt) {

	strsplice(inp->value, inp->cursor_pos, 0, txt);
	inp->cursor_pos += strlen(txt);

}


void TEXT_INPUT_Delete(TEXT_INPUT* inp, size_t count) {

	if (inp->cursor_pos < count) {
		return;
	}

	strsplice(inp->value, inp->cursor_pos - count, count, NULL);
	inp->cursor_pos -= count;

}


void TEXT_INPUT_Clear(TEXT_INPUT* inp) {
	
	inp->value[0] = '\0';
	inp->cursor_pos = 0;
	
}


void TEXT_INPUT_Init(TEXT_INPUT* inp) {

	TEXT_INPUT_Clear(inp);
	
}


void TEXT_INPUT_ShiftCursor(TEXT_INPUT* inp, int shift) {

	int new_pos = inp->cursor_pos + shift;

	if (new_pos < 0) {
		
		inp->cursor_pos = 0;

	} else {
		
		const size_t len = strlen(inp->value);
		
		if (new_pos > len) {
			inp->cursor_pos = len;
		} else {
			inp->cursor_pos = new_pos;
		}

	}
	
}