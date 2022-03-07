update: 25.02.2021
	
How to run:
	For object detection:	
		1.launch the gazebo world: # node that contains objects and the prosthesis
			roslaunch prosthesis_v7 gazebo.launch
		2.launch deep-learning node:
			roslaunch darknet_ros darknet_ros.launch: #node that do the detection work
		3.run cam_move node: # node to move prosthesis horizontally
			rosrun prosthesis_v7 cam_move
			(in this file change line45 and line50, then the speed will be changed)


	For auto localization&grasping:
		1.launch the gazebo world: # node that contains objects and the prosthesis
			roslaunch prosthesis_v7 gazebo.launch
		2.launch deep-learning node:
			roslaunch darknet_ros darknet_ros.launch: #node that do the detection work
		3.run depth processing node:
			rosrun depth_proc depth_proc
		4.run the grasping node: #subcribe the center point and create the trajectory and let the robot move
			rosrun prosthesis_v7 grasping_task_panner.py
			


Introduction:

darknet_ros is the deep learning part to detect objects.
depth_proc is the package to retrieve the coordinates from camera.
cam_gazebo is the package to run the world file which is decorated with Coke can and standing Persong.





