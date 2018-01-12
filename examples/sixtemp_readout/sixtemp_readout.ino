/*
  sixtemp_readout.ino - read sixtemp via i2c

  Created by Jozef Kutej, 30 December 2017.
  Released into the public domain.

  Example output on serial console:

    sensor count: 6
    sensor 1: n/a
    sensor 2: 28ff3ff6a1160322 19.8 C
    sensor 3: 286164122e7c499a 21.8 C
    sensor 4: 286164122f88f163 error
    sensor 5: 28ffa59492160558 19.8 C
    sensor 6: 286164122f89682a 21.5 C
    readouts count: 92, errors: 9

*/

#include <Arduino.h>
#include <sixtemp_i2c.h>
#include <Wire.h>

sixtemp_i2c stemp;

void setup () {
    Serial.begin(9600);
    while (Serial.available()) { Serial.read(); }

    stemp.begin();
}

void loop () {
    static uint32_t readouts = 0;
    static uint32_t readouts_err = 0;

    stemp.refresh();
    uint8_t readout_count = stemp.count;
    readouts++;
    if (!readout_count) readouts_err++;
    Serial.println("sensor count: "+String(readout_count));
    for (uint8_t i = 0; i < readout_count; i++) {
        sixtemp_i2c_temp temp = stemp.get_temp(i);

        Serial.print("sensor "+String(i+1)+": ");
        if (temp.has_address) {
            Serial.print(temp.address_as_string());
            Serial.print(' ');
        }
        Serial.println(temp.as_string());

        readouts++;
        if (temp.has_error) readouts_err++;
    }
    Serial.println("readouts count: "+String(readouts)+", errors: "+String(readouts_err));

    delay(3000);
}
