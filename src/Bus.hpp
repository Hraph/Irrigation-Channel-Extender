#ifndef BUS_H
#define BUS_H

#include <Arduino.h>
#include <Wire.h>
#include "Commands.hpp"
#include "Singleton.hpp"
#include "Master.hpp"
#include "Slave.hpp"
#define SLAVE_ADDRESS 0x79
#define HAS_PARAMETER_MASK 0x8000


#pragma once

namespace Communication {
    class Bus : public Singleton<Bus> {
        friend class Singleton<Bus>; 
    public:
        void initialize();
        void processSlaveReceivedCommand(App::Commands command, bool hasParameter = false, short parameterValue = 0);
        void sendCommand(App::Commands command, bool hasParameter = false, short parameterValue = 0);
        static void slaveOnReceive(int size);
    protected:
    };
}

#endif //Bus.hpp