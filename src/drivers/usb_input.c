
#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>

#include "mdt/clkinfo.h"
#include "mdt/drivers/usb_input.h"

#ifdef USB_SUPPORT_ENABLED

#include "usbh_core.h"
#include "usbh_hid.h"

static MDT_EVENT_QUEUE* ev_queue = NULL;

static USBH_HandleTypeDef hUsbHost;

static HID_KEYBD_Info_TypeDef *kbdInfo = NULL;

static HID_MOUSE_Info_TypeDef *mouseInfo = NULL;

static char curr_key = 0;

static uint16_t event_count;

static USBH_StatusTypeDef last_code;

static char usb_state[64] = "UNINITIALIZED";


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

static void MDT_USB_INPUT_InitBackgroundProcess();

static void MDT_USB_INPUT_EVENTS_PushEvent(MDT_EVENT_QUEUE* queue, MDT_EVENT* evt);


void MDT_USB_INPUT_Init() {

	if (USBH_Init(&hUsbHost, USBH_UserProcess, HOST_HS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHost, USBH_HID_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_Start(&hUsbHost) != USBH_OK) {
		Error_Handler();
	}

	MDT_USB_INPUT_InitBackgroundProcess();

}


static void MDT_USB_INPUT_InitBackgroundProcess() {
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	LL_TIM_InitTypeDef hTim4 = {
		.Prescaler = 1000,
		.CounterMode = LL_TIM_COUNTERMODE_UP,
		.Autoreload = 1 * MDT_GetAPB1TimersMHz(), //1ms
		.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
		.RepetitionCounter = 0,
	};
	LL_TIM_Init(TIM4, &hTim4);
	LL_TIM_EnableARRPreload(TIM4);

	/**
	 * Setup global interrupt please note his interrupt must have a lower 
	 * priority than systick global interrupt
	 */
	LL_TIM_EnableIT_UPDATE(TIM4);
	uint32_t priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 15);
	NVIC_SetPriority(TIM4_IRQn, priority);
	NVIC_EnableIRQ(TIM4_IRQn);

	LL_TIM_EnableCounter(TIM4);

}


/**
 * Rutina de interrupción del timer 4 (USBH_Process)
 */
void TIM4_IRQHandler() {

	LL_TIM_ClearFlag_UPDATE(TIM4);
	
    last_code = USBH_Process(&hUsbHost);

}


const char* MDT_USB_INPUT_GetState() {
	
	return usb_state;

}


MDT_USB_INPUT_KeybdInfo* MDT_USB_INPUT_GetKeybdInfo() {
	
	return (MDT_USB_INPUT_KeybdInfo*) kbdInfo;

}


MDT_USB_INPUT_MouseInfo* MDT_USB_INPUT_GetMouseInfo() {

	return (MDT_USB_INPUT_MouseInfo*) mouseInfo;

}


USBH_HandleTypeDef* MDT_USB_INPUT_GetHandle() {
	
	return &hUsbHost;

}


uint16_t MDT_USB_INPUT_GetEventCount() {
	
	return event_count;

}


USBH_StatusTypeDef MDT_USB_INPUT_GetLastCode() {
	
	return last_code;

}


bool MDT_USB_INPUT_IsKbdKeyPressed(uint8_t key) {
	
	if (kbdInfo == NULL) {
		return false;

	}

	for (uint8_t i = 0; i < 6; i++) {
		if (kbdInfo->keys[i] == key) {
			return true;
		}
	}

	return false;

}


static void USB_INPUT_CreateKbdEvent(HID_KEYBD_Info_TypeDef* prevInfo, HID_KEYBD_Info_TypeDef* currInfo, MDT_EVENT* destEvt) {

	uint8_t i = 0;
	uint8_t j = 0;

	MDT_EVENT evt = {
		.keyboard = {
			.keycode = 0,
			.modifiers = 0,
			.type = 0,
			.repeat = false,
			.unichar = 0,
			.timestamp = 0,
		}
	};

	if (!prevInfo->lctrl && currInfo->lctrl) {

		evt.keyboard.keycode = MDT_KEY_LEFTCONTROL;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->lctrl && !currInfo->lctrl) {

		evt.keyboard.keycode = MDT_KEY_LEFTCONTROL;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else if (!prevInfo->rctrl && currInfo->rctrl) {

		evt.keyboard.keycode = MDT_KEY_RIGHTCONTROL;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->rctrl && !currInfo->rctrl) {

		evt.keyboard.keycode = MDT_KEY_RIGHTCONTROL;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else if (!prevInfo->lalt && currInfo->lalt) {

		evt.keyboard.keycode = MDT_KEY_LEFTALT;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->lalt && !currInfo->lalt) {

		evt.keyboard.keycode = MDT_KEY_LEFTALT;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else if (!prevInfo->ralt && currInfo->ralt) {

		evt.keyboard.keycode = MDT_KEY_RIGHTALT;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->ralt && !currInfo->ralt) {

		evt.keyboard.keycode = MDT_KEY_RIGHTALT;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else if (!prevInfo->lshift && currInfo->lshift) {

		evt.keyboard.keycode = MDT_KEY_LEFTSHIFT;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->lshift && !currInfo->lshift) {

		evt.keyboard.keycode = MDT_KEY_LEFTSHIFT;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else if (!prevInfo->rshift && currInfo->rshift) {

		evt.keyboard.keycode = MDT_KEY_RIGHTSHIFT;
		evt.keyboard.type = MDT_EVENT_KEY_DOWN;

	} else if (prevInfo->rshift && !currInfo->rshift) {

		evt.keyboard.keycode = MDT_KEY_RIGHTSHIFT;
		evt.keyboard.type = MDT_EVENT_KEY_UP;

	} else {
		
		/**
		 * Esta implementacion supone que solo puede haber un cambio por cada
		 * evento recibido en USBH_HID_EventCallback, pero está implementado de
		 * manera que podria detectar varios cambios si fuera necesario, se han 
		 * insertado breaks ya que no es posible devolver mas de un evento.
		 */
		while (i < 6 && j < 6) {
			if (prevInfo->keys[i] == currInfo->keys[j]) {
				i++;
				j++;
			} else if (prevInfo->keys[i] == 0) {
				evt.keyboard.keycode = currInfo->keys[j];
				evt.keyboard.type = MDT_EVENT_KEY_DOWN;
				i++;
				j++;
				break;
			} else {
				evt.keyboard.keycode = prevInfo->keys[i];
				evt.keyboard.type = MDT_EVENT_KEY_UP;
				i++;
				break;
			}
		}
	
	}

	*destEvt = evt;

}


void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {

	HID_TypeTypeDef device_type = USBH_HID_GetDeviceType(phost);
	
	static HID_KEYBD_Info_TypeDef prevKbdInfo;

	switch (device_type) {

	case HID_MOUSE:

		mouseInfo = USBH_HID_GetMouseInfo(phost);
		break;
	
	case HID_KEYBOARD:

		kbdInfo = USBH_HID_GetKeybdInfo(phost);
		if (kbdInfo->keys[0] == KEY_ERRORROLLOVER) {
			//kbdInfo = &prevKbdInfo;
			//discard this event!!
			return;
		}
		curr_key = USBH_HID_GetASCIICode(kbdInfo);

		if (ev_queue != NULL) {
			MDT_EVENT evt;
			USB_INPUT_CreateKbdEvent(&prevKbdInfo, kbdInfo, &evt);
			MDT_USB_INPUT_EVENTS_PushEvent(ev_queue, &evt);
		}

		prevKbdInfo = *kbdInfo;
		break;

	default:

		break;

	}

	event_count++;

}


/**
 * user callback definition
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {

	switch(id) {
	case HOST_USER_SELECT_CONFIGURATION:
		strcpy(usb_state, "HOST_USER_SELECT_CONFIGURATION");
		break;

	case HOST_USER_DISCONNECTION:
		strcpy(usb_state, "HOST_USER_DISCONNECTION");
		break;

	case HOST_USER_CLASS_ACTIVE:
		strcpy(usb_state, "HOST_USER_CLASS_ACTIVE");
		break;

	case HOST_USER_CONNECTION:
		strcpy(usb_state, "HOST_USER_CONNECTION");
		break;

	default:
		break;
	}
  
}

static void MDT_USB_INPUT_EVENTS_PushEvent(MDT_EVENT_QUEUE* queue, MDT_EVENT* evt) {

	if (queue->count >= 20) {
		//ERROR
		return;
	}

	queue->events[queue->count] = *evt;
	queue->count++;

}

void MDT_USB_INPUT_EVENTS_SetQueue(MDT_EVENT_QUEUE* queue) {

	ev_queue = queue;

}

void MDT_USB_INPUT_EVENTS_UnsetQueue() {
	
	ev_queue = NULL;

}

#endif
