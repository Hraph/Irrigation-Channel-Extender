#include "Slave.hpp"

namespace App {
	Slave::Slave(){
		Communication::Bus::getInstance()->initialize();
	}

	void Slave::initialize(uint8_t channelsPins[]) {
		channels = channelsPins;
		initializePins(channelsPins, CHANNELS_COUNT, OUTPUT);
	}

	void Slave::start(uint8_t channel) {
		if (currentChannelState && channel != currentChannel) { // A different channel is currently active
			stop();
		}
		
		#ifdef DEBUG
		Serial.print("Start: ");
		Serial.println(channel);
		#endif // DEBUG

		digitalWrite(channel, HIGH);

		currentChannel = channel;
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