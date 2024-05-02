
#include <Wire.h>
#include "spool.h"


SPOOL spool(Wire, 13);

void setup() {

  // Set up the debug port
  Serial.begin(115200);
  Serial.print("Spool Drive Test Program\n");

  Wire.begin();

  // Set up the spool
  spool.init();
  spool._debug = 2;
  spool.set_max_effort(0.5f);
  
  spool.enable();
  
}

void loop() {
  
  // Drive the motor at lots of different speeds
  for (uint8_t ii = 1; ii<10; ii++)
  {
    spool.set_effort(0.1f*ii);
    delay(2000);
    spool.set_effort(-0.1f*ii);
    delay(2000);
    spool.brake();
    delay(2000);

  }
    
}
