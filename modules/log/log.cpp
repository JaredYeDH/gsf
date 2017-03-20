#include "log.h"

#include <string>

#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>


void gsf::modules::LogModule::init()
{
	listen(this, event_id::log::init
		, std::bind(&LogModule::init_impl, this
		, std::placeholders::_1
		, std::placeholders::_2));

	listen(this, event_id::log::info
		, std::bind(&LogModule::log_info, this
		, std::placeholders::_1
		, std::placeholders::_2));

	listen(this, event_id::log::warning
		, std::bind(&LogModule::log_warning, this
		, std::placeholders::_1
		, std::placeholders::_2));

	listen(this, event_id::log::error
		, std::bind(&LogModule::log_error, this
		, std::placeholders::_1
		, std::placeholders::_2));
}

void gsf::modules::LogModule::execute()
{
	while (!log_.empty())
	{
		auto itr = log_.begin();

		switch (itr->first)
		{
		case event_id::log::info:
			LOG(INFO) << itr->second;
			break;
		}
	
		log_.pop_front();
	}
}

void gsf::modules::LogModule::shut()
{
	google::ShutdownGoogleLogging();
}

void gsf::modules::LogModule::init_impl(gsf::Args args, gsf::EventHandlerPtr callback)
{
	std::string _appid = args.pop_string(0);
	std::string _path = args.pop_string(1);

	FLAGS_log_dir			= _path;			//�������·��
	FLAGS_alsologtostderr	= true;  //������־��Ϣ������־�ļ�֮���Ƿ�ȥ��׼���
	FLAGS_colorlogtostderr	= true;  //���ü�¼����׼�������ɫ��Ϣ������ն�֧�֣�
	FLAGS_max_log_size		= 10;  //���������־�ļ���С����MBΪ��λ��

	google::InitGoogleLogging("log");
}

void gsf::modules::LogModule::log_info(gsf::Args args, gsf::EventHandlerPtr callback)
{
	log_.push_back(std::make_pair(event_id::log::info, args.pop_string(0)));
}

void gsf::modules::LogModule::log_warning(gsf::Args args, gsf::EventHandlerPtr callback)
{

}

void gsf::modules::LogModule::log_error(gsf::Args args, gsf::EventHandlerPtr callback)
{

}

