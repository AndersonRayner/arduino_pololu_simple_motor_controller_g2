#include "spool.h"

SPOOL::SPOOL(TwoWire& SPOOL_I2C, int SPOOL_LIMIT) :
  _SPOOL_I2C(SPOOL_I2C),
  _SPOOL_LIMIT(SPOOL_LIMIT)
{
    
}

// Functions for the spool drive
void SPOOL::init()
{
  // Set all the spool pins correctly
  // Software pullups not required as all done in hardware
  pinMode(_SPOOL_LIMIT,INPUT);

  // Spool initialised
  return;
  
}

void SPOOL::enable()
{

  // Required to allow motors to move.  Must be called when controller restarts and after any error.
  spool_command_data tx_buffer;

  tx_buffer.msg.command  = 0x83;  // Send the exit safe start code
  tx_buffer.msg.checksum = checksum(tx_buffer.msgData, sizeof(tx_buffer)-1);

  if (_debug >= 2)
  {
    SERIAL_DEBUG.print("SPOOL\n\t(enable): ");
  }
  
  // Send the data
  send_buffer(tx_buffer.msgData, sizeof(tx_buffer)-ignore_checksum);

  // Should be good to go now
  return;
}   

void SPOOL::send_buffer(uint8_t buf[], uint8_t len)
{
  if (_debug) { SERIAL_DEBUG.print("Sending "); printArray(buf,len); SERIAL_DEBUG.print("\n"); }

  _SPOOL_I2C.beginTransmission(_SPOOL_DRIVE_ADDR);
  _SPOOL_I2C.write(buf,len);                             
  _SPOOL_I2C.endTransmission();

  return;
}

void SPOOL::brake()
{
  // Debugging
  if (_debug >= 2)
  {
    SERIAL_DEBUG.print("SPOOL\n\t(brake): ");
  }
  
  // Brakes the spool motor at maximum force
  spool_brake_data tx_buffer;

  tx_buffer.msg.header      = 0x92;
  tx_buffer.msg.brake_force = 0x20; // max 0x20
  tx_buffer.msg.checksum    = checksum(tx_buffer.msgData, sizeof(tx_buffer));

  // Send the command over I2C
  send_buffer(tx_buffer.msgData, sizeof(tx_buffer)-ignore_checksum);

  // Return
  return;
  
}

void SPOOL::set_speed(float drive_speed)
{    
  // We expect to take in a value of [-1 1] and will map that to [-3200 and 3200] which the motor driver expects
   
  // Debugging
  if (_debug >= 2)
  {
    SERIAL_DEBUG.print("SPOOL\n\t(set_speed): ");
  }

  // Apply limits
  drive_speed = apply_limits(drive_speed);

  // Assemble the drive packet
  spool_drive_data tx_buffer;
  
  if (drive_speed < 0.0f)
  {
    tx_buffer.msg.header = 0x86; // Motor reverse
    drive_speed = -drive_speed;
    
  } else {
    tx_buffer.msg.header = 0x85; // Motor forward
    
  }

  // =============== Map the drive speed here =====================
  drive_speed = drive_speed * 3200;

   int16_t _speed = (int16_t) drive_speed;


  tx_buffer.msg.drive_speed = 0x1F08;
  tx_buffer.msg.drive_speed = _speed;

  // Override speed command because the other thing isn't going
  tx_buffer.msgData[1] = (_speed & 0x1F);
  tx_buffer.msgData[2] = (_speed >> 5 & 0x7F);

  tx_buffer.msg.checksum = checksum(tx_buffer.msgData, sizeof(tx_buffer)-1);



  // Send the command over I2C
  send_buffer(tx_buffer.msgData, sizeof(tx_buffer)-ignore_checksum);

  // All done
  return;
  
}

void SPOOL::check_limit()
{
    // Stops the spool if it's on the limit switch
    if (limit_switch())
    {        
            brake();

    }
    
    return;
}

float SPOOL::apply_limits(float drive_speed)
{
  // Limit the drive speed to [-1 1]
  drive_speed = constrain(drive_speed,-_max_speed,_max_speed);

  // Limit to only drive out if on the limit switch [-1, 0]
  if (limit_switch())
  {
    drive_speed = constrain(drive_speed,-_max_speed,0.0f);
  }  

  return (drive_speed);
    
}

void SPOOL::set_max_speed(float max_speed)
{
    _max_speed = abs(max_speed);
    return;
}

bool SPOOL::limit_switch()
{
    return (0);
    return (digitalRead(_SPOOL_LIMIT));
}

uint8_t SPOOL::checksum(uint8_t buf[], uint8_t len)
{
    
  // Code taken from https://www.pololu.com/docs/0J77/8.13
  uint8_t ii, jj, crc = 0;

  // Calculate the checksum
  for (ii = 0; ii < len; ii++)
  {
    crc ^= buf[ii];
    for (jj = 0; jj < 8; jj++)
    {
      if (crc & 1)
        crc ^= 0x91;  // CRC7_POLY, Section 6.5 of G2 User Guide
      crc >>= 1;
    }
  }
  return (crc);
}

void SPOOL::printArray(uint8_t buf[], uint8_t len)
{
  // Prints a buffer in HEX
  for (uint8_t ii = 0; ii < len; ii++)
  {
    SERIAL_DEBUG.print("0x"); 
    
    if (buf[ii] < 0x10)
    {
      SERIAL_DEBUG.print("0"); SERIAL_DEBUG.print(buf[ii],HEX); SERIAL_DEBUG.print(" ");
    } else {
      SERIAL_DEBUG.print(buf[ii],HEX); SERIAL_DEBUG.print(" ");
    }
  }
  
  SERIAL_DEBUG.print("\n");

  return;

}
