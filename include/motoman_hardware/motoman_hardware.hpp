// Copyright 2020 Norwegian University of Science and Technology.
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

#ifndef MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_HPP_
#define MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_HPP_

#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/macros.hpp>

#include <angles/angles.h>
#include <hardware_interface/robot_hardware.hpp>
#include <hardware_interface/types/hardware_interface_return_values.hpp>

#include "hardware_interface/components/base_interface.hpp"
#include "hardware_interface/components/system_interface.hpp"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_status_values.hpp"


#include <motoman_hardware/visibility_control.h>

using hardware_interface::return_type;

namespace motoman_hardware
{
class MotomanHardware
  : public hardware_interface::components::BaseInterface<hardware_interface::components::SystemInterface>
{
public:

public:
  RCLCPP_SHARED_PTR_DEFINITIONS(MotomanHardware);

  return_type configure(const hardware_interface::HardwareInfo & info) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  return_type start() override;

  return_type stop() override;

  return_type read() override;

  return_type write() override;

private:
  // Dummy parameters
  double hw_start_sec_, hw_stop_sec_, hw_slowdown_;
  // Store the command for the simulated robot
  std::vector<double> hw_commands_, hw_states_;
};

}  // namespace motoman_hardware

#endif  // MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_HPP_