#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class TalkerNode : public rclcpp::Node{
    public:
        TalkerNode() : Node("talker_node"){
            publisher_=create_publisher<std_msgs::msg::String>("chatter",10);
            timer_=create_wall_timer(
                       std::chrono::milliseconds(500),
                       [this](){
                            std_msgs::msg::String message;
                            message.data="Hello ROS2!";
                            publisher_->publish(message);
                       }

                   );
        }
    private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
    };

int main(int argc,char* argv[]){
    rclcpp::init(argc,argv);
    std::shared_ptr<TalkerNode> talker_node;
    talker_node=std::make_shared<TalkerNode>();
    rclcpp::spin(talker_node);
    rclcpp::shutdown();
    return 0;
}