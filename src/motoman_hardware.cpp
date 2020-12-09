// Copyright 2019 Norwegian University of Science and Technology.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>

#include <motoman_hardware/motoman_hardware.hpp>

namespace motoman_hardware
{
return_type MotomanHardware::configure(const hardware_interface::HardwareInfo& info)
{
  if (configure_default(info) != return_type::OK)
  {
    return return_type::ERROR;
  }

  hw_start_sec_ = stod(info_.hardware_parameters["example_param_hw_start_duration_sec"]);
  hw_stop_sec_ = stod(info_.hardware_parameters["example_param_hw_stop_duration_sec"]);
  hw_slowdown_ = stod(info_.hardware_parameters["example_param_hw_slowdown"]);

  positions_.resize(info_.joints.size());
  velocities_.resize(info_.joints.size());
  commands_.resize(info_.joints.size());
  for (uint i = 0; i < info_.joints.size(); i++)
  {
    positions_[i] = std::numeric_limits<double>::quiet_NaN();
    velocities_[i] = std::numeric_limits<double>::quiet_NaN();
    commands_[i] = std::numeric_limits<double>::quiet_NaN();
  }

  {
    for (const hardware_interface::ComponentInfo& joint : info_.joints)
    {
      if (joint.command_interfaces.size() != 1)
      {
        RCLCPP_FATAL(rclcpp::get_logger("MotomanHardware"), "Joint '%s' has %d command interfaces found. 1 expected.",
                     joint.name.c_str(), joint.command_interfaces.size());
        return return_type::ERROR;
      }

      if (joint.command_interfaces[0].name != hardware_interface::HW_IF_VELOCITY)
      {
        RCLCPP_FATAL(rclcpp::get_logger("MotomanHardware"),
                     "Joint '%s' have %s command interfaces found. '%s' expected.", joint.name.c_str(),
                     joint.command_interfaces[0].name.c_str(), hardware_interface::HW_IF_VELOCITY);
        return return_type::ERROR;
      }

      if (joint.state_interfaces.size() != 2)
      {
        RCLCPP_FATAL(rclcpp::get_logger("MotomanHardware"), "Joint '%s' has %d state interface. 2 expected.",
                     joint.name.c_str(), joint.state_interfaces.size());
        return return_type::ERROR;
      }

      if (joint.state_interfaces[0].name != hardware_interface::HW_IF_POSITION)
      {
        RCLCPP_FATAL(rclcpp::get_logger("MotomanHardware"), "Joint '%s' have %s state interface '%s' expected.",
                     joint.name.c_str(), joint.state_interfaces[0].name.c_str(), hardware_interface::HW_IF_POSITION);
        return return_type::ERROR;
      }

      if (joint.state_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
      {
        RCLCPP_FATAL(rclcpp::get_logger("MotomanHardware"), "Joint '%s' have %s state interface. '%s' expected.",
                     joint.name.c_str(), joint.state_interfaces[1].name.c_str(), hardware_interface::HW_IF_VELOCITY);
        return return_type::ERROR;
      }
    }
  }

  udp_ip_address_ = info_.hardware_parameters["udp_ip_address"];
  udp_port_ = stoi(info_.hardware_parameters["udp_port"]);
  udp_server_socket_.bind(Poco::Net::SocketAddress(udp_ip_address_, udp_port_));

  status_ = hardware_interface::status::CONFIGURED;
  return return_type::OK;
}

std::vector<hardware_interface::StateInterface> MotomanHardware::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;
  for (uint i = 0; i < info_.joints.size(); i++)
  {
    state_interfaces.emplace_back(
        hardware_interface::StateInterface(info_.joints[i].name, hardware_interface::HW_IF_POSITION, &positions_[i]));
    state_interfaces.emplace_back(
        hardware_interface::StateInterface(info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocities_[i]));
  }

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> MotomanHardware::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  for (uint i = 0; i < info_.joints.size(); i++)
  {
    command_interfaces.emplace_back(
        hardware_interface::CommandInterface(info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &commands_[i]));
  }

  return command_interfaces;
}

return_type MotomanHardware::start()
{
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Starting ...please wait...");

  for (int i = 0; i <= hw_start_sec_; i++)
  {
    rclcpp::sleep_for(std::chrono::seconds(1));
    RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "%.1f seconds left...", hw_start_sec_ - i);
  }

  // set some default values
  for (uint i = 0; i < positions_.size(); i++)
  {
    if (std::isnan(positions_[i]))
    {
      positions_[i] = 0.0;
      velocities_[i] = 0.0;
      commands_[i] = 0.0;
    }
  }

  status_ = hardware_interface::status::STARTED;

  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "System Sucessfully started!");

  return return_type::OK;
}

return_type MotomanHardware::stop()
{
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Stopping ...please wait...");

  for (int i = 0; i <= hw_stop_sec_; i++)
  {
    rclcpp::sleep_for(std::chrono::seconds(1));
    RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "%.1f seconds left...", hw_stop_sec_ - i);
  }

  status_ = hardware_interface::status::STOPPED;

  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "System sucessfully stopped!");

  return return_type::OK;
}

return_type MotomanHardware::read()
{
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Reading...");

  // udp_server_socket_.receiveFrom()

  for (uint i = 0; i < hw_states_.size(); i++)
  {
    // Simulate RRBot's movement
    hw_states_[i] = hw_commands_[i] + (hw_states_[i] - hw_commands_[i]) / hw_slowdown_;
    RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Got state %.5f for joint %d!", hw_states_[i], i);
  }
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Joints sucessfully read!");

  return return_type::OK;
}

return_type MotomanHardware::write()
{
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Writing...");

  for (uint i = 0; i < hw_commands_.size(); i++)
  {
    // Simulate sending commands to the hardware
    RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Got command %.5f for joint %d!", hw_commands_[i], i);
  }
  RCLCPP_INFO(rclcpp::get_logger("MotomanHardware"), "Joints sucessfully written!");
  return return_type::OK;
}

}  // namespace motoman_hardware

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(motoman_hardware::MotomanHardware, hardware_interface::SystemInterface)