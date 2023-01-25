/*
 * my_ADC.c
 *
 * Created: 2018/09/14 16:58:57
 *  Author: yama
 */ 

#define F_CPU 16000000UL //マイコンの動作周波数(16MHz)

#include <avr/io.h>

void AD_Init( void )
{
	// ADCの初期化
	ADMUX = 0 ;// 電源5V(AVref)を使用, データ右詰め
	ADCSRA |= _BV(ADPS2) | _BV(ADPS0); // ADEN, ADSC, CK/32
}

void selADchannel(uint8_t x)
{
	uint8_t tmp ;
	
	ADCSRA &= ~_BV(ADEN); // ADCを禁止
	
	/* judge */
	if ( x > 15 ) return ;
	/* get now mode */
	tmp = ADMUX ;
	/* clear multiplexer */
	tmp &= 0xf0 ;
	/* select new channel */
	ADMUX = tmp | x ;
	
	ADCSRA |= _BV(ADEN); // ADCを許可
}

uint16_t ADC_Read( uint8_t ch_num )
{
	selADchannel(ch_num); // ADCのチャンネルを選択
	ADCSRA |= _BV(ADSC);     // ADをスタート
	while( (ADCSRA & _BV(ADIF)) == 0 ); // AD変換の終了を待つ
	ADCSRA &= ~_BV(ADIF); // 終了フラグのリセット
	return ( ADC & 0x3ff ); // AD変換結果を返す．
}

