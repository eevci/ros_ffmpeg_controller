#pragma once

#include <boost/process.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

namespace bp = boost::process;
namespace bip = boost::interprocess;


typedef std::tuple<int, std::shared_future<std::string>, std::shared_future<std::string>> ExecutionResult;

class CommandExecutor
{
    
    public:
         ExecutionResult execute(std::vector<std::string> args, boost::asio::io_service& io_service);
};