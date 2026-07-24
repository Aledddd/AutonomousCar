//C++ Standard Headers
#include <chrono>
#include <memory>

//ROS Headers
#include "rclcpp/rclcpp.hpp"
#include "cv_bridge/cv_bridge.hpp"
#include "image_transport/image_transport.hpp"

#include "camera.hpp"


using namespace std::chrono_literals;

class CameraPublisher : public rclcpp::Node
{
public:
    CameraPublisher() : Node("cam_node"), cam()
    {
        cam.setup(640, 480);
        publisher_ = image_transport::create_publisher(this, "camera/image_raw");

        auto timer_callback =
            [this]() -> void {
                if(cam.isOpened())
                {
                    cv::Mat frame = cam.getFrame();
                    std_msgs::msg::Header header;
                    header.stamp = this->get_clock()->now();
                    header.frame_id = "camera_link";
                    
                    publisher_.publish(cv_bridge::CvImage(header, "bgr8", frame).toImageMsg());
                    
                    //publisher_->publish(*msg);
                }
        };

        timer_ = this->create_wall_timer(33ms, timer_callback);
    }
private:
    rclcpp::TimerBase::SharedPtr timer_;
    image_transport::Publisher publisher_;
        
    Camera cam;      
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraPublisher>());
    rclcpp::shutdown();
    return 0;
}