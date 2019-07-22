/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** rc_pcint.cpp
** Global pin change interrupt handler
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Arduino.h>

#include "rc_debug_lib.h"
#include "rc_pcint.h"


#ifdef RC_USE_PCINT

namespace rc {
namespace pcint {

enum
{
	BPins = 7,
	CPins = 6,
	DPins = 8
};

static uint8_t  s_lastB = 0; //!< last read value of PINB
static Callback s_callB[BPins] = { 0 }; //!< Callback functions for port B
static void*    s_userB[BPins] = { 0 }; //!< User data for port B

static uint8_t  s_lastC = 0; //!< last read value of PINC
static Callback s_callC[CPins] = { 0 }; //!< Callback functions for port C
static void*    s_userC[CPins] = { 0 }; //!< User data for port C

static uint8_t  s_lastD = 0; //!< last read value of PIND
static Callback s_callD[DPins] = { 0 }; //!< Callback functions for port D
static void*    s_userD[DPins] = { 0 }; //!< User data for port D


void enable(uint8_t p_pin, Callback p_callback, void* p_user)
{
	pinMode(p_pin, INPUT);
	
	uint8_t bit = digitalPinToPCMSKbit(p_pin);
	uint8_t port = digitalPinToPort(p_pin);
	
	if (port == 2)
	{
		s_callB[bit]  = p_callback;
		s_userB[bit]  = p_user;
		PCMSK0       |= _BV(bit);
#ifdef PCICR
		PCICR        |= _BV(0);
#elif defined(GIMSK) /* e.g. ATtiny XX */
#ifdef PCIE0
       GIMSK |= (1  << (port + PCIE0));
#elif defined(PCIE) /* e.g. ATtiny X5 */
  GIMSK |= (1  << (pcintPort + PCIE));
#else
#error MCU has no PCIE register
#endif
#else
#error MCU has no such a register
#endif

	}
	else if (port == 3)
	{
		s_callC[bit]  = p_callback;
		s_userC[bit]  = p_user;
		PCMSK1       |= _BV(bit);
#ifdef PCICR
		PCICR        |= _BV(1);
#elif defined(GIMSK) /* e.g. ATtiny XX */
#ifdef PCIE0
       GIMSK |= (1  << (port + PCIE0));
#elif defined(PCIE) /* e.g. ATtiny X5 */
  GIMSK |= (1  << (pcintPort + PCIE));
#else
#error MCU has no PCIE register
#endif
#else
#error MCU has no such a register
#endif
	}
	else if (port == 4)
	{
		s_callD[bit]  = p_callback;
		s_userD[bit]  = p_user;
#ifdef PCICR
		PCMSK2       |= _BV(bit);
		PCICR        |= _BV(2);
#elif defined(GIMSK) /* e.g. ATtiny XX */
#ifdef PCIE0
       GIMSK |= (1  << (port + PCIE0));
#elif defined(PCIE) /* e.g. ATtiny X5 */
  GIMSK |= (1  << (pcintPort + PCIE));
#else
#error MCU has no PCIE register
#endif
#else
#error MCU has no such a register
#endif

	}
}


void disable(uint8_t p_pin)
{
	uint8_t bit = digitalPinToPCMSKbit(p_pin);
	uint8_t port = digitalPinToPort(p_pin);
	
	if (port == 2)
	{
		PCMSK0 &= ~_BV(bit);
		if (PCMSK0 == 0)
		{
			#ifdef PCICR
			PCICR &= ~_BV(0);
			#else
			//not implemented
			#endif
		}
		
		s_callB[bit] = 0;
		s_userB[bit] = 0;
	}
	else if (port == 3)
	{
		PCMSK1 &= ~_BV(bit);
		if (PCMSK1 == 0)
		{
			#ifdef PCICR
			PCICR &= ~_BV(1);
			#else
			//not implemented
			#endif
		}
		
		s_callC[bit] = 0;
		s_userC[bit] = 0;
	}
	else if (port == 4)
	{
		#ifdef PCICR
		PCMSK2 &= ~_BV(bit);
		
		if (PCMSK2 == 0)
		{
			PCICR &= ~_BV(2);
		}
		#else
		//not implemented
		#endif
		
		s_callD[bit] = 0;
		s_userD[bit] = 0;
	}
}


// namespace end
}
}

// Pin change 0 (port B) interrupt
ISR(PCINT0_vect)
{
	uint8_t newB = PINB;
	uint8_t change = (newB ^ rc::pcint::s_lastB) & PCMSK0; // only show changed pins with active change interrupt
	rc::pcint::s_lastB = newB;
	
	for (uint8_t i = 0; i < rc::pcint::BPins; ++i)
	{
		if (change & 1 && rc::pcint::s_callB[i] != 0)
		{
			rc::pcint::s_callB[i](i + 8, rc::pcint::s_lastB & _BV(i), rc::pcint::s_userB[i]);
		}
		change >>= 1;
	}
}

// Pin change 1 (port C) interrupt
ISR(PCINT1_vect)
{
	//changed should not work!!
	uint8_t newC = PINA1;
	uint8_t change = (newC ^ rc::pcint::s_lastC) & PCMSK1; // only show changed pins with active change interrupt
	rc::pcint::s_lastC = newC;
	
	for (uint8_t i = 0; i < rc::pcint::CPins; ++i)
	{
		if (change & 1 && rc::pcint::s_callC[i] != 0)
		{
			rc::pcint::s_callC[i](i + 14, rc::pcint::s_lastC & _BV(i), rc::pcint::s_userC[i]);
		}
		change >>= 1;
	}
}

// // Pin change 2 (port D) interrupt
// ISR(PCINT2_vect)
// {
// 	//changed!!! should not work!
// 	uint8_t newD = PINB;
// 	uint8_t change = (newD ^ rc::pcint::s_lastD) & PCMSK1; // only show changed pins with active change interrupt
// 	rc::pcint::s_lastD = newD;
	
// 	for (uint8_t i = 0; i < rc::pcint::DPins; ++i)
// 	{
// 		if ((change & 1) && rc::pcint::s_callD[i] != 0)
// 		{
// 			rc::pcint::s_callD[i](i, rc::pcint::s_lastD & _BV(i), rc::pcint::s_userD[i]);
// 		}
// 		change >>= 1;
// 	}
// }


#endif // RC_USE_PCINT
