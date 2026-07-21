//C++ Standard Headers
#include <chrono>
#include <memory>

//ROS Headers
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "imu.h"

using namespace std::chrono_literals;

class ImuPublisher : public rclcpp::Node
{
public:
    ImuPublisher() : Node("imu_node"), imu()
    {
        imu.initialization();

        publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("/imu", 100);

        auto imu_callback = 
            [this]() -> void {
                data = imu.getData();
                std::string m = std::to_string(data[0]);
                RCLCPP_INFO(this->get_logger(), "Publishing data[0] : %s", m.c_str());
            };

        timer_ = this->create_wall_timer(10ms, imu_callback);
    }
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;

    IMU imu;
    float* data;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImuPublisher>());
    rclcpp::shutdown();
    return 0;
}