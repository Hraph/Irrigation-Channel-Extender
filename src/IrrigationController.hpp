#ifndef IRRIGATIONCONTROLLER_H
#define IRRIGATIONCONTROLLER_H

#include <Arduino.h>
#include "Config.hpp"

#define PULLUP_HIGH 0
#define PULLUP_LOW 1

#pragma once

namespace App {
    class IrrigationController {
    public:
			virtual void initialize(uint8_t channelsPins[]) = 0;
		  virtual void start(uint8_t channel) = 0;
      virtual void stop() = 0;
        
    protected:
			void initializePins(uint8_t pins[], uint8_t size, uint8_t mode) {
				for (uint8_t i = 0; i < size; ++i) {
					pinMode(pins[i], mode);
				}
			}
			uint8_t* channels;
			uint8_t currentChannel = 0;
			bool currentChannelState = false;
    };
}

#endif //IrrigationController.hpp