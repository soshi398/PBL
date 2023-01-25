#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "my_lib.h"
void GoTo16MHz(void)
{
	CLKPR = 0x80;
	CLKPR = 0x00;
}










int main(void)
{
	GoTo16MHz();
	USART1_Init();
	AD_Init();
	uint8_t recv_data;

	DDRB |= 0x03; //PB１を出力に設定
	PORTB &= ~0x03; //PB１を０にリセット
	
	int light=5000;

	
	//ボードPC７（オンボードLED用）の向きを出力
	DDRC |= 0b10000000;
	PORTC &= ~0b10000000; //オンボードLEDを消灯
	PWM16_Init(4000); //PWM16の基本設定 周期は20ms(=40000?0.5us)


	/* Replace with your application code */
	while (1)
	{
		_delay_ms(500);
		PINC = 0b10000000; //オンボードLEDを反転

		recv_data = UART1_Byte_Recv();
		uint16_t ad_data[1];
		
		ad_data[0] = ADC_Read(0);
		
		for(int i = 0; i < 1; i++){
			UART1_Byte_Send(ad_data[i]);
			UART1_Byte_Send(ad_data[i] >>8);
		}
		
		if(recv_data == 0){
			light = light -100;
			
		}else if (recv_data == 1){
			light = light + 100;
		}else{
			light = light;
		}
		
		
		PWM16_PulseWidth(0b0001,light); //OC1Aのパルス幅を(=lightset?0.5us)
		PWM16_Start(0b0001) ;//OC1Aの出力開始





	}
}