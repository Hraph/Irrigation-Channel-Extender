#include "Slave.hpp"

namespace App {
	Slave::Slave(){
		Communication::Bus::getInstance()->initialize();
	}

	void Slave::initialize(uint8_t channelsPins[]) {
		channels = channelsPins;
		initializePins(channelsPins, CHANNELS_COUNT, OUTPUT);
	}

	void Slave::start(uint8_t channelId) {
		if (currentChannelState && channels[channelId] != currentChannel) { // A different channel is currently active
			stop();
		}
		
		#ifdef DEBUG
		Serial.print("Start: ");
		Serial.print(channelId);
		Serial.print(" on pin ");
		Serial.println(channels[channelId]);
		#endif // DEBUG

		digitalWrite(channels[channelId], HIGH);

		currentChannel = channels[channelId];
		currentChannelState = true;
	}

	void Slave::stop() {
		#ifdef DEBUG
		Serial.print("Stop: ");
		Serial.println(currentChannel);
		#endif // DEBUG
			
		digitalWrite(currentChannel, LOW);
		currentChannelState = false;
	}
}