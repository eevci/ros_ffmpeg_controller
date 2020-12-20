#include <ros_ffmpeg_controller/command_executor.h>

ExecutionResult CommandExecutor::execute(std::vector<std::string> args, boost::asio::io_context& io_context){
    std::future<std::string> data;
    std::future<std::string> error;
    std::string shared_memory_name = std::to_string(rand());
    io_context.notify_fork(io_context.fork_prepare);

    std::mutex m;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(m);
    int pid = -1;
    std::thread m_workerThread ([args, &pid, &data, &error, &io_context, &lk, &cv]()
        {
            bp::child c = bp::child(args,
                bp::std_in.close(),
                bp::std_out > data,
                bp::std_err > error,
                io_context);
            pid = c.id();
            lk.unlock();
            cv.notify_one();
            io_context.run();
        }
    );
    m_workerThread.detach();
    cv.wait(lk);
    return std::make_tuple<>(pid, data.share(), error.share());
}