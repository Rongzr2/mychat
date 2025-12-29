#pragma once
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <memory>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "boost/filesystem.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include <queue>
#include <mutex>
#include <atomic>
#include "ConfigMgr.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
	Sussess = 0,
	Error_Json = 1001,  // json解析错误
	RPCFailed = 1002    // RPC请求错误
};