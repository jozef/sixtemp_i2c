/*
  synopsis.ino - read sixtemp via i2c

  Example output from synopsis section code:

    sensor count: 6
    temp: 286164122f910528 29.6 C
*/

#include <Arduino.h>
#include <sixtemp_i2c.h>
#include <Wire.h>

sixtemp_i2c stemp(0x18);

void setup () {
    Serial.begin(9600);
    stemp.begin();
}

void loop () {
    stemp.refresh();
    Serial.println("sensor count: "+String(stemp.count));
    if (stemp.count) {
        sixtemp_i2c_temp temp = stemp.get_temp(0);
        Serial.println("temp: "+temp.address_as_string()+" "+temp.as_string());
    }

    delay(3000);
}
