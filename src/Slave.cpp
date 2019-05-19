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
		if (currentChannelState) { // A channel is currently active
			stop();
		}

		digitalWrite(channel, HIGH);

		// If no error
		currentChannel = channel;
		currentChannelState = true;
	}

	void Slave::stop() {
		digitalWrite(currentChannel, LOW);
		currentChannelState = false;
	}
}