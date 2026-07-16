#include <rclcpp/rclcpp.hpp>

#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance.hpp>
#include <nav_msgs/msg/odometry.hpp>

class CameraTransforms: public rclcpp::Node
{
    private:
        // Declare a timer for the tf_broadcaster_
        rclcpp::TimerBase::SharedPtr transform_timer_;
        // Declare the transforms broadcaster
        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        // Subscription for Odometry
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
        // Declare the transform to broadcast
        geometry_msgs::msg::TransformStamped transf_;

        void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
        {
            transf_.transform.translation.x = msg->pose.pose.position.x;
            transf_.transform.translation.y = msg->pose.pose.position.y;
            transf_.transform.translation.z = msg->pose.pose.position.z;

            transf_.transform.rotation.x = msg->pose.pose.orientation.x;
            transf_.transform.rotation.y = msg->pose.pose.orientation.y;
            transf_.transform.rotation.z = msg->pose.pose.orientation.z;
            transf_.transform.rotation.w = msg->pose.pose.orientation.w;
        }

        void tfTimer(){
            // All transforms must be correctly timestamped
            transf_.header.stamp = this->get_clock()->now();
            tf_broadcaster_->sendTransform(transf_);
        }

    public:
        CameraTransforms(const std::string & name):
        Node(name){
            // Initialize the transforms broadcaster
            tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
            // Define the "parent" frame
            transf_.header.frame_id = "map";
            // Define the "child" frame
            transf_.child_frame_id = "odom";
            // Initialize timer for publishing transform
            transform_timer_ = create_wall_timer(std::chrono::seconds(1),
            std::bind(&CameraTransforms::tfTimer, this));
            // Create subscriber for cmdVel
            odom_sub_ = create_subscription<nav_msgs::msg::Odometry>(
            "/odometry/slam",10,std::bind(&CameraTransforms::odomCallback, this, std::placeholders::_1));
        }     
        ~CameraTransforms(){}   
};

int main(int argc, char const *argv[]){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CameraTransforms>("rpi_cam_tf");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}