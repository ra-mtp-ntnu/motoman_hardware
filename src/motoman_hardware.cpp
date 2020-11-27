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

#include <motoman_hardware/motoman_hardware.hpp>

#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <rclcpp/rclcpp.hpp>

namespace motoman_hardware
{
return_type configure(const hardware_interface::HardwareInfo& info)
{
  return return_type::OK;
}

std::vector<hardware_interface::StateInterface> export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;
  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  return command_interfaces;
}

return_type start()
{
  return return_type::OK;
}

return_type stop()
{
  return return_type::OK;
}

return_type read()
{
  return return_type::OK;
}

return_type write()
{
  return return_type::OK;
}

}  // namespace motoman_hardware

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  motoman_hardware::MotomanHardware,
  hardware_interface::components::SystemInterface
)