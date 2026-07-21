//C++ Standard Headers
#include <chrono>
#include <memory>

//ROS Headers
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "imu.h"

using namespace std::chrono_literals;

class ImuPublisher : public rclcpp::Node, public IMU
{
public:
    ImuPublisher() : Node("imu_node"), imu()
    {
        initialization();

        publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("/imu", 100);

        auto imu_callback = 
            [this]() -> void {
                getRawData();
                getReadableData();

                sensor_msgs::msg::Imu::SharedPtr msg;
            };

        timer_ = this->create_wall_timer(10ms, imu_callback);
    }
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraPublisher>());
    rclcpp::shutdown();
    return 0;
}