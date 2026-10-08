#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "custom_action_interfaces/action/increment.hpp"

using Increment = custom_action_interfaces::action::Increment;
using IncrementGoalHandle = rclcpp_action::ServerGoalHandle<Increment>;

class IncrementServerNode : public rclcpp::Node
{
public:
  IncrementServerNode()
      : Node("increment_server")
  {
    cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    increment_server_ = rclcpp_action::create_server<Increment>(
        this,
        "increment",
        [this](const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const Increment::Goal> goal)
        {
          return this->goal_callback(uuid, goal);
        },
        [this](const std::shared_ptr<IncrementGoalHandle> goal_handle)
        {
          return this->cancel_callback(goal_handle);
        },
        [this](const std::shared_ptr<IncrementGoalHandle> goal_handle)
        {
          this->handle_accepted_callback(goal_handle);
        },
        rcl_action_server_get_default_options(),
        cb_group_);
    RCLCPP_INFO(this->get_logger(), "Action server has been started");
  }

private:
  rclcpp_action::GoalResponse goal_callback(
      [[maybe_unused]] const rclcpp_action::GoalUUID &uuid, [[maybe_unused]] std::shared_ptr<const Increment::Goal> goal)
  {
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  rclcpp_action::CancelResponse cancel_callback(
      [[maybe_unused]] const std::shared_ptr<IncrementGoalHandle> goal_handle)
  {
    return rclcpp_action::CancelResponse::ACCEPT;
  }
  void handle_accepted_callback(
      const std::shared_ptr<IncrementGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Executing...");
    execute_goal(goal_handle);
  }
  void execute_goal(const std::shared_ptr<IncrementGoalHandle> goal_handle)
  {
    int target_number = goal_handle->get_goal()->target_number;
    int counter = 0;
    rclcpp::Rate loop_rate(1.0);

    auto result = std::make_shared<Increment::Result>();
    auto feedback = std::make_shared<Increment::Feedback>();

    for (int i = 0; i < target_number && rclcpp::ok(); i++)
    {
      if (goal_handle->is_canceling())
      {
        result->result_number = counter;
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

    result->result_number = counter;
    goal_handle->succeed(result);
  }
  rclcpp_action::Server<Increment>::SharedPtr increment_server_;
  rclcpp::CallbackGroup::SharedPtr cb_group_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<IncrementServerNode>();
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
