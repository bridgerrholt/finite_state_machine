#include <iostream>
#include <functional>
#include <locale>

#include "include/fsm.h"

enum class KeyboardSignal
{
	NONE = 0,
	SHIFT_PRESSED,
	SHIFT_RELEASED,
	ANY_KEY
};


class KeyboardEvent
{
	public:
		KeyboardEvent() : signal {KeyboardSignal::NONE}, code {0} {}

		KeyboardSignal signal;
		char code;

};


class Keyboard
{
	public:
		Keyboard() : fsm_(&Keyboard::defaultEvent) {}

		void dispatch(KeyboardEvent event)
		{
			fsm_.dispatch(*this, event);
		}


	private:
		Fsm<Keyboard, KeyboardEvent> fsm_;

		void defaultEvent(KeyboardEvent const & event)
		{
			switch (event.signal)
			{
				case KeyboardSignal::SHIFT_PRESSED:
					std::cout << "default: SHIFT_PRESSED\n";
					fsm_.transition(&Keyboard::shiftedEvent);
					break;

				case KeyboardSignal::ANY_KEY:
					std::cout << "key " << std::tolower(event.code, {}) << '\n';
					break;

				default:
					break;
			}
		}


		void shiftedEvent(KeyboardEvent const & event)
		{
			switch (event.signal)
			{
				case KeyboardSignal::SHIFT_RELEASED:
					std::cout << "default: SHIFT_RELEASED\n";
					fsm_.transition(&Keyboard::defaultEvent);
					break;

				case KeyboardSignal::ANY_KEY:
					std::cout << "key " << std::toupper(event.code, {}) << '\n';
					break;

				default:
					break;
			}
		}
};


// A basic example of event-driven code using a state machine
int main()
{
	Keyboard keyboard;

	while (true)
	{
		KeyboardEvent ke;

		std::cin >> ke.code;

		bool toQuit {false};

		switch (ke.code)
		{
			case '^':
				ke.signal = KeyboardSignal::SHIFT_PRESSED;
				break;

			case '6':
				ke.signal = KeyboardSignal::SHIFT_RELEASED;
				break;

			case '.':
				toQuit = true;
				break;

			default:
				ke.signal = KeyboardSignal::ANY_KEY;
				break;
		}

		keyboard.dispatch(ke);

		if (toQuit)
			break;
	}

	return 0;
}