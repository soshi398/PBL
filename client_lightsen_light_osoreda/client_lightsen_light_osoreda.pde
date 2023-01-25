import processing.net.*;
import processing.serial.*;

Serial MyPort;

Client myClient0;
Client myClient1;
Client myClient2;
Server myServer0;

byte keyval;

int State;
int aaaa= 3;
String Message;
String Message1="";
String Message2="";
int numRecvByte = 0;
byte[] RecvByteBuff = new byte[256];

String data_all0 = "";

void setup(){
size(400,200);
PFont font = createFont("BIZ UDゴシック",16,true);
textFont(font);
myClient0 = new Client(this,/*"192.168.9.1"*/ "localhost",2222);//光センサ（外）
myClient1 = new Client(this, /*"192.168.9.1"*/"localhost",4444);//LED

State = 0;
Message = " ";
int i;
for (i = 0; i < Serial.list().length; i++){
println(Serial.list()[i]);
}
println("are available");

MyPort = new Serial(this, Serial.list()[i - 1], 115200);

}

int []ADIntVal = new int [1];
float [] ADRealVal = new float[1];
void draw(){
background(#2D3986);
text("クライアントA",0,0);

text(Message ,20,20);
text(Message1 , 50,90);
text(Message2 , 20,110);
MyPort.write(aaaa);
println(aaaa);

//マイコンに送るデータ


text("CH0:" + nf(ADIntVal[0],4) + ", 実数値=" + nf(ADRealVal[0],1,3) + "[V]", 40,40);

}

void clientEvent(Client c){
int NumBytes = c.available();
byte[]myBuffer = c.readBytes(NumBytes);


switch( State ){

case 0:
Message = "サーバと接続完了" + str(myBuffer[0]);
State = 1;
break;
case 1:
if(c == myClient2){
//data_all0 = " ";
for(int i=0;i<NumBytes;i++){
byte keyval0;
keyval0 = myBuffer[i];
data_all0=data_all0+str(char(keyval0));

}
aaaa = Integer.parseInt(data_all0);
println(data_all0);

//MyPort.write(aaaa);


Message2 = "サーバから受信"+ data_all0;
data_all0 = "";
}
break;
}

c.clear();
}//サーバー側からデータ受信

void serialEvent (Serial RecvPort){
RecvByteBuff[numRecvByte]=(byte)RecvPort.read();
numRecvByte++;
if(numRecvByte == 2){
ADIntVal[0] = RecvByteBuff[1] << 8 | (RecvByteBuff[0] & 0xff);
ADRealVal[0] = (ADIntVal[0] / 1023.0 ) * 5.0;
numRecvByte = 0;



String sendData0 =String.valueOf(ADIntVal[0]);

//sendData = ADIntVal[0];
myClient0.write(sendData0);

delay (500);
}


}//サーバー側にデータ送信
