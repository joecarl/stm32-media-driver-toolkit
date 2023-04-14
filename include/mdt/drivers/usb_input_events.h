#ifndef MDT_USB_INPUT_EVENTS_H
#define MDT_USB_INPUT_EVENTS_H

/**
 * This header defines the input event queue handling, it's strongly based on ALLEGRO_EVENT
 */

#include <stdbool.h>
#include <stdint.h>

/* Type: MDT_EVENT_TYPE
 */
typedef unsigned int MDT_EVENT_TYPE;

enum
{
	MDT_EVENT_JOYSTICK_AXIS               =  1,
	MDT_EVENT_JOYSTICK_BUTTON_DOWN        =  2,
	MDT_EVENT_JOYSTICK_BUTTON_UP          =  3,
	MDT_EVENT_JOYSTICK_CONFIGURATION      =  4,

	MDT_EVENT_KEY_DOWN                    = 10,
	MDT_EVENT_KEY_CHAR                    = 11,
	MDT_EVENT_KEY_UP                      = 12,

	MDT_EVENT_MOUSE_AXES                  = 20,
	MDT_EVENT_MOUSE_BUTTON_DOWN           = 21,
	MDT_EVENT_MOUSE_BUTTON_UP             = 22,
	MDT_EVENT_MOUSE_ENTER_DISPLAY         = 23,
	MDT_EVENT_MOUSE_LEAVE_DISPLAY         = 24,
	MDT_EVENT_MOUSE_WARPED                = 25,

	MDT_EVENT_TOUCH_BEGIN                 = 50,
	MDT_EVENT_TOUCH_END                   = 51,
	MDT_EVENT_TOUCH_MOVE                  = 52,
	MDT_EVENT_TOUCH_CANCEL                = 53,
	
};


/*
 * Event structures
 *
 * All event types have the following fields in common.
 *
 *  type      -- the type of event this is
 *  timestamp -- when this event was generated
 *  source    -- which event source generated this event
 *
 * For people writing event sources: The common fields must be at the
 * very start of each event structure, i.e. put _MDT_EVENT_HEADER at the
 * front.
 */

#define _MDT_EVENT_HEADER()                    \
	MDT_EVENT_TYPE type;                              \
	double timestamp;


typedef struct MDT_ANY_EVENT
{
	_MDT_EVENT_HEADER()
} MDT_ANY_EVENT;

/*
typedef struct MDT_JOYSTICK_EVENT
{
	_MDT_EVENT_HEADER()
	struct MDT_JOYSTICK *id;
	int stick;
	int axis;
	float pos;
	int button;
} MDT_JOYSTICK_EVENT;
*/


typedef struct MDT_KEYBOARD_EVENT
{
	_MDT_EVENT_HEADER()
	//struct MDT_DISPLAY *display; /* the window the key was pressed in */
	int keycode;                 /* the physical key pressed */
	int unichar;                 /* unicode character or negative */
	unsigned int modifiers;      /* bitfield */
	bool repeat;                 /* auto-repeated or not */
} MDT_KEYBOARD_EVENT;



typedef struct MDT_MOUSE_EVENT
{
	_MDT_EVENT_HEADER()
	//struct MDT_DISPLAY *display;
	/* (display) Window the event originate from
	 * (x, y) Primary mouse position
	 * (z) Mouse wheel position (1D 'wheel'), or,
	 * (w, z) Mouse wheel position (2D 'ball')
	 * (pressure) The pressure applied, for stylus (0 or 1 for normal mouse)
	 */
	int x,  y,  z, w;
	int dx, dy, dz, dw;
	unsigned int button;
	float pressure;
} MDT_MOUSE_EVENT;


/* Type: MDT_EVENT
 */
typedef union MDT_EVENT MDT_EVENT;

union MDT_EVENT
{
	/* This must be the same as the first field of _MDT_EVENT_HEADER.  */
	MDT_EVENT_TYPE type;
	/* `any' is to allow the user to access the other fields which are
	 * common to all event types, without using some specific type
	 * structure.
	 */
	MDT_ANY_EVENT      any;
	//MDT_JOYSTICK_EVENT joystick;
	MDT_KEYBOARD_EVENT keyboard;
	MDT_MOUSE_EVENT    mouse;
};



/* Event queues */

/* Type: MDT_EVENT_QUEUE
 */
typedef struct MDT_EVENT_QUEUE MDT_EVENT_QUEUE;

struct MDT_EVENT_QUEUE
{
	MDT_EVENT events[20];
	uint8_t count;
};


bool MDT_USB_INPUT_EVENTS_IsQueueEmpty(MDT_EVENT_QUEUE*);
bool MDT_USB_INPUT_EVENTS_GetNextEvent(MDT_EVENT_QUEUE*, MDT_EVENT *ret_event);
void MDT_USB_INPUT_EVENTS_FlushQueue(MDT_EVENT_QUEUE*);
void MDT_USB_INPUT_EVENTS_WaitForEvent(MDT_EVENT_QUEUE*, MDT_EVENT *ret_event);


#endif
