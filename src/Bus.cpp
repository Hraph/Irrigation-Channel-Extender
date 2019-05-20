#include "Bus.hpp"

namespace Communication {
	void Bus::slaveOnReceive(int size){
        bool nextIsHigherByte = true; // First receive is higher
        bool nextReceiveIsParameter = false;
        App::Commands latestReceivedCommand;
        short data = 0;

        while (Wire.available() > 0){
            char d = (char) Wire.read();

			if (nextIsHigherByte) {
				data = ((d & 0xFF) << 8); // Reset and keep higher byte
				nextIsHigherByte = false;
			}

			else { // Lower byte: Got complete short
				data |= (d & 0xFF); // Keep lower byte
				nextIsHigherByte = true;

                if (!nextReceiveIsParameter && (data & HAS_PARAMETER_MASK) != 0) { // Command has parameter: save for next receive
                    latestReceivedCommand = (App::Commands) (data & ~HAS_PARAMETER_MASK); // Save command for next receive
                    nextReceiveIsParameter = true;
                }
                else if (nextReceiveIsParameter) { // Process command with parameter
                    Bus::getInstance()->processSlaveReceivedCommand(latestReceivedCommand, true, data);
                    nextReceiveIsParameter = false;
                }
                else { // Command without parameter
                    Bus::getInstance()->processSlaveReceivedCommand((App::Commands) (data & ~HAS_PARAMETER_MASK));
                }
			}
        }
    }

    void Bus::initialize() {
        #ifdef MASTER
        Wire.begin(); // join i2c bus (address optional for master)
        #endif // DEBUG

        #ifdef SLAVE
        Wire.begin(SLAVE_ADDRESS);
        Wire.onReceive(Bus::slaveOnReceive);
        #endif // DEBUG
    }

    bool Bus::sendCommand(App::Commands command, bool hasParameter, short parameterValue){
        Wire.beginTransmission(SLAVE_ADDRESS); // Transmit to device

        int cmd = command;
        if (hasParameter)
            cmd = (command | HAS_PARAMETER_MASK);

		Wire.write(cmd >> 8); // Send higher
        Wire.write(cmd & 0xFF); // Send lower

        if (hasParameter) {
			Wire.write(parameterValue >> 8); // Send higher
            Wire.write(parameterValue & 0xFF); // Send lower
        }
        
		#ifdef DEBUG
				Serial.println("Sending command");
		#endif // DEBUG

        char wireStatus = Wire.endTransmission();

		// TODO handle errors

		return (wireStatus == 0);
    }

    #ifdef MASTER
    void Bus::processSlaveReceivedCommand(App::Commands command, bool hasParameter, short parameterValue){
        switch (command)
        {
        case App::Commands::REPLY_Error:
            Serial.println("Received ERROR response for command");
            Serial.println((App::Commands) (command & ~HAS_PARAMETER_MASK));
            break;

        case App::Commands::REPLY_Ok:
            Serial.println("Received OK response for command");
            Serial.println((App::Commands) (command & ~HAS_PARAMETER_MASK));
            break;
        }
    }
    #endif // MASTER

    #ifdef SLAVE
    void Bus::processSlaveReceivedCommand(App::Commands command, bool hasParameter, short parameterValue){
		#ifdef DEBUG
		Serial.print("Received command: ");
		Serial.print((App::Commands) command);

		Serial.print(" - with parameter: ");
		Serial.println(parameterValue);
		#endif // DEBUG

        switch (command)
        {
        case App::Commands::REQUEST_Start:
            App::Slave::getInstance()->start(parameterValue);
            break;

        case App::Commands::REQUEST_Stop:
            App::Slave::getInstance()->stop();
            break;
        }
    }
    #endif // SLAVE
}