/*
 * Master_pj1.c
 *
 * Created: 2/27/2025 3:58:24 PM
 * Author : njfg0
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "I2C/I2C.h"
#include "lcd/lcd.h"

#define temp_adress 0x38
#define slave1 0x10
#define slave2 0x20


uint8_t bufferI2C;
float temperatura;
uint8_t temp_f;
char * temperatura_t;
uint8_t flag_sT=0;
uint8_t temp;
uint8_t data_Ttemp[6];
uint8_t estado_sT;
char cadena1_u[5];
char cadena1_d[5];

//iconos para LCD
uint8_t temp_icon[8]={
	0b00100,0b01010,0b01010,0b01010,0b01110,0b01110,0b01110,0b00100
};

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

float temp_converter(uint8_t *data){
	uint32_t rawTemp = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
	return (rawTemp * 200.0) / 1048576.0 - 50;
}

int main(void)
{
	cli();
	I2C_Master_Init(100000,1); // Inicializa I2C a 100KHz
	initLCD8b();
	sei();
	
	// Enviar comando de inicialización (si necesario)
	I2C_Master_Start();
	temp = I2C_Master_Write((temp_adress << 1) | 0);
	I2C_Master_Write(0xE1);
	I2C_Master_Stop();
	_delay_ms(10);

	//cargar icono
	LCD_Set_Icon(0,temp_icon);

	LCD_Set_Cursor(0,1);
	LCD_write_char(0);

	while (1)
	{
		PORTB |= (1 << PORTB4); // Enciende LED de debug

		// Enviar comando de medición
		I2C_Master_Start();
		temp = I2C_Master_Write((temp_adress << 1) | 0);
		if (temp != 1) {
			I2C_Master_Stop();
			PORTB &= ~(1 << PORTB4);
			continue;
		}

		I2C_Master_Write(0xAC);
		I2C_Master_Write(0x33);
		I2C_Master_Write(0x00);
		I2C_Master_Stop();

		// Esperar hasta que la medición esté lista
		uint8_t estado = 0;
		do {
			I2C_Master_Start();
			temp = I2C_Master_Write((temp_adress << 1) | 1);
			I2C_Master_Read(&estado, 1);
			I2C_Master_Stop();
		} while (estado & 0x80);

		// Leer datos
		I2C_Master_Start();
		temp = I2C_Master_Write((temp_adress << 1) | 1);
		for (uint8_t i = 0; i < 6; i++) {
			estado_sT = I2C_Master_Read(&data_Ttemp[i], (i < 5)); // ACK en los primeros 5
		}
		I2C_Master_Stop();

		// Convertir temperatura
		temperatura = temp_converter(data_Ttemp);
		temp_f = (int)temperatura;

		// Mostrar en LCD
		LCD_Set_Cursor(1,1);
		char salida[15];
		sprintf(salida, "%2d'C  ", temp_f);
		LCD_write_String(salida);

		PORTB &= ~(1 << PORTB4); // Apaga LED de debug
	}
}

