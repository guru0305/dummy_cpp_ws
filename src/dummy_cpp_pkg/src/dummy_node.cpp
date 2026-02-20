#include "rclcpp/rclcpp.hpp"

class DummyNode : public rclcpp::Node
{
public:
  DummyNode() : Node("dummy_node")
  {
    RCLCPP_INFO(this->get_logger(), "Dummy node started");
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DummyNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
