#ifndef _GSF_LUA_SCRIPT_HEADER_
#define _GSF_LUA_SCRIPT_HEADER_

#include <sol.hpp>

#include <module/module.h>
#include <event/event.h>

#include <vector>
#include <string>

namespace gsf
{
	namespace modules
	{
		// sol ��state�Ǹ�unique_ptr�� �������е�����������дʵ�ֹ��ܡ�
		struct LuaState
		{
			sol::state state_;
		};

		class LuaScriptModule
			: public gsf::Module
			, public gsf::IEvent
		{
			void init();

			void execute();

			void shut();

		private:
			//! ��ʵ�֣���Ҫһ����Ч�ķ�ʽ��װ�����ýű�ֱ�ӵ���dispatch
			void ldispatch(sol::variadic_args args);

			//! ����һ���µ�lua module
			void create_event(gsf::Args args, gsf::EventHandlerPtr callback);
			void create(uint32_t module_id, std::string path);

			//! ����һ�����е�lua module
			void destroy_event(gsf::Args args, gsf::EventHandlerPtr callback);
			int destroy(uint32_t module_id);

			//! ����һ�����е�lua module
			void reload_event(gsf::Args args, gsf::EventHandlerPtr callback);

		private:
			uint32_t log_module_;

			typedef std::vector<std::tuple<uint32_t, LuaState*, std::string>> StateMap;
			StateMap lua_map_;
		};
	}
}

#endif
