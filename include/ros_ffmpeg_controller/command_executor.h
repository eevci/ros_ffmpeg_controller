#pragma once

#include <boost/process.hpp>

namespace bp = boost::process;



class CommandExecutor
{
    
    public:
        std::pair<int, std::shared_future<std::string>> execute(std::vector<std::string> args);
};