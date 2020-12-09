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

#ifndef MOTOMAN_HARDWARE__SIMPLE_MESSAGE_HPP_
#define MOTOMAN_HARDWARE__SIMPLE_MESSAGE_HPP_

namespace motoman_hardware::simple_message
{
#pragma pack(push, 1)

constexpr int ROS_MAX_JOINT = 10;
constexpr int MOT_MAX_GR = 4;

struct Prefix
{
  int length;
};

enum class MsgType
{
  MOTO_REALTIME_MOTION_JOINT_STATE_EX = 2030,
  MOTO_REALTIME_MOTION_JOINT_COMMAND_EX = 2031
};

enum class CommType
{
  INVALID = 0,
  TOPIC = 1,
  SERVICE_REQUEST = 2,
  SERVICE_REPLY = 3
};

enum class ReplyType
{
  INVALID = 0,
  SUCCESS = 1,
  FAILURE = 2
};

struct Header
{
  MsgType msg_type;
  CommType comm_type;
  ReplyType reply_type;
};

enum class MotoRealTimeMotionMode
{
  IDLE = 0,
  JOINT_POSITION = 1,
  JOINT_VELOCITY = 2
};

struct MotoRealTimeMotionJointStateExData
{
  int groupno;               // Robot/group ID;  0 = 1st robot
  float pos[ROS_MAX_JOINT];  // Feedback joint positions in radian. Base to Tool joint order
  float vel[ROS_MAX_JOINT];  // Feedback joint velocities in radian/sec.
};

struct MotoRealTimeMotionJointStateEx
{
  int message_id;  // Message id that the external control must echo back in the command
  MotoRealTimeMotionMode mode;
  int number_of_valid_groups;
  MotoRealTimeMotionJointStateExData joint_state_ex_data[MOT_MAX_GR];
};

struct MotoRealTimeMotionJointCommandExData
{
  int groupno;                   // Robot/group ID;  0 = 1st robot
  float command[ROS_MAX_JOINT];  // Command data. Either joint positions or velocities dependent on the current mode.
};

struct MotoRealTimeMotionJointCommandEx
{
  int message_id;  // Message id that the external control must echo back in the command
  int number_of_valid_groups;
  MotoRealTimeMotionJointCommandExData joint_command_ex_data[MOT_MAX_GR];
};

union Body
{
  MotoRealTimeMotionJointStateEx joint_state_ex;
  MotoRealTimeMotionJointCommandEx joint_command_ex;
};

struct SimpleMessage
{
  Prefix prefix;
  Header header;
  Body body;
};

#pragma pack(pop)

}  // namespace motoman_hardware::simple_message

#endif  // MOTOMAN_HARDWARE__SIMPLE_MESSAGE_HPP_
