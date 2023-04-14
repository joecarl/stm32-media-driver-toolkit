#include "mdt/drivers/usb_input_events.h"


bool MDT_USB_INPUT_EVENTS_IsQueueEmpty(MDT_EVENT_QUEUE* queue) {
	
	return queue->count == 0;

}

bool MDT_USB_INPUT_EVENTS_GetNextEvent(MDT_EVENT_QUEUE* queue, MDT_EVENT *ret_event) {
	
	if (MDT_USB_INPUT_EVENTS_IsQueueEmpty(queue)) {
		return false;
	}

	*ret_event = queue->events[0];

	for (uint8_t i = 1; i < queue->count; i++) {
		queue->events[i - 1] = queue->events[i];
	}

	queue->count--;

	return true;

}


void MDT_USB_INPUT_EVENTS_FlushQueue(MDT_EVENT_QUEUE* queue) {

	queue->count = 0;

}

void MDT_USB_INPUT_EVENTS_WaitForEvent(MDT_EVENT_QUEUE* queue, MDT_EVENT *ret_event) {

	while (!MDT_USB_INPUT_EVENTS_GetNextEvent(queue, ret_event)) {
		//delay ?
	}

}