#include "ros/ros.h"
#include "std_msgs/String.h"
#include "package1/imu.h"
#include <sstream>
#include <boost/asio.hpp>           
#include <boost/bind.hpp>
#include "packet.h"
#include "imu_data_decode.h"
using namespace boost::asio;

int main(int argc, char *argv[])
{uint8_t ID;
int16_t Acc[3];
int16_t Gyo[3];
int16_t Mag[3];
float Eular[3];
float Quat[4];
int32_t Pressure;
	
	ros::init(argc, argv, "talker");

	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<package1::imu>("imu_info", 1);
	package1::imu msg;

unsigned char buff[41];
   io_service iosev;
   serial_port sp(iosev, "/dev/ttyUSB0");        
   sp.set_option(serial_port::baud_rate(115200));
   sp.set_option(serial_port::flow_control());
   sp.set_option(serial_port::parity());
   sp.set_option(serial_port::stop_bits());
   sp.set_option(serial_port::character_size(8));

	imu_data_decode_init();
	ros::Rate loop_rate(10);
while (ros::ok())
	{
	    read (sp,buffer(buff));
	    for(int i=0;i<41;i++)
		{
		Packet_Decode(buff[i]);
    
    	get_raw_acc(Acc);
    	get_raw_gyo(Gyo);
 		get_raw_mag(Mag);
    	get_eular(Eular);
    	get_quat(Quat);
    	get_id(&ID);
		}
		ROS_INFO("Acc: %d %d %d", Acc[0], Acc[1], Acc[2]);
		ROS_INFO("---------------------");
		msg.acc_x=Acc[0]*0.0098;
		msg.acc_y=Acc[1]*0.0098;
		msg.acc_z=Acc[2]*0.0098;
		msg.header.frame_id="imu_frame";
		msg.header.stamp = ros::Time::now();
		//msg.num=1;
		chatter_pub.publish(msg);
		//loop_rate.sleep();
	}

	return 0;
}
