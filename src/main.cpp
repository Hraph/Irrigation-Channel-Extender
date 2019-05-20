#include <Arduino.h>
#include "Config.hpp"
#include "Master.hpp"
#include "Slave.hpp"

#ifdef MASTER
App::Master *master;  
uint8_t channels[] = {MASTER_CHANNEL_PIN_1, MASTER_CHANNEL_PIN_2, MASTER_CHANNEL_PIN_3, MASTER_CHANNEL_PIN_4, MASTER_CHANNEL_PIN_5, MASTER_CHANNEL_PIN_6};
#endif // MASTER

#ifdef SLAVE
App::Slave *slave; 
uint8_t channels[] = {SLAVE_CHANNEL_PIN_1, SLAVE_CHANNEL_PIN_2, SLAVE_CHANNEL_PIN_3, SLAVE_CHANNEL_PIN_4, SLAVE_CHANNEL_PIN_5, SLAVE_CHANNEL_PIN_6};
#endif // SLAVE

void setup() {
	#ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Starting");
	#endif // DEBUG

	#ifdef MASTER
	master = App::Master::getInstance();
	master->initialize(channels);
	#endif // MASTER

	#ifdef SLAVE
	slave = App::Slave::getInstance();
	slave->initialize(channels);
	#endif // SLAVE
}

void debugPinState(uint8_t pins[], int delayMs){
	for (uint8_t i = 0; i < CHANNELS_COUNT; ++i)
		{
			Serial.print(digitalRead(pins[i]) == PULLUP_HIGH, 2);
		}
	Serial.println();
	delay(delayMs);
}

void loop() {
}
