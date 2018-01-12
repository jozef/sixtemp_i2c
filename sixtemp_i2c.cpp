/* see sixtemp_i2c.h */

#include <sixtemp_i2c.h>
#include <Arduino.h>
#include <Wire.h>

#define TRANSMISSION_REQUEST_DELAY 100

String sixtemp_i2c_temp::as_string() {
    if (has_error) {
        return "error";
    }

    if (!has_address) {
        return "n/a";
    }

    return String(int(temp_c))
        +'.'
        +String(int((temp_c - int(temp_c))*10))
        +" C";
}

String sixtemp_i2c_temp::address_as_string() {
    if (!has_address) {
        return "n/a";
    }

    char addr_as_char[2*sizeof(address)+1];
    addr_as_char[2*sizeof(address)+1-1] = '\0';
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t digit = address[i >> 1];
        digit = (i & 1 ? (digit & 0x0F) : (digit >> 4));
        addr_as_char[i] = (digit > 9 ? ('a'+digit-10) : ('0'+digit));
    }

    return String(addr_as_char);
}

sixtemp_i2c::sixtemp_i2c(uint8_t i2c_addr) {
    _i2c_addr = i2c_addr;
    count = 0;
    memset(temp_sensors,0,sizeof(temp_sensors));
}

void sixtemp_i2c::begin() {
    Wire.begin();
    refresh();
}

sixtemp_i2c_temp sixtemp_i2c::get_temp(uint8_t idx) {
    return temp_sensors[idx];
}

void sixtemp_i2c::refresh() {
    Wire.beginTransmission(_i2c_addr);
    Wire.write(0x60); // read sensors count
    Wire.endTransmission();
    delayMicroseconds(TRANSMISSION_REQUEST_DELAY);
    Wire.requestFrom(_i2c_addr, (uint8_t)1);
    count = Wire.read();
    if (count > 0x80) {
        count = 0;
        return;
    }
    if (count > MAX_SENSORS) {
        count = MAX_SENSORS;
    }
    for (uint8_t i = 0; i < count; i++) {
        Wire.beginTransmission(_i2c_addr);
        Wire.write(0x60+1+i); // set sensor idx
        Wire.endTransmission();
        delayMicroseconds(TRANSMISSION_REQUEST_DELAY);
        Wire.requestFrom(_i2c_addr, sizeof(temp_sensors[0]));
        for (uint8_t i2 = 0; i2 < sizeof(temp_sensors[0]); i2++) {
            if (!Wire.available()) {
                temp_sensors[i].has_error = true;
                temp_sensors[i].has_address = false;
                break;
            }
            char rch = Wire.read();
            *((char*)&temp_sensors[i] + i2) = rch;
        }
    }
    for (uint8_t i = count; i < MAX_SENSORS; i++) {
        if (temp_sensors[i].has_address) {
            temp_sensors[i].has_error = true;
        }
    }
}
