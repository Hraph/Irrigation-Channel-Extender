#ifndef MASTER_H
#define MASTER_H

#ifdef SLAVE
#define ICACHE_RAM_ATTR // No attribute for UNO Board
#endif

#include "IrrigationController.hpp"
#include "Commands.hpp"
#include "Bus.hpp"
#include "Config.hpp"
#include <Arduino.h>

#pragma once

namespace App {
    class Master : public Singleton<Master>, public App::IrrigationController {
        friend class Singleton<Master>; 
    public:
		void initialize(uint8_t channelsPins[]);
        void start(uint8_t channel);
        void stop();
        static void onInterrupt();
        void processChangePinStatus();
        Master();
    };
}

#endif //Master.hpp