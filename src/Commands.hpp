#ifndef COMMANDS_H
#define COMMANDS_H

namespace App {
    enum Commands {
        REQUEST_Status = 0,
        REQUEST_Start,
        REQUEST_Stop,
        REQUEST_Temperature,
        REQUEST_Humidity,
        REQUEST_FlowSensor,
        REPLY_Ok,
        REPLY_Error
    };
}

#endif //Commands.hpp
