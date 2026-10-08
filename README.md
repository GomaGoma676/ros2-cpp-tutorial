# ROS 2 C++ Development Tutorial

> **Reference Documentation:**
>
> - [ROS2 Docker Desktop VNC](https://github.com/Tiryoh/docker-ros2-desktop-vnc)
> - [ROS2 Jazzy: Official Tutorial](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)

## 0. Environment Setup (Docker)

Launch the ROS 2 Jazzy Desktop environment:

```bash
docker run -d \
  -p 6080:80 \
  --security-opt seccomp=unconfined \
  --shm-size=512m \
  -v ~/ros2_ws:/home/ubuntu/ros2_ws \
  ghcr.io/tiryoh/ros2-desktop-vnc:jazzy
```

## 1. Simple C++ Package Setup

```bash
# Navigate to workspace and set up source directory
cd ~/ros2_ws
mkdir src && cd src

# Create a basic C++ package
ros2 pkg create --build-type ament_cmake --license Apache-2.0 cpp_basic
cd cpp_basic

# Open in VSCodium (Install CMake Tools / C++ extension if prompted)
codium .
touch basic.cpp

# Write code
# Adjust CMakeLists.txt and package.xml

# Build and execute
cd ~/ros2_ws
colcon build --packages-select cpp_basic
source install/setup.bash
ros2 run cpp_basic basic
```

## 2. Publisher & Subscriber

```bash
# Create package with ROS 2 C++ client library
cd ~/ros2_ws/src
ros2 pkg create pub_sub --dependencies rclcpp std_msgs

touch {pub.cpp,sub.cpp}

# Write code
# Adjust CMakeLists.txt and package.xml

# Build
cd ~/ros2_ws
colcon build --packages-select pub_sub
source install/setup.bash

# Run Talker and Listener nodes
ros2 run pub_sub talker
ros2 run pub_sub listener
```

## 3. Services (Server & Client)

```bash
# Inspect existing service interface
ros2 interface show example_interfaces/srv/AddTwoInts

# Create package with dependencies
cd ~/ros2_ws/src
ros2 pkg create server_client --dependencies rclcpp example_interfaces

# Write code
# Adjust CMakeLists.txt and package.xml

# Build and run server/client
cd ~/ros2_ws
colcon build --packages-select server_client
source install/setup.bash

ros2 run server_client add_server
ros2 run server_client add_client

# Service CLI utilities
ros2 service list
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 1, b: 2}"
```

## 4. Parameters

```bash
# Create parameter package
cd ~/ros2_ws/src
ros2 pkg create params --dependencies rclcpp std_msgs

# Write code
# Adjust CMakeLists.txt and package.xml

# Build params package
cd ~/ros2_ws
colcon build --packages-select params
source install/setup.bash

# Run node with an overridden parameter
ros2 run params params --ros-args -p number:=3

# Inspect and update parameters dynamically
ros2 topic echo /number
ros2 param set number_publisher number 5
```

## 5. Custom Messages

```bash
# Create custom interface package
cd ~/ros2_ws/src
ros2 pkg create custom_interface
cd custom_interface
rm -rf include src   # Interface-only packages do not need source/include folders
mkdir msg &&  cd msg
touch Gps.msg

# Write code
# Adjust CMakeLists.txt and package.xml

# Build custom interface package
cd ~/ros2_ws
colcon build --packages-select custom_interface
source install/setup.bash
ros2 interface show custom_interface/msg/Gps

# Create new package that consumes a custom interface
cd ~/ros2_ws/src
ros2 pkg create pub_sub_custom_msg --dependencies rclcpp custom_interface

# Write code
# Adjust CMakeLists.txt and package.xml

# Build and run
cd ~/ros2_ws
colcon build --packages-select pub_sub_custom_msg
source install/setup.bash

ros2 run pub_sub_custom_msg talker_custom
ros2 run pub_sub_custom_msg listener_custom
```

## 6. Actions

```bash
# Create custom interface package and action demo package
cd ~/ros2_ws/src
ros2 pkg create custom_action_interfaces
ros2 pkg create action_basic --dependencies rclcpp rclcpp_action custom_action_interfaces

# Write code for custom interface & action demo
# Adjust CMakeLists.txt and package.xml

cd ~/ros2_ws
colcon build --packages-select custom_action_interfaces action_basic
source install/setup.bash

# Run Action Server
ros2 run action_basic count_until_server

# Action CLI Commands
# Send a goal
ros2 action send_goal /count_until custom_action_interfaces/action/CountUntil "{target_number: 4, period: 1.0}"

# Cancel an active goal
ros2 service call /count_until/_action/cancel_goal action_msgs/srv/CancelGoal "{}"
```

## 7. Executors & Multithreading

```bash
# Create executor package
cd ~/ros2_ws/src
ros2 pkg create executor_basic --dependencies rclcpp

# Write code
# Adjust CMakeLists.txt and package.xml

# Build package
cd ~/ros2_ws
colcon build --packages-select executor_basic
source install/setup.bash

# Single-threaded vs Multi-threaded execution
ros2 run executor_basic single_th_executor
ros2 run executor_basic multi_th_executor
```

## 8. Rosbag Recording & Playback

```bash
# Create rosbag package
cd ~/ros2_ws/src
ros2 pkg create rosbag_basic --dependencies rclcpp rosbag2_cpp std_msgs

# Write code
# Adjust CMakeLists.txt and package.xml

# Build package
cd ~/ros2_ws
colcon build --packages-select rosbag_basic
source install/setup.bash

# Launch rosbag recorder + publish data
ros2 run rosbag_basic rosbag_recorder
ros2 run demo_nodes_cpp talker

# Playback recorded rosbag data and subscribe it
ros2 run demo_nodes_cpp listener
ros2 bag play my_bag
```
