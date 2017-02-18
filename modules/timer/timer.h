#ifndef _TIMER_HEADER_
#define _TIMER_HEADER_

#include <stdint.h>
#include <memory>
#include <map>

#include <chrono>
#include <ctime>
#include <tuple>

#include <module/module.h>
#include <stream/ostream.h>
#include <stream/istream.h>
#include <event/event.h>


namespace gsf
{
	namespace modules
	{
		struct TimerEvent
		{
			TimerEvent(uint32_t id);

            gsf::core::EventHandlerPtr timer_handler_ptr_;
			std::chrono::system_clock::time_point tp_;
			uint32_t timerid_;

			uint32_t get_id() const { return timerid_; }

			bool operator > (const TimerEvent & t)
			{
				return tp_ > t.tp_;
			}
		};

		typedef std::shared_ptr<TimerEvent> TimerEventPtr;

		class Timer
                : public gsf::core::Module
                , public gsf::core::Door
		{
		public:
			
			~Timer();
			Timer();

		protected:

            void init();

            void execute();

		private:

			void delay_milliseconds(gsf::stream::OStream args, gsf::core::EventHandlerPtr callback);
			void delay_day(gsf::stream::OStream args, gsf::core::EventHandlerPtr callback);
			//void delay_week(std::tuple<gsf::utils::Any> args, gsf::core::EventHandlerPtr callback);
			//void delay_month(std::tuple<gsf::utils::Any> args, gsf::core::EventHandlerPtr callback);

            void remove_timer(gsf::stream::OStream args, gsf::core::EventHandlerPtr callback);

			uint32_t make_timer_id();

		private:

			std::map<uint32_t, TimerEventPtr> map_;
			uint32_t timer_id_;
		};


	}
}

#endif