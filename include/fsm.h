//
// Created by Bridger Holt on 11/20/18.
//

#ifndef FSM_FSM_H
#define FSM_FSM_H

#include <functional>

class FsmBase
{
	public:
		template <class Event>
		using StatePtr = void(FsmBase &, Event const &);

		template <class Event>
		using StateFunc = std::function<StatePtr<Event>>;

		constexpr FsmBase() noexcept : FsmBase {nullptr} {}

		explicit constexpr FsmBase(void * funcPtr) : state_ {funcPtr} {}

		template <class Event>
		explicit constexpr FsmBase(StateFunc<Event> state) :
			state_ {stateTarget<Event>(state)} {}


		template <class Event>
		void dispatch(Event const & event)
		{
			static_cast<StatePtr<Event>>(state_)(*this, event);
		}


		void transition(void * funcPtr)
		{
			state_ = funcPtr;
		}


		template <class Event>
		void transition(StateFunc<Event> state)
		{
			state_ = stateTarget<Event>(state);
		}



	private:
		template <class Event>
		static void * stateTarget(StateFunc<Event> & state)
		{
			return state.template target<StatePtr<Event>>();
		}

		/// Function pointer
		void * state_;
};



/// Finite State Machine
/// Could be implemented uses inheritance.
template <class Owner, class Event>
class Fsm
{
	public:
		using StateFunc = std::function<void(Owner &, Event const &)>;

		constexpr Fsm() noexcept = default;

		explicit constexpr Fsm(StateFunc state) noexcept :
			state_ {std::move(state)} { }


		void dispatch(Owner & owner, Event const & event)
		{
			state_(owner, event);
		}


		void transition(StateFunc target) noexcept
		{
			state_ = std::move(target);
		}



	private:
		// The current state method
		StateFunc state_;
};


#endif //FSM_FSM_H
