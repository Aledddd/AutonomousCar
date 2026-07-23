//C++ Standard Headers
#include <chrono>
#include <memory>

//ROS Headers
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"

#include "imu.h"

using namespace std::chrono_literals;

class ImuPublisher : public rclcpp::Node
{
public:
    ImuPublisher() : Node("imu_node"), imu()
    {
        imu.initialization();

        publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("imu/data", 100);

        auto imu_callback = 
            [this]() -> void {
                imu.getRawData();
                sensor_msgs::msg::Imu imu_msg;
                imu_msg.header.stamp = this->get_clock()->now();
                imu_msg.header.frame_id = "imu_link";
                
                imu_msg.orientation.x = imu.data[3];
                imu_msg.orientation.y = imu.data[4];
                imu_msg.orientation.z = imu.data[5];
                imu_msg.orientation.w = imu.data[6];

                imu_msg.angular_velocity.x = imu.data[0];
                imu_msg.angular_velocity.y = imu.data[1];
                imu_msg.angular_velocity.z = imu.data[2];

                imu_msg.linear_acceleration.x = imu.data[7];
                imu_msg.linear_acceleration.y = imu.data[8];
                imu_msg.linear_acceleration.z = imu.data[9];

                publisher_->publish(imu_msg);

        };

        timer_ = this->create_wall_timer(10ms, imu_callback);
    }
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;

    IMU imu;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImuPublisher>());
    rclcpp::shutdown();
    return 0;
}