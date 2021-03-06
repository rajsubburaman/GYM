/*
 * Copyright (C) 2014 Walkman
 * Author: Mirko Ferrati
 * email:  mirko.ferrati@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#ifndef _YARP_SWITCH_INTERFACE_H_
#define _YARP_SWITCH_INTERFACE_H_
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <vector>
#include <iostream>
#include <yarp/os/RateThread.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <mutex>
#include "internal_yarp_command_interface.hpp"

namespace walkman
{

    class yarp_switch_sender_interface:public internal_yarp_command_sender_interface<std::string>
    {
    public:
        yarp_switch_sender_interface(const std::string& module_prefix):internal_yarp_command_sender_interface< std::string >(module_prefix,"/switch")
        {

        }
    };


    class yarp_switch_interface:public internal_yarp_command_interface<std::string>
    {
    public:
        yarp_switch_interface(const std::string& module_prefix):internal_yarp_command_interface< std::string >(module_prefix,"/switch:i")
        {

        }
    };

}
#endif
