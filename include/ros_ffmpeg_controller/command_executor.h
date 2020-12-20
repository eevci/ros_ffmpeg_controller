#pragma once

#include <boost/process.hpp>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace bp = boost::process;


typedef std::tuple<int, std::shared_future<std::string>, std::shared_future<std::string>> ExecutionResult;

class CommandExecutor
{
    
    public:
         ExecutionResult execute(std::vector<std::string> args, boost::asio::io_context& io_context);
};