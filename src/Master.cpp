#include "Master.hpp"

namespace App {
	Master::Master(){
		Communication::Bus::getInstance()->initialize();
	}

	void Master::initialize(uint8_t* channelsPins) {
		channels = channelsPins;
		initializePins(channelsPins, CHANNELS_COUNT, INPUT_PULLUP);
		for (uint8_t i = 0; i < CHANNELS_COUNT; ++i)
		{
			attachInterrupt(digitalPinToInterrupt(channels[i]), Master::onInterrupt, CHANGE);	
		}
		processChangePinStatus(); // Initial check
	}

    void Master::start(uint8_t channel) {
			if (currentChannelState) { // A channel is currently active
				stop();
			}

			Serial.print("start ");
			Serial.println(channel);
			Communication::Bus::getInstance()->sendCommand(App::Commands::REQUEST_Start, true, channel);

			// If no error
			currentChannel = channel;
			currentChannelState = true;
    }

    void Master::stop() {
			Serial.println("stop");
			Communication::Bus::getInstance()->sendCommand(App::Commands::REQUEST_Stop);

			// If no error
			currentChannelState = false;
    }

	void ICACHE_RAM_ATTR Master::onInterrupt(){
		noInterrupts();
		Master::getInstance()->processChangePinStatus();
		interrupts();
	}

	void Master::processChangePinStatus(){
		for (uint8_t i = 0; i < CHANNELS_COUNT; ++i)
		{
			if (currentChannel == channels[i] && currentChannelState && digitalRead(channels[i]) == PULLUP_LOW) { // Detect current channel fall
				stop();
			}
			else if (currentChannel == channels[i] && !currentChannelState && digitalRead(channels[i]) == PULLUP_HIGH) { // Same channel changed: The currentChannel is now high
				start(channels[i]);
			}
			else if (currentChannel != channels[i] && digitalRead(channels[i]) == PULLUP_HIGH && digitalRead(currentChannel) == PULLUP_LOW){ // Channel changed: Old channel low and new channel activated
				start(channels[i]);
			}
		}
	}
}