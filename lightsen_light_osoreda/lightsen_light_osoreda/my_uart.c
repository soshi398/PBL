/*
 * my_uart.c
 *
 * Created: 2018/09/14 14:50:27
 *  Author: yama
 */ 
#define F_CPU 16000000UL //マイコンの動作周波数(16MHz)

#include <stdio.h>

#include <avr/io.h>

 // シリアル通信の設定 (115.2kbps)
 void USART1_Init( void )
 {
	 UBRR1  = 8; // 動作周波数(Hz) / (ボーレート * 16) - 1
	 UCSR1A = 0b00000000;
	 UCSR1B = 0b00011000; // 送受信可
	 UCSR1C = 0b00000110; // データ8bit, 非同期, パリティなし, Stop 1bit
	 //UCSR1D = 0b00000010; // flow control : CTS有効
	 UCSR1D = 0b00000000; // flow control : CTS無効
 }

 // UART1へのバイトデータの送信
 void UART1_Byte_Send ( uint8_t data )
 {
	 while( ( UCSR1A & ( 1 << UDRE1 ) ) == 0 ); // 送信可フラグが立つまで待つ．
	 UDR1 = data;
 }

 // UART1からバイトデータの受信
 uint8_t UART1_Byte_Recv ( void )
 {
	 while( ( UCSR1A & 0b10000000 ) == 0 );  // 受信フラグが立つまで待つ．

	 return UDR1; // UART1の受信レジスタの内容を返す．
 }
 
 //printf相当の機能(C言語の可変長引数というテクを活用)
 void my_printf(char *msg, const char *format, ...) {

	int i;
	int  msg_len;

	va_list ap;
	
	//printfの書式に従って可変長引数を文字列としてmsgに格納．
	//格納した文字列の文字数を返す．
	va_start(ap, format);
	msg_len = vsprintf(msg, format, ap);
	va_end(ap);

	//文字数分，変換文字列の各文字をパソコンにシリアル送信
	for(i = 0; i < msg_len ; i++ ) UART1_Byte_Send(msg[i]);
}

// UART1への10bitのADCデータを送信(2バイト送信になります)．
void UART1_ADC_Data_Send ( uint16_t ad_data )
{
	while( ( UCSR1A & ( 1 << UDRE1 ) ) == 0 ); // 送信可フラグが立つまで待つ．
	UDR1 = ad_data; // 下位1バイトを送信
	
	while( ( UCSR1A & ( 1 << UDRE1 ) ) == 0 ); // 送信可フラグが立つまで待つ．
	UDR1 = ad_data >> 8; // 上位1バイトを送信	
}

