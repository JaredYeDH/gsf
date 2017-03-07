﻿#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <sys/types.h>


#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif // WIN32

#include <module/application.h>
#include <event/event.h>

#include <network/acceptor.h>
#include <network/connector.h>

#include <iostream>

#include <random>

static int test_tick = -1;

class TestNetworkApp : public gsf::Application
{
public:
	void tick()
	{
		int t = (test_tick + 1) % 50;

		if (t == 0){
			//printf("delay %d\n", delay_);
		}

		test_tick = t;
	}

};

class TestNetworkModule
	: public gsf::Module
	, public gsf::Door
{
public:

	void before_init()
	{
		acceptor_ = new gsf::network::AcceptorModule();
		acceptor_->before_init();
	}

	void init()
	{
		acceptor_->init();

		listen_callback(event_id::network::new_connect, [=](gsf::Args args){
			uint32_t _fd = args.pop_uint32(0);
			std::cout << "new connect " << _fd << std::endl;
		});

		listen_callback(event_id::network::dis_connect, [=](gsf::Args args){
		});

		// 
		gsf::Args _a_args;
		_a_args << get_door_id() << std::string("127.0.0.1") << uint32_t(8001);

		dispatch(event_id::network::make_acceptor, _a_args);
	}

	void execute()
	{
		acceptor_->execute();
	}

private:
	//! 这里再考虑下该怎么实现
	gsf::network::AcceptorModule * acceptor_;
};

int main()
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(1, 1);
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (result != 0){
		exit(1);
	}
#endif // WIN32


	TestNetworkApp app;
	new gsf::EventModule;

	app.regist_module(gsf::EventModule::get_ptr());
	app.regist_module(new TestNetworkModule);

	app.run();

	return 0;
}
