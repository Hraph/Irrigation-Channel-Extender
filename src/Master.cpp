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

    void Master::start(uint8_t channelId) {
		if (channels[channelId] == currentChannel && currentChannelState) // No need to send
			return;

		#ifdef DEBUG
		Serial.print("Start: ");
		Serial.print(channelId);
		Serial.print(" on pin ");
		Serial.println(channels[channelId]);
		#endif // DEBUG
		
		if (Communication::Bus::getInstance()->sendCommand(App::Commands::REQUEST_Start, true, channelId)) { // If no error
			currentChannel = channels[channelId];
			currentChannelState = true;
		}
    }

    void Master::stop() {
			#ifdef DEBUG
			Serial.print("Stop: ");
			Serial.println(currentChannel);
			#endif // DEBUG

			if (Communication::Bus::getInstance()->sendCommand(App::Commands::REQUEST_Stop)) // If no error
				currentChannelState = false;
    }

	void ICACHE_RAM_ATTR Master::onInterrupt(){
		noInterrupts();
		Master::getInstance()->changedPinStatusTicker.attach_ms(INTERRUPT_THRESHOLD_DELAY, Master::onTickerEnd);
		interrupts();
	}

	void Master::onTickerEnd(){
		Master::getInstance()->processChangePinStatus();
	}

	void Master::processChangePinStatus(){
		for (uint8_t i = 0; i < CHANNELS_COUNT; ++i)
		{
			if (currentChannel == channels[i] && currentChannelState && digitalRead(channels[i]) == PULLUP_LOW) { // Detect current channel fall
				stop();
			}
			else if (currentChannel == channels[i] && !currentChannelState && digitalRead(channels[i]) == PULLUP_HIGH) { // Same channel changed: The currentChannel is now high
				start(i);
			}
			else if (currentChannel != channels[i] && digitalRead(channels[i]) == PULLUP_HIGH && digitalRead(currentChannel) == PULLUP_LOW){ // Channel changed: Old channel low and new channel activated
				start(i);
			}
		}
	}
}