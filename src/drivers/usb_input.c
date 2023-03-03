
#include <stm32f4xx.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_tim.h>

#include "usbh_core.h"
#include "mdt/clkinfo.h"
#include "mdt/drivers/usb_input.h"


static USBH_HandleTypeDef hUsbHost;

static HID_KEYBD_Info_TypeDef *kbdInfo = NULL;

static HID_MOUSE_Info_TypeDef *mouseInfo = NULL;

static char curr_key = 0;

static uint16_t event_count;

static USBH_StatusTypeDef last_code;

static char usb_state[64] = "UNINITIALIZED";


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

static void MDT_USB_INPUT_InitBackgroundProcess();


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


HID_KEYBD_Info_TypeDef* MDT_USB_INPUT_GetKbdInfo() {
	
	return kbdInfo;

}


HID_MOUSE_Info_TypeDef* MDT_USB_INPUT_GetMouseInfo() {

	return mouseInfo;

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


void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {

	HID_TypeTypeDef device_type = USBH_HID_GetDeviceType(phost);

	switch (device_type) {

	case HID_MOUSE:

		mouseInfo = USBH_HID_GetMouseInfo(phost);
		break;
	
	case HID_KEYBOARD:

		//HID_KEYBD_Info_TypeDef *kbdInfo;
		kbdInfo = USBH_HID_GetKeybdInfo(phost);
		curr_key = USBH_HID_GetASCIICode(kbdInfo);
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

