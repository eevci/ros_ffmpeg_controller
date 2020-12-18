#include <ros_ffmpeg_controller/command_executor.h>

std::pair<int, std::shared_future<std::string>> CommandExecutor::execute(std::vector<std::string> args){
    boost::asio::io_service ios;
    std::future<std::string> data;
    std::future<std::string> error;
    bp::child c(args, 
            bp::std_in.close(),
            bp::std_out > data, 
            bp::std_err > error,
            ios);
    c.detach();
    ios.run(); 
    return std::make_pair<>(c.id(), error.share());
}