#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

#include "ros_ffmpeg_controller/command_executor.h"
#include "ros_ffmpeg_controller/ExecuteCommand.h"
#include "ros_ffmpeg_controller/StopCommand.h"

CommandExecutor command_executor;
std::string FFMPEG_EXE_PATH = bp::search_path("ffmpeg").string();
std::string NO_AUDIO_ARG = "-an";
std::string FORMAT_ARG_KEY = "-f";
std::string FRAMERATE_ARG_KEY = "-framerate";
std::string MAX_DELAY_ARG_KEY = "-max_delay";
std::string PROBESIZE_ARG_KEY = "-probesize";
std::string VIDEOSIZE_ARG_KEY = "-video_size";
std::string VIDEO_INPUT_ARG_KEY = "-i";
std::string VIDEO_CODEC_ARG_KEY = "-vcodec";
std::string THREAD_CNT_ARG_KEY = "-threads";
std::string BITRATE_ARG_KEY = "-b:v";

boost::asio::io_context io_context;

std::vector<std::string> buildCommandString(ros_ffmpeg_controller::ExecuteCommand::Request& req){
    std::vector<std::string> args;
    args.push_back(FFMPEG_EXE_PATH);
    args.push_back(NO_AUDIO_ARG);
    if(!req.inputFormat.empty()){
        args.push_back(FORMAT_ARG_KEY);
        args.push_back(req.inputFormat);
    }
    if(req.framerate!=0){
        args.push_back(FRAMERATE_ARG_KEY);
        args.push_back(std::to_string(req.framerate));
    }
    
    if(req.max_delay!=0){
        args.push_back(MAX_DELAY_ARG_KEY);
        args.push_back(std::to_string(req.max_delay));  
    }
    
    if(req.probesize!=0){
        args.push_back(PROBESIZE_ARG_KEY);
        args.push_back(std::to_string(req.probesize));
    }
    
    if(!req.video_size.empty()){
        args.push_back(VIDEOSIZE_ARG_KEY);
        args.push_back(req.video_size);
    }
    
    args.push_back(VIDEO_INPUT_ARG_KEY);
    args.push_back(req.input_path);
    if(!req.vcodec.empty()){
        args.push_back(VIDEO_CODEC_ARG_KEY);
        args.push_back(req.vcodec);
    }
    
    if(req.threadCount!=0){
        args.push_back(THREAD_CNT_ARG_KEY);
        args.push_back(std::to_string(req.threadCount));
    }

    if(req.bitrate_k!=0){
        args.push_back(BITRATE_ARG_KEY);
        args.push_back(std::to_string(req.bitrate_k) + "k");
    }
    
    args.push_back(FORMAT_ARG_KEY);
    args.push_back(req.outputFormat);
    args.push_back(req.output_path);
    for(std::string arg: args)
        std::cout<<arg<<" ";
    std::cout<<std::endl;
    return args;

}

bool handle_ffmpeg_execute_request(ros_ffmpeg_controller::ExecuteCommand::Request& req,
    ros_ffmpeg_controller::ExecuteCommand::Response& res)
{
    std::vector<std::string> commandArgs = buildCommandString(req);
    ExecutionResult results = command_executor.execute(commandArgs, io_context);
	res.pid = std::get<0>(results);
    return res.pid > 0;
}

bool handle_ffmpeg_stop_request(ros_ffmpeg_controller::StopCommand::Request& req,
    ros_ffmpeg_controller::StopCommand::Response& res)
{    
	res.result = command_executor.stop(req.pid);
    return res.result;
}

int main(int argc, char** argv){
   
   
    ros::init(argc, argv, "ffmpeg_controller");
    ros::NodeHandle n;

    ros::ServiceServer executerService = n.advertiseService("/ffmpeg_controller/execute", handle_ffmpeg_execute_request);
    ros::ServiceServer stopperService = n.advertiseService("/ffmpeg_controller/stop", handle_ffmpeg_stop_request);
    ros::spin();
}