#include <ros/ros.h>
#include <sensor_msgs/Image.h>
//#include <darknet_ros_msgs/BoundingBox.h>
//#include <darknet_ros_msgs/BoundingBoxes.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PointStamped.h>
#include <sensor_msgs/PointCloud2.h>
#include <depth_proc/BoundingBoxes.h>
#include <depth_proc/BoundingBox.h>

#include <tf/transform_listener.h>
#include <iostream>


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub = n_.advertise<geometry_msgs::PointStamped>("/can_center",100);

    //Topic you want to subscribe
    sub1 = n_.subscribe("/camera/depth/points", 3, &SubscribeAndPublish::Depthcallback, this);
	sub2 = n_.subscribe("/darknet_ros/bounding_boxes", 3, &SubscribeAndPublish::Boxcallback, this);
    count=0;
  }

  void Depthcallback(const sensor_msgs::PointCloud2& pCloud)
  {
	//can_center_2d.x = 236;
	//can_center_2d.y = 180;
    //get width and height of 2d point clud data
    int width = pCloud.width;
    int height = pCloud.height;
	//convert from pixel(x,y) to position in array
	//where also (x,y,z) starts
	int u = can_center_2d.x;
	int v = can_center_2d.y;
	int arrayPosition = v*pCloud.row_step + u*pCloud.point_step;

	//compute position in array where x,y,z data start
    int arrayPosX = arrayPosition + pCloud.fields[0].offset; // X has an offset of 0
    int arrayPosY = arrayPosition + pCloud.fields[1].offset; // Y has an offset of 4
    int arrayPosZ = arrayPosition + pCloud.fields[2].offset; // Z has an offset of 8

	//now retrieve the data of xyz coordinates
    float X = 0.0;
    float Y = 0.0;
    float Z = 0.0;

    memcpy(&X, &pCloud.data[arrayPosX], sizeof(float));
    memcpy(&Y, &pCloud.data[arrayPosY], sizeof(float));
    memcpy(&Z, &pCloud.data[arrayPosZ], sizeof(float));

	can_center_3d.header = pCloud.header;
    can_center_3d.point.x = X;
	can_center_3d.point.y = Y;
	can_center_3d.point.z = Z;

	//ROS_INFO_STREAM("x is: "<< can_center_3d.x);
	//ROS_INFO_STREAM("y is: "<< can_center_3d.y);
	//ROS_INFO_STREAM("z is: "<< can_center_3d.z);


	//transform into world coordinates
    //can_center_3d.point.x = 1;
	//can_center_3d.point.y = 1;
	//can_center_3d.point.z = 1;
	//tf::TransformListener listener;
	//listener.transformPoint("base_link", can_center_3d, center_transformed);

	//publich it to topic /can_center
	//pub.publish(center_transformed);
    count = count+1;
    if(count==50)
    {    
      count=0;
      ROS_INFO_STREAM("Bottle detected!");
      ROS_INFO_STREAM("World coordinate:\n("<<can_center_3d.point.z-0.04<<","<<can_center_3d.point.y+0.103<<","<<can_center_3d.point.x+0.99<<")");
	}

	pub.publish(can_center_3d);

  }

  void Boxcallback(const depth_proc::BoundingBoxes& BB)
  {	

	depth_proc::BoundingBox r;
	int length = BB.bounding_boxes.size();
	//ROS_INFO_STREAM("size of BB.boxes: "<<sizeof(BB.bounding_boxes));
	int i=0;
	for(i;i<length;i++)
	{	
		//ROS_INFO_STREAM("loop comes");
		r = BB.bounding_boxes[i];	
		//ROS_INFO_STREAM("id:"<< r.id);
		if((r.id==39)||(r.id==75))  // 39-bottle  75-vase
		{
			can_center_2d.x = (r.xmin+r.xmax)/2;
			can_center_2d.y = (r.ymin+r.ymax)/2;	
			//ROS_INFO_STREAM("Bottle detected!");
			//ROS_INFO_STREAM("At ("<<can_center_2d.x<<","<<can_center_2d.y<<")");
			//pub.publish(can_center_2d);
		}
	}

  }
 
private:
  ros::NodeHandle n_; 
  ros::Publisher pub;
  ros::Subscriber sub1;
  ros::Subscriber sub2;
  geometry_msgs::Point can_center_2d;
  geometry_msgs::PointStamped can_center_3d;
  geometry_msgs::PointStamped center_transformed;
  int count;
};

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "depth_proc");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}












