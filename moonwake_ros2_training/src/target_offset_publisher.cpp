#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <memory>
#include <vector>
#include <cstddef>
#include <chrono>
#include <functional>

class TargetOffsetPublisher : public rclcpp::Node{
    public:
        TargetOffsetPublisher() : Node("target_offset_publisher"){
            declare_parameter<double>("publish_hz",5.0);
            declare_parameter<double>("amplitude",0.8);
            get_parameter("publish_hz",publish_hz_);
            get_parameter("amplitude",amplitude_);

            publisher_=create_publisher<std_msgs::msg::Float64>("target_offset",10);
            double A=amplitude_;
            offset_values_={-A,-A/2,0,A/2,A};


            timer_=create_wall_timer(std::chrono::duration<double>(1.0/publish_hz_),
                                        std::bind(&TargetOffsetPublisher::publish_offset,this));

        }

    private:
        double publish_hz_;
        double amplitude_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        std::vector<double> offset_values_;
        std::size_t current_index_=0;
        void publish_offset(){
            double offset=offset_values_[current_index_];
            std_msgs::msg::Float64 msg;
            msg.data=offset;
            current_index_++;
            publisher_->publish(msg);

            if(current_index_==offset_values_.size()){
                current_index_=0;
            }
        }

};

int main(int argc,char*argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<TargetOffsetPublisher>());
    rclcpp::shutdown();
    return 0;
}