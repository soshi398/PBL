/*
 * my_lib.h
 *
 * Created: 2018/09/19 13:05:45
 *  Author: yama
 */ 


#ifndef MY_LIB_H_
#define MY_LIB_H_

//==================================================
// シリアル通信(UART1)関連
//==================================================
#define CTS_ON  1
#define CTS_OFF 0

void USART1_Init( );

// UART1へのバイトデータの送信
void UART1_Byte_Send ( uint8_t );

// UART1からバイトデータの受信
uint8_t UART1_Byte_Recv ( void );

void my_printf(char *msg, const char *format, ...);

uint16_t ADC_Read( uint8_t ); //AD変換データ(10bit)の送信
//==================================================

//==================================================
// AD変換器関連
//==================================================
void     AD_Init     ( void    );
void     selADchannel( uint8_t );
uint16_t ADC_Read    ( uint8_t );
//==================================================

//==================================================
// PWM16関連
//==================================================
void PWM16_Init( uint16_t );
void PWM16_PulseWidth( uint8_t, uint16_t);
void PWM16_Start( uint8_t);
void PWM16_Stop( uint8_t ch );
//==================================================

//==================================================
// 汎用ポート関連(PBとPDがごちゃ混ぜ)
//==================================================
void PortDirection(uint8_t, uint8_t);
void PortWriteBit (uint8_t, uint8_t);
//==================================================

#endif /* MY_LIB_H_ */