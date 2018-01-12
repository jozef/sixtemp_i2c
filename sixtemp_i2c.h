/* VERSION 0.01; 1.1.2018; see below for description and documentation */
#ifndef sixtemp_i2c_h
#define sixtemp_i2c_h

#include <Arduino.h>

#define MAX_SENSORS 6
#define DEFAULT_I2C_ADDR 0x18

class sixtemp_i2c_temp {
    public:
        bool has_error;
        bool has_address;
        float temp_c;
        char address[8];
        String as_string();
        String address_as_string();
};

class sixtemp_i2c {
    public:
        sixtemp_i2c(uint8_t i2c_addr = DEFAULT_I2C_ADDR);
        void begin();
        sixtemp_i2c_temp get_temp(uint8_t idx);
        void refresh();
        uint8_t count;
        sixtemp_i2c_temp temp_sensors[MAX_SENSORS];
    private:
        uint8_t _i2c_addr;
};

#endif

/*

=head1 NAME

    sixtemp_i2c.h - client library for sixtemp sensor

=head1 SYNOPSIS

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

=head1 DESCRIPTION

This is a client library for sixtemp sensor L<https://github.com/jozef/sixtemp_sensor>

=head1 INSTALL

Clone the repository folder into F<sketchbook/libraries/sixtemp_i2c>

=head1 CLASS

=head2 class sixtemp_i2c

=head3 sixtemp_i2c(uint8_t i2c_addr = DEFAULT_I2C_ADDR);

Class constructor.

=head3 void begin();

Start i2c.

=head3 sixtemp_i2c_temp get_temp(uint8_t idx);

Return L</class sixtemp_i2c_temp> for temperature at C<idx>.

=head3 void refresh();

Refresh all temperature readings from sixtemp sensor via i2c.

=head3 uint8_t count;

Number of temperatures present.

=head3 sixtemp_i2c_temp temp_sensors[MAX_SENSORS];

Array of all L</class sixtemp_i2c_temp>.

=head2 class sixtemp_i2c_temp

Represents one temperature. Has following public variables and methods:

=head3 bool has_error;

true/false if there was an error

=head3 bool has_address;

true/false if there temp position has address assigned

=head3 float temp_c;

temperature

=head3 String as_string();

temp_c as String, ex.: "21.8 C"

=head3 char address[8];

Temperature sensors 8 byte address.

=head3 String address_as_string();

address as String, ex.: "28ff3ff6a1160322"

=head1 EXAMPLES

=head2 examples/sixtemp_readout/sixtemp_readout.ino

Example serial output when uploaded to an Arduino connected to sixtemp_sensor
via i2c:

    sensor count: 6
    sensor 1: n/a
    sensor 2: 28ff3ff6a1160322 19.8 C
    sensor 3: 286164122e7c499a 21.8 C
    sensor 4: 286164122f88f163 error
    sensor 5: 28ffa59492160558 19.8 C
    sensor 6: 286164122f89682a 21.5 C
    readouts count: 92, errors: 9

=head1 SEE ALSO

L<https://github.com/jozef/sixtemp_sensor>

=head1 LICENSE

This is free software, licensed under the MIT License.

=head1 AUTHOR

    Jozef Kutej

=cut

*/
