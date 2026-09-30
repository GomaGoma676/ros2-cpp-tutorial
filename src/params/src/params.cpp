#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int64.hpp"


using namespace std::chrono_literals;

class NumberPublisherNode : public rclcpp::Node
{
public:
  NumberPublisherNode()
  : Node("number_publisher")
  {
        this->declare_parameter("number", 2);

        number_publisher_ = this->create_publisher<std_msgs::msg::Int64>("number",10);

        number_timer_ = this->create_wall_timer(
            std::chrono::duration<double>(1.0),
            [this]() { publishNumber(); });

        RCLCPP_INFO(this->get_logger(), "Number publisher has been started.");
  }

private:
    int number_;
    void publishNumber()
    {
        number_ = this->get_parameter("number").as_int();
        auto msg = std_msgs::msg::Int64();
        msg.data = number_;
        number_publisher_->publish(msg);
    }
    rclcpp::Publisher<std_msgs::msg::Int64>::SharedPtr number_publisher_;
    rclcpp::TimerBase::SharedPtr number_timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NumberPublisherNode>());
  rclcpp::shutdown();
  return 0;
}
