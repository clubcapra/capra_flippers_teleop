#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <capra_control_msgs/msg/flippers.hpp>

class FlippersTeleop : public rclcpp::Node
{
public:
  FlippersTeleop()
  : Node("flippers_teleop")
  {
    // Declare parameters
    front_select_button_ = this->declare_parameter<int>("front_select_button", 0);
    rear_select_button_ = this->declare_parameter<int>("rear_select_button", 0);
    move_axis_ = this->declare_parameter<int>("move_axis", 0);
    fl_scale_ = this->declare_parameter<double>("fl_scale", 1.0);
    fr_scale_ = this->declare_parameter<double>("fr_scale", 1.0);
    rl_scale_ = this->declare_parameter<double>("rl_scale", 1.0);
    rr_scale_ = this->declare_parameter<double>("rr_scale", 1.0);

    // Publisher
    pub_ = this->create_publisher<capra_control_msgs::msg::Flippers>(flippers_topic_, 10);

    // Subscriber
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
      joy_topic_,
      10,
      std::bind(&FlippersTeleop::joyCallback, this, std::placeholders::_1)
    );

  }

private:
  void joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    capra_control_msgs::msg::Flippers out;
    bool front_selected = false, rear_selected = false;
    if (front_select_button_ < 0 || front_select_button_ >= static_cast<int>(msg->buttons.size()))
    {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        2000,
        "Button index %d out of range (buttons size: %zu)",
        front_select_button_,
        msg->buttons.size()
      );
      return;
    } else {
      front_selected = msg->buttons[front_select_button_];
    }
    if (rear_select_button_ < 0 || rear_select_button_ >= static_cast<int>(msg->buttons.size()))
    {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        2000,
        "Button index %d out of range (buttons size: %zu)",
        rear_select_button_,
        msg->buttons.size()
      );
      return;
    } else {
      rear_selected = msg->buttons[rear_select_button_];
    }
    if (move_axis_ < 0 || move_axis_ >= static_cast<int>(msg->axes.size()))
    {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        2000,
        "Axis index %d out of range (buttons size: %zu)",
        move_axis_,
        msg->axes.size()
      );
      return;
    } else {
      out.control_mode = capra_control_msgs::msg::Flippers::VELOCITY_CONTROL_MODE;
      double front_vel = front_selected ? msg->axes[move_axis_] : 0;
      double rear_vel = rear_selected ? msg->axes[move_axis_] : 0;

      out.front_left = front_vel * fl_scale_;
      out.front_right = front_vel * fr_scale_;
      out.rear_left = rear_vel * rl_scale_;
      out.rear_right = rear_vel * rr_scale_;
    }
    if (out != last_ || out.front_left != 0 || out.front_right != 0 || out.rear_left != 0 || out.rear_right != 0) {
      // Don't publish if all velocities == 0 except for the first time they go from != 0 to == 0.
      // Publish anytime the velocities != 0
      // This allows the flippers_mux to select another input if the user isn't controlling them manually
      pub_->publish(out);
    }
    last_ = out;
  }
  
  capra_control_msgs::msg::Flippers last_;

  int front_select_button_;
  int rear_select_button_;
  int move_axis_;
  double fl_scale_;
  double fr_scale_;
  double rl_scale_;
  double rr_scale_;
  std::string joy_topic_ = "~/joy";
  std::string flippers_topic_ = "~/flippers";

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
  rclcpp::Publisher<capra_control_msgs::msg::Flippers>::SharedPtr pub_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FlippersTeleop>());
  rclcpp::shutdown();
  return 0;
}
