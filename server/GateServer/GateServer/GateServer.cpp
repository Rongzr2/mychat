#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "CServer.h"
#include "ConfigMgr.h"
#include "const.h"

int main()
{
	try {
		auto& gCfgMgr = ConfigMgr::Inst();
		std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
		unsigned short gate_port = atoi(gate_port_str.c_str());
		// 创建io_context对象，参数1表示io_context拥有一个工作线程
		net::io_context ioc{ 1 };

		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

		signals.async_wait([&ioc](const boost::system::error_code error, int signal_number) {
			if (error) {
				return;
			}
			ioc.stop();
	    });

		// 创建对象监听连接
		std::make_shared<CServer>(ioc, gate_port)->Start();
		std::cout << "Gate Server listen on port " << gate_port << std::endl;

		// 调用io_context的run方法，开始处理事件队列中的事件
	    // 这将阻塞当前线程，直到io_context停止
		ioc.run();
	}
	catch (std::exception const& exp) {
		std::cout << "exception is " << exp.what() << std::endl;
		return EXIT_FAILURE;
	}
}