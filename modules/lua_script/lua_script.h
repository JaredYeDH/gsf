#ifndef _GSF_LUA_SCRIPT_HEADER_
#define _GSF_LUA_SCRIPT_HEADER_

#include <sol.hpp>
#include <array>

#include <module/module.h>
#include <event/event.h>

#include <vector>
#include <string>

namespace gsf
{
	namespace modules
	{
		// sol ��state�Ǹ�unique_ptr�� �������е�����������дʵ�ֹ��ܡ�
		enum LuaAppState
		{
			BEFORE_INIT = 0,
			INIT,
			EXECUTE,
			SHUT,
			AFTER_SHUT,
		};

		struct LuaProxy
		{
			LuaProxy(uint32_t lua_id)
				: lua_id_(lua_id)
				, app_state_(LuaAppState::BEFORE_INIT)
			{}

			sol::state state_;
			std::string path_;
			LuaAppState app_state_;
			uint32_t lua_id_;

			std::array<std::function<void(sol::table)>, 5> call_list_;
		};

		class LuaScriptModule
			: public gsf::Module
			, public gsf::IEvent
		{
		public:
			LuaScriptModule()
				: Module("LuaScriptModule")
			{}

			void init();

			void execute();

			void shut();

		private:
			//! ��ʵ�֣���Ҫһ����Ч�ķ�ʽ��װ�����ýű�ֱ�ӵ���dispatch
			//sol::variadic_args args
			void ldispatch(uint32_t target, uint32_t event, gsf::Args args, gsf::CallbackFunc callback = nullptr);
			void ldispatch_remote(uint32_t target, uint32_t fd, uint32_t msg_id, const std::string &str);
			void llisten(uint32_t self, uint32_t event, sol::function func);

			//! ����һ���µ�lua module
			void create_event(gsf::Args args, gsf::CallbackFunc callback);
			void create(uint32_t module_id, std::string path);

			//! ����һ�����е�lua module
			void destroy_event(gsf::Args args, gsf::CallbackFunc callback);
			int destroy(uint32_t module_id);

			//! ����һ�����е�lua module
			void reload_event(gsf::Args args, gsf::CallbackFunc callback);

		private:
			uint32_t log_module_;

			typedef std::vector<std::pair<uint32_t, LuaProxy*>> StateMap;
			StateMap lua_map_;
		};
	}
}

#endif
