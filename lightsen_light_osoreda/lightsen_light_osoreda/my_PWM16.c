/*
 * my_PWM.c
 *
 * Created: 2018/09/19 12:28:10
 *  Author: yama
 */ 

// 4出力サポート (OC1A:PB5, OC1B:PB6, OC1C:PB7, OC3A:PC6)
// 周期は0～32.8msの間.

#define F_CPU 16000000UL //マイコンの動作周波数(16MHz)

#include <avr/io.h>

void PWM16_Init( uint16_t width ) // width = 周期/0.5us
                                  // 0   <= width <= 65535
								  // 0ms <= 周期  <= 32.8ms 
{
	//タイマ/カウンタ1制御レジスタA
	TCCR1A = 0b00000010;  //7-6: 00 (OC1Aの波形出力無し)
	                      //5-4: 00 (OC1Bの波形出力無し)
						  //3-2: 00 (OC1Cの波形出力無し)
						  //1-0: 10 (高速PWM動作:最大値ICR1)
	//タイマ/カウンタ1制御レジスタB
	TCCR1B = 0b00011010;  //7-5:  00 (未使用)
	                      //4-3:  11 (高速PWM動作:最大値ICR1)
	                      //2-0: 010 (分周1/8)
						  
	// 1周期分のカウント値 	  	
	ICR1 = width - 1; 
	
	//PB5(OC1A), PB6(OC1B), PB7(OC1C)の向きを出力
	DDRB |= 0b11100000;

	//タイマ/カウンタ3制御レジスタA
	TCCR3A = 0b00000010;  //7-6: 00 (OC3Aの波形出力無し)
 	                      //1-0: 10 (高速PWM動作:最大値ICR3)
						   
	//タイマ/カウンタ3制御レジスタB
	TCCR3B = 0b00011010;  //7-5:  00 (未使用)
	                      //4-3:  11 (高速PWM動作:最大値ICR3)
	                      //2-0: 010 (分周1/8)
	
	// 1周期分のカウント値
	ICR3 = width - 1;
	
	//ポートPC6(OC3A)の向きを出力
	DDRC |= 0b01000000;
}

// OC1A(0bit), OC1B(1bit), OC1C(2bit), OC3A(3bit)出力の
// パルス幅を整数値(width)で設定
// 時間幅 = width * 0.5us
void PWM16_PulseWidth( uint8_t ch, uint16_t width)
{
	if ( (ch & 0b0001) != 0 ) OCR1A = width;
	if ( (ch & 0b0010) != 0 ) OCR1B = width;
	if ( (ch & 0b0100) != 0 ) OCR1C = width;
	if ( (ch & 0b1000) != 0 ) OCR3A = width;
}

//PWM出力を開始
//OC1A(0bit)，OC1B(1bit), OC1C(2bit), OC3A(3bit)
void PWM16_Start( uint8_t ch )
{
	if ( (ch & 0b0001) != 0 ) 
		TCCR1A |= 0b10000000; //7-6: 10 (OC1AはコンペアマッチAでLOW)
		
	if ( (ch & 0b0010) != 0)
		TCCR1A |= 0b00100000; //5-4: 10 (OC1BはコンペアマッチBでLOW)

	if ( (ch & 0b0100) != 0)
		TCCR1A |= 0b00001000; //3-2: 10 (OC1CはコンペアマッチCでLOW)
	
	if ( (ch & 0b1000) != 0)
		TCCR3A |= 0b10000000; //7-6: 10 (OC3AはコンペアマッチAでLOW)	
}

//PWM出力を停止
//OC1A(0bit)，OC1B(1bit), OC1C(2bit), OC3A(3bit)
void PWM16_Stop( uint8_t ch )
{
	if ( (ch & 0b0001) != 0 )
	TCCR1A &= ~0b10000000; //7-6: 10 (OC1Aは出力無し)
	
	if ( (ch & 0b0010) != 0)
	TCCR1A &= ~0b00100000; //5-4: 10 (OC1Bは出力無し)

	if ( (ch & 0b0100) != 0)
	TCCR1A &= ~0b00001000; //3-2: 10 (OC1Cは出力無し)
	
	if ( (ch & 0b1000) != 0)
	TCCR3A &= ~0b10000000; //7-6: 10 (OC3Aは出力無し)
}