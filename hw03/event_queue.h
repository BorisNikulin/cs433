#pragma once

#include "event.h"
#include "types.h"

#include <queue>
#include <vector>
#include <functional>

namespace sim
{
	class EventQueue
	{
		private:
			std::priority_queue<
				Event,
				std::vector<Event>,	
				std::greater<Event>> eventQueue;

		public:
			bool empty() const noexcept;
			Event popEvent();
			void pushEvent(Event e);
			void pushEvents(Events es);

			UnitDuration time() const;
	};
}
