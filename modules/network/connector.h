﻿#ifndef _GSF_CONNECTOR_HEADER_
#define _GSF_CONNECTOR_HEADER_

#include <vector>
#include <memory>
#include <functional>

#include <core/module.h>
#include <core/event.h>

#include <event2/util.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>

namespace gsf
{
	namespace network
	{
		class Session;
		class MsgBinder;
		typedef std::shared_ptr<Session> SessionPtr;

		class ConnectorModule
			: public gsf::Module
			, public gsf::IEvent
		{
		public:
			ConnectorModule(const std::string &name);
			ConnectorModule();
			~ConnectorModule();

			virtual void before_init();
			virtual void init();

			virtual void execute();

			virtual void shut();
			virtual void after_shut();

		private:
			void make_connector(gsf::Args args, gsf::CallbackFunc callback);
			void bind_remote(gsf::Args args, gsf::CallbackFunc callback);
			
			void need_close_session(int fd);

			void send_msg(uint32_t fd, uint32_t msg_id, std::string block);

		private:

			uint32_t module_id_;

			uint32_t log_module_;

			event_base *event_base_ptr_;

			SessionPtr session_ptr_;
			MsgBinder *binder_;

			::bufferevent *buffer_event_ptr_;

			std::vector<int> disconnect_vec_;
		};
	}
}


#endif
