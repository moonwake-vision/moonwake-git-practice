#include <std_msgs/msg/string.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <functional>

class SteeringHintNode : public rclcpp::Node{
    public:
        SteeringHintNode() : Node("steering_hint_node"){
            declare_parameter<double>("deadband",0.10);
            get_parameter("deadband",deadband_);
            hint_publisher_=create_publisher<std_msgs::msg::String>("steering_hint",10);
            offset_subscriber_=create_subscription<std_msgs::msg::Float64>("target_offset",10,
                                   std::bind(&SteeringHintNode::offset_callback,this,std::placeholders::_1));

    }
    private:
        double deadband_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr hint_publisher_;
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr offset_subscriber_;
        std_msgs::msg::String message;

        void offset_callback(const std_msgs::msg::Float64::SharedPtr msg){
            if( (msg->data) < (-deadband_) ){
                message.data="LEFT";
                hint_publisher_->publish(message);
            }
            else if( (msg->data) >= (-deadband_) && (msg->data) <= (deadband_) ){
                message.data="STRAIGHT";
                hint_publisher_->publish(message);
            }
            else{
                message.data="RIGHT";
                hint_publisher_->publish(message);
            }

        };

};

int main(int argc,char* argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<SteeringHintNode>());
    rclcpp::shutdown();
    return 0;
}