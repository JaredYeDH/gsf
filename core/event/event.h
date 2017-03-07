//
// Created by pojol on 2017/2/13.
//

#ifndef _GSF_EVENT_HEADER_
#define _GSF_EVENT_HEADER_

#include "../module/module.h"
#include "event_handler.h"

#include <functional>
#include <tuple>
#include <list>
#include <vector>
#include <unordered_map>

#include <args/args.h>

#include "../../common/single.h"

namespace gsf
{
	typedef std::function<void(gsf::Args, EventHandlerPtr)> EventFunc;

	class Door
	{
	public:
		Door();

		uint32_t get_door_id() const { return door_id_; }

		virtual void listen(uint32_t door, EventFunc func);

		virtual void listen_callback(uint32_t sub_event, std::function<void(gsf::Args)> func);

		virtual void dispatch(uint32_t door, gsf::Args, EventHandlerPtr callback = nullptr);

		virtual void dispatch(uint32_t door, uint32_t sub_event, gsf::Args args);
    protected:
		uint32_t door_id_;
	};

	// 如果需要监听多个同步事件,辅助类
	struct AllSuccess
	{
		void listen(gsf::Door *door, std::vector<uint32_t> vec, std::function<void()> func)
		{
			count_ = vec.size();

			for (auto itr = vec.begin(); itr != vec.end(); ++itr)
			{
				door->listen_callback(*itr, [&](gsf::Args args) {
					count_--;

					if (count_ == 0) {
						func();
					}
				});
			}

		}

	private:
		uint32_t count_;
	};

	//AllSuccess as;
	//as.listen(this, {event_id::timer::make_timer_success}, [&](){
	//	std::cout << "success !" << std::endl;
	//});

	struct AnyoneFail
	{
		void listen(gsf::Door * door, std::vector<uint32_t> vec, std::function<void()> func)
		{
			for (auto itr = vec.begin(); itr != vec.end(); ++itr)
			{
				door->listen_callback(*itr, [&](gsf::Args args) {
					func();
					return;
				});
			}
		}
	};

	class EventModule
			: public gsf::utils::Singleton<EventModule>
			, public Module
	{
		friend class Door;

	public:
		EventModule();

	protected:
		void execute();

		void add_event(uint32_t event, EventFunc func);

		void add_event(uint32_t event, std::function<void(gsf::Args)> func);

		void add_cmd(uint32_t door, gsf::Args args, EventHandlerPtr callback = nullptr);

		void add_cmd(uint32_t door, uint32_t sub_event, gsf::Args args);

		uint32_t make_door_id();

    private:
		std::unordered_map<uint32_t, EventFunc> map_;

		std::list<std::tuple<uint32_t, gsf::Args, EventHandlerPtr>> list_;

		std::unordered_map<uint32_t, std::function<void(gsf::Args)>> callback_map_;

		std::list<std::tuple<uint32_t, gsf::Args>> callback_list_;

		uint32_t door_id_;
	};
}

#endif