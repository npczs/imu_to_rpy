#include <math.h>
#include <iostream>

#include <ros/ros.h>
#include <serial/serial.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>

#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/NavSatStatus.h>

#include "tf/LinearMath/Quaternion.h"

#include "tf/transform_datatypes.h"

sensor_msgs::Imu rpy;
class imu_listener
{
private:

    ros::NodeHandle nh;
    ros::Subscriber imu_sub;
    ros::Publisher rpy_pub;
    
public:
    imu_listener()
    {

        imu_sub = nh.subscribe("imu/data", 1000,&imu_listener::callback, this);

        rpy_pub = nh.advertise<sensor_msgs::Imu>("imu2rpy/data", 100);
    }

    void callback(const sensor_msgs::Imu::ConstPtr& msg);

};

void imu_listener::callback(const sensor_msgs::Imu::ConstPtr& msg)
{
	tf::Quaternion quate;
    tf::quaternionMsgToTF(msg->orientation, quate);
	double roll, pitch, yaw;//定义存储r\p\y的容器
    tf::Matrix3x3(quate).getRPY(roll, pitch, yaw);//进行转换
    ROS_INFO_STREAM("do");
    rpy.header.stamp = ros::Time::now();
    rpy.header.frame_id = "pry_link";

    rpy.orientation.w = 1;
    rpy.orientation.x = roll;
    rpy.orientation.y = pitch;
    rpy.orientation.z = yaw;
    rpy.orientation_covariance[0] = 0;
    rpy.linear_acceleration.x = msg->linear_acceleration.x;
    rpy.linear_acceleration.y = msg->linear_acceleration.y;
    rpy.linear_acceleration.z = msg->linear_acceleration.z;
    rpy.linear_acceleration_covariance[0] = 0;
    rpy.angular_velocity.x = msg->angular_velocity.x;
    rpy.angular_velocity.y = msg->angular_velocity.y;
    rpy.angular_velocity.z = msg->angular_velocity.z;
    rpy.angular_velocity_covariance[0] = 0;
    rpy_pub.publish(rpy);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "imu2rpy");
    imu_listener imu_listener_1;
    ros::spin();


	
	return 0;

}