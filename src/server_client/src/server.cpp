#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class AddTwoIntsServerNode : public rclcpp::Node
{
public:
  AddTwoIntsServerNode()
  : Node("add_two_ints_server")
  {
      server_ = this->create_service<example_interfaces::srv::AddTwoInts>(
        "add_two_ints",
        [this](const example_interfaces::srv::AddTwoInts::Request::SharedPtr request,
              const example_interfaces::srv::AddTwoInts::Response::SharedPtr response) {
            response->sum = request->a + request->b;
            RCLCPP_INFO(this->get_logger(), "%d + %d = %d",
                        (int)request->a, (int)request->b, (int)response->sum);
        });
      RCLCPP_INFO(this->get_logger(), "Add Two Ints");
  }

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr server_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddTwoIntsServerNode>());
  rclcpp::shutdown();
  return 0;
}
