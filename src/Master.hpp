#ifndef MASTER_H
#define MASTER_H

#ifdef SLAVE
#define ICACHE_RAM_ATTR // No attribute for UNO Board
class Ticker { // Arduino UNO has no Ticker library
    public:
    void attach_ms(int, void (&)());
};
#endif

#include "IrrigationController.hpp"
#include "Commands.hpp"
#include "Bus.hpp"
#include "Config.hpp"
#include <Arduino.h>

#ifdef MASTER
#include <Ticker.h>
#endif // MASTER

#pragma once

namespace App {
    class Master : public Singleton<Master>, public App::IrrigationController {
        friend class Singleton<Master>; 
    public:
		void initialize(uint8_t channelsPins[]);
        void start(uint8_t channel);
        void stop();
        static void onInterrupt();
        static void onTickerEnd();
        void processChangePinStatus();
        Master();
        Ticker changedPinStatusTicker;
    };
}

#endif //Master.hpp