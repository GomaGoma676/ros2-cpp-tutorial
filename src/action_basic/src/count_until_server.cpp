#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "custom_action_interfaces/action/count_until.hpp"

using CountUntil = custom_action_interfaces::action::CountUntil;
using CountUntilGoalHandle = rclcpp_action::ServerGoalHandle<CountUntil>;
using namespace std::placeholders;

class CountUntilServerNode : public rclcpp::Node
{
public:
  CountUntilServerNode()
  : Node("count_until_server")
  {
      cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
      count_until_server_ = rclcpp_action::create_server<CountUntil>(
        this,
        "count_until",
        [this](const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const CountUntil::Goal> goal) {
            return this->goal_callback(uuid, goal);
        },
        [this](const std::shared_ptr<CountUntilGoalHandle> goal_handle) {
            return this->cancel_callback(goal_handle);
        },
        [this](const std::shared_ptr<CountUntilGoalHandle> goal_handle) {
            this->handle_accepted_callback(goal_handle);
        },
        rcl_action_server_get_default_options(),
        cb_group_
      );
      RCLCPP_INFO(this->get_logger(), "Action server has been started");    
  }

private:
  rclcpp_action::GoalResponse goal_callback(
    [[maybe_unused]] const rclcpp_action::GoalUUID &uuid, [[maybe_unused]] std::shared_ptr<const CountUntil::Goal> goal)
  {
    // (void)uuid;
    // (void)goal;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  rclcpp_action::CancelResponse cancel_callback(
    const std::shared_ptr<CountUntilGoalHandle> goal_handle)
  {
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }
  void handle_accepted_callback(
    const std::shared_ptr<CountUntilGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Executing...");
    execute_goal(goal_handle);
    //std::thread{ [this, goal_handle]() { this->execute_goal(goal_handle); } }.detach();
  }
  void execute_goal(const std::shared_ptr<CountUntilGoalHandle> goal_handle)
  {
    int target_number = goal_handle->get_goal()->target_number;
    double period = goal_handle->get_goal()->period;
    int counter = 0;
    rclcpp::Rate loop_rate(1.0/period);
    
    auto result = std::make_shared<CountUntil::Result>();
    auto feedback = std::make_shared<CountUntil::Feedback>();

    for (int i = 0; i < target_number && rclcpp::ok(); i++) {
      // Check if client requested cancellation
        if (goal_handle->is_canceling()) {
            result->reached_number = counter;
            goal_handle->canceled(result);
            RCLCPP_INFO(this->get_logger(), "Goal canceled at count %d", counter);
            return;
        }
        counter++;
        RCLCPP_INFO(this->get_logger(), "%d", counter);
        feedback->current_number = counter;
        goal_handle->publish_feedback(feedback);

        loop_rate.sleep();
    }
   
    result->reached_number = counter;
    goal_handle->succeed(result);
  }
  rclcpp_action::Server<CountUntil>::SharedPtr count_until_server_;
  rclcpp::CallbackGroup::SharedPtr cb_group_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CountUntilServerNode>();
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();
  //rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
