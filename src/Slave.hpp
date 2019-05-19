#ifndef SLAVE_H
#define SLAVE_H

#include "IrrigationController.hpp"
#include "Commands.hpp"
#include "Bus.hpp"
#include "Config.hpp"
#include <Arduino.h>

#pragma once

namespace App {
    class Slave : public Singleton<Slave>, public App::IrrigationController {
        friend class Singleton<Slave>; 
    public:
        Slave();
		void initialize(uint8_t channelsPins[]);
        void start(uint8_t channel);
        void stop();
    };
}

#endif //Slave.hpp