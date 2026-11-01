#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class ListenerNode : public rclcpp::Node {
    public:
        ListenerNode():Node("listener_node"){
            subscriber_=create_subscription<std_msgs::msg::String>(
                "chatter",10,[this](std_msgs::msg::String::ConstSharedPtr msg){
                                 RCLCPP_INFO(get_logger(),"accept the message:%s",msg->data.c_str());
                             }
                            );

        }
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr  subscriber_;

};

int main(int argc,char* argv[]){
    rclcpp::init(argc,argv);
    auto listener_node=std::make_shared<ListenerNode>();
    rclcpp::spin(listener_node);
    rclcpp::shutdown();
    return 0;
}