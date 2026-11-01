#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include <memory>

class HeartbeatNode : public rclcpp::Node{
    public:
        HeartbeatNode():Node("heartbeat_node"){
            timer_=create_wall_timer(std::chrono::seconds(1),[this](){
                RCLCPP_INFO(get_logger(),"heartbeat alive");
            });
        }

    private:
        rclcpp::TimerBase::SharedPtr timer_;

};

int main(int argc,char* argv[]){
    rclcpp::init(argc,argv);
    auto node = std::make_shared<HeartbeatNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}