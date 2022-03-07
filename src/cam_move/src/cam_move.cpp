#include <ros/ros.h>

//#include <darknet_ros_msgs/BoundingBox.h>
//#include <darknet_ros_msgs/BoundingBoxes.h>

#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/ModelStates.h>

#include <iostream>


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub = n_.advertise<gazebo_msgs::ModelState>("/gazebo/set_model_state",100);

    //Topic you want to subscribe
    sub1 = n_.subscribe("/gazebo/model_states", 2, &SubscribeAndPublish::SubCallback, this);
	//sub2 = n_.subscribe("/darknet_ros/bounding_boxes", 3, &SubscribeAndPublish::Boxcallback, this);

	MS.model_name = (std::string) "kinect_2";
	MS.pose.position.x = 0;
	MS.pose.position.y = 1.4;
	MS.pose.position.z = 1.2;
	MS.pose.orientation.x = 0;
	MS.pose.orientation.y = 0;
	MS.pose.orientation.z = 0.717356;
	MS.pose.orientation.w = 0.696706;
	MS.reference_frame = (std::string) "world";





  }

  void SubCallback(const gazebo_msgs::ModelStates& m)
  {
	MS.pose.position.x = MS.pose.position.x +0.00012 ;
	pub.publish(MS);
	

  }

 
private:
  ros::NodeHandle n_; 
  ros::Publisher pub;
  ros::Subscriber sub1;
  gazebo_msgs::ModelState MS;
};

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "cam_move");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}
