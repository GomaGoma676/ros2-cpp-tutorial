#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "custom_interface/msg/gps.hpp" // Gps.msg に対応するヘッダー

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
      : Node("minimal_subscriber")
  {
    auto topic_callback =
        [this](const custom_interface::msg::Gps::SharedPtr msg) -> void
    {
      RCLCPP_INFO_STREAM(
          this->get_logger(),
          "I heard: [ID: " << msg->sensor_id << ", Healthy: " << std::boolalpha << msg->is_healthy << ", Lat: " << msg->lat << ", Lon: " << msg->lon << "]");
    };

    subscription_ =
        this->create_subscription<custom_interface::msg::Gps>(
            "gps", 10, topic_callback);
  }

private:
  rclcpp::Subscription<custom_interface::msg::Gps>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}