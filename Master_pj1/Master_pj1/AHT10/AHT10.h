/*
 * AHT10.h
 *
 * Created: 2/27/2025 4:19:33 PM
 *  Author: njfg0
 */ 


#ifndef AHT10_H_
#define AHT10_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

void init_sensor_temp();




#endif /* AHT10_H_ */