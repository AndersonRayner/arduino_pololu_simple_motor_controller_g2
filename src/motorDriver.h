// Library for reading the comms coming from the companion computer
//
// Matt Anderson, 2020

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>

#include "data_structures.h"

#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG Serial
#endif

class motorDriver {
    public :
        // Constructor
        motorDriver(TwoWire& i2c_bus);
        
        // Functions
        void init();
     
        void enable(); 
        void disable();
        void brake();
        
        bool limit_switch();

        void set_speed(float drive_speed);
        void set_max_speed(float max_speed);

        float get_cmd() { return last_cmd_; }

        void invert() { invert_ = !invert_; }
        
        // Variables
        uint8_t _debug = 0;
        
    private :
        const uint8_t _SPOOL_DRIVE_ADDR = 13;
            
        TwoWire& _i2c_bus;
        
        void check_limit();
        float apply_limits(float drive_speed);
        
        float _max_speed = 1.0f;
        float last_cmd_ = 0.0f;
        bool invert_ = false;

        // Data transmission
        void send_buffer(uint8_t buf[], uint8_t len);
        
        // Checksumming
        uint8_t ignore_checksum = 1;
        uint8_t checksum(uint8_t buf[], uint8_t len);

        // Debugging
        void printArray(uint8_t buf[],uint8_t len);
};
