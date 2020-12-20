# ros_ffmpeg_controller

This ROS package simply start or stop an ffmpeg process by sending specific messages to its services.

## Services

### 1) Execute Service

You can make a call to execute service to start an ffmpeg process with the given parameters;


* input_path
* output_path
* framerate
* bitrate_k
* max_delay
* input_format
* output_format
* probesize
* video_size
* vcodec
* thread_count

And returns the pid of the opened process.


### 2) Stop Service

You can stop the process with the given process id

* pid

And it returns a boolean indicates that operation was successful or not.
