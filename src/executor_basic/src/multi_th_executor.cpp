#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <thread>
#include <sstream>
#include <map>
#include <mutex>
#include <atomic>

using namespace std::chrono_literals;

class ThreadIdMapper
{
public:
    static int getThreadNum()
    {
        static ThreadIdMapper instance;
        return instance.get(std::this_thread::get_id());
    }

private:
    int get(std::thread::id id)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (map_.find(id) == map_.end())
        {
            map_[id] = ++counter_;
        }
        return map_[id];
    }

    std::map<std::thread::id, int> map_;
    int counter_ = 0;
    std::mutex mutex_;
};

class Node1 : public rclcpp::Node
{
public:
    Node1() : Node("node1")
    {
        cb_group1_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        cb_group2_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        this->timer1_ = this->create_wall_timer(
            1000ms, [this]()
            { callbackTimer1(); }, cb_group1_);
        this->timer2_ = this->create_wall_timer(
            1000ms, [this]()
            { callbackTimer2(); }, cb_group2_);
    }

private:
    void callbackTimer1()
    {
        RCLCPP_INFO(this->get_logger(), "cb 1 on Thread: %d", ThreadIdMapper::getThreadNum());
        std::this_thread::sleep_for(2000ms);
    }

    void callbackTimer2()
    {
        RCLCPP_INFO(this->get_logger(), "cb 2 on Thread: %d", ThreadIdMapper::getThreadNum());
        std::this_thread::sleep_for(2000ms);
    }

    rclcpp::TimerBase::SharedPtr timer1_;
    rclcpp::TimerBase::SharedPtr timer2_;

    rclcpp::CallbackGroup::SharedPtr cb_group1_;
    rclcpp::CallbackGroup::SharedPtr cb_group2_;
};

class Node2 : public rclcpp::Node
{
public:
    Node2() : Node("node2")
    {
        cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        this->timer3_ = this->create_wall_timer(
            1000ms, [this]()
            { callbackTimer3(); }, cb_group_);
        this->timer4_ = this->create_wall_timer(
            1000ms, [this]()
            { callbackTimer4(); }, cb_group_);
    }

private:
    int counter_ = 0;
    // std::atomic<int> counter_{0};
    void callbackTimer3()
    {
        int current = ++counter_;
        RCLCPP_INFO(this->get_logger(), "cb 3 on Thread: %d - Counter: %d", ThreadIdMapper::getThreadNum(), current);
        std::this_thread::sleep_for(2000ms);
    }

    void callbackTimer4()
    {
        int current = ++counter_;
        RCLCPP_INFO(this->get_logger(), "cb 4 on Thread: %d - Counter: %d", ThreadIdMapper::getThreadNum(), current);
        std::this_thread::sleep_for(2000ms);
    }

    rclcpp::TimerBase::SharedPtr timer3_;
    rclcpp::TimerBase::SharedPtr timer4_;

    rclcpp::CallbackGroup::SharedPtr cb_group_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node1 = std::make_shared<Node1>();
    // auto node2 = std::make_shared<Node2>();
    unsigned int num_threads = std::thread::hardware_concurrency();
    num_threads = 4;
    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(), num_threads);

    RCLCPP_INFO(node1->get_logger(), "Total available threads (Pool size): %u", num_threads);
    // RCLCPP_INFO(node2->get_logger(), "Total available threads (Pool size): %u", num_threads);
    executor.add_node(node1);
    // executor.add_node(node2);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}
