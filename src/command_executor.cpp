#include <ros_ffmpeg_controller/command_executor.h>

ExecutionResult CommandExecutor::execute(std::vector<std::string> args, boost::asio::io_service& io_service){
    using namespace boost::interprocess;
    std::future<std::string> data;
    std::future<std::string> error;
    std::string shared_memory_name = std::to_string(rand());
    io_service.notify_fork(io_service.fork_prepare);

    named_mutex mutex(open_or_create, shared_memory_name.c_str());

    shared_memory_object shm_obj(open_or_create, shared_memory_name.c_str(), read_write);
    shm_obj.truncate(sizeof(int));
    mapped_region regionParent(shm_obj, read_only);
    mapped_region regionChild(shm_obj, read_write);

    if (fork()==0) // child proc
    {
        bp::child c = bp::child(args, 
            bp::std_in.close(),
            bp::std_out > data, 
            bp::std_err > error,
            io_service);
        int *pid = static_cast<int*>(regionChild.get_address());
        *pid = c.id();
        io_service.notify_fork(io_service.fork_child);
        io_service.run(); 
        exit(0);
    }
    else // parent proc
    {
        io_service.notify_fork(io_service.fork_parent);
        int *pid = static_cast<int*>(regionParent.get_address());

        bip::shared_memory_object::remove(shared_memory_name.c_str());
        return std::make_tuple<>(*pid, data.share(), error.share());
    }
    
}