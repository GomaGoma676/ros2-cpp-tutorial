#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "custom_interface/msg/gps.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
      : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<custom_interface::msg::Gps>("gps", 10);
    auto timer_callback =
        [this]() -> void
    {
      auto message = custom_interface::msg::Gps();
      message.sensor_id = "sensor_1";
      message.is_healthy = true;
      message.lat = 35.6812f + (this->count_ * 0.0001f);
      message.lon = 139.7671f + (this->count_ * 0.0001f);

      RCLCPP_INFO_STREAM(
        this->get_logger(),
        "Publishing: ID='" << message.sensor_id << 
        "', Healthy=" << (message.is_healthy ? "true" : "false") << 
        ", Lat=" << message.lat << 
        ", Lon=" << message.lon
      );
      this->publisher_->publish(message);
      this->count_++;
    };
    timer_ = this->create_wall_timer(500ms, timer_callback);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<custom_interface::msg::Gps>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
