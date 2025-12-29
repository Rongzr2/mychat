#include "CServer.h"
#include "AsioIOServicePool.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):
	_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
}

void CServer::Start()
{
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);

	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			if (ec) {
				std::cerr << "Accept error " << ec.message() << std::endl;
				self->Start();
				return;
			}

			// 没问题就创建一个新的HttpConnection来管理这个新连接
			/*std::make_shared<HttpConnection>(std::move(self->_socket))->Start();*/
			new_con->Start();

			// 递归调用 Start 方法以继续监听新的连接，用于不断接受新的连接
			self->Start();
		}
		// 如果在处理新连接时发生异常，打印异常信息并重新调用 Start 方法以继续监听新连接
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}

	});
}
