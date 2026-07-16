cmake_minimum_required(VERSION 3.20)
project(autonomous_car)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)

include_directories(include/autonomous_car/)

add_executable(rpi_cam_node 
                src/camera_publisher.cpp 
                src/resources/camera.cpp)

add_executable(rpi_cam_tf 
                src/camera_transforms.cpp)


target_link_libraries(rpi_cam_node PUBLIC
  rclcpp::rclcpp
  ${sensor_msgs_TARGETS}
  ${OpenCV_LIBS}
  cv_bridge::cv_bridge
  image_transport::image_transport
)

target_link_libraries(rpi_cam_tf PUBLIC
  rclcpp::rclcpp
  ${sensor_msgs_TARGETS}
  ${geometry_msgs_TARGETS}
  ${nav_msgs_TARGETS}
  tf2::tf2
  tf2_ros::tf2_ros
)

install(TARGETS 
  rpi_cam_node
  rpi_cam_tf
  DESTINATION lib/${PROJECT_NAME})

ament_package()