all: README

README: sixtemp_i2c.h
	pod2readme $< README
