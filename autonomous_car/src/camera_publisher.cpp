//C++ Standard Headers
#include <chrono>
#include <memory>

//ROS Headers
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>


#include "camera.hpp"


using namespace std::chrono_literals;

class CameraPublisher : public rclcpp::Node
{
public:
    CameraPublisher() : Node("rpi_cam_node"), cam()
    {
        cam.setup(640, 480);
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("cam/image_raw", 10);

        auto timer_callback =
            [this]() -> void {
                if(cam.isOpened())
                {
                    cv::Mat frame_pub = cam.getFrame();
                    sensor_msgs::msg::Image::SharedPtr msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame_pub).toImageMsg();
                    this->publisher_->publish(*msg);
                }
        };

        timer_ = this->create_wall_timer(33ms, timer_callback);
    }
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
        
    Camera cam;      
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraPublisher>());
    rclcpp::shutdown();
    return 0;
}