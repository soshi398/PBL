import processing.net.*;
import controlP5.*;
import java.util.Calendar;

ControlP5 cp5;
int lightset;

int hitokanti=0;
int lightset_result=9999;
int uti;
int soto;

int []startTime = new int[2];


String setteijikoku="";
String MessageO="";
int hour, minute;

final int iti_x = 400;
final int iti_y = 500;



final int MAX_CLIENT =5;





Server[] myServer = new Server[MAX_CLIENT];
String[]ServerIP = new String[MAX_CLIENT];
int []PortNum = new int[MAX_CLIENT];



Client[]myClient =  new Client[MAX_CLIENT];
String[] ClientIP = new String[MAX_CLIENT];

int State;
String[] Message = new String[MAX_CLIENT];
String[] Message2 = new String[MAX_CLIENT];

int numClient;

void setup() {
  size(1000, 700);

  startTime[0]=99;
  startTime[1]=99;

  PFont font = createFont("BIZ UDゴシック", 20, true);
  textFont(font);

  PortNum[0] = 1111;//光センサ（内）
  PortNum[1] = 2222;//光センサ（外）
  PortNum[2] = 3333;//人感センサ
  PortNum[3] = 4444;//LED
  PortNum[4] = 5555;//モーター
  for (int i=0; i<MAX_CLIENT; i++) {
    myServer[i] = new Server(this, PortNum[i]);
    myClient[i] = new Client(this, "192.168.9.2", PortNum[i]);
  }

  for (int i = 0; i < MAX_CLIENT; i++) {
    ServerIP[i] = myServer[i].ip() +"#" +PortNum[i];
    ClientIP[i] = " ";
    Message[i] = " ";
    Message2[i]=" ";
  }
  State = 0;
  numClient = 0;


  cp5=new ControlP5(this);
  ControlFont cf1 = new ControlFont(createFont("BIZ UDゴシック", 20));

  cp5.addButton ("schedule")
    .setLabel("スケジュールを設定")
    .setFont(cf1)
    .setPosition(50+iti_x, 700-iti_y)
    .setSize(240, 40);



  cp5.addTextfield("startH")
    .setLabel("")
    .setFont(cf1)
    .setPosition(50+iti_x, 600-iti_y)
    .setSize(40, 40)
    .setAutoClear(false);


  cp5.addTextfield("startM")
    .setLabel("")
    .setFont(cf1)
    .setPosition(130+iti_x, 600-iti_y)
    .setSize(40, 40)
    .setAutoClear(false);


  cp5.addSlider("lightset")
    .setRange(0, 100)
    .setValue(50)
    .setPosition(40+iti_x, 900-iti_y)
    .setSize(200, 30)
    .setNumberOfTickMarks(101);


  cp5.addButton ("Lset")
    .setLabel("明るさを設定")
    .setFont(cf1)
    .setPosition(40+iti_x, 950-iti_y)
    .setSize(240, 40);
}

void draw() {


  hour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
  minute = Calendar.getInstance().get(Calendar.MINUTE);

  background(#086C52);
  for (int i = 0; i < MAX_CLIENT; i++) {
    text("サーバー"+ ServerIP[i]+ ",状態:"+ State, 30, 20+i*100);
    text("クライアント:"+ClientIP[i], 30, 40+i * 100);
    text(Message[i], 30, 60+i*100);
    text(Message2[i], 30, 80+i*100);
    text("起床時刻", 50+iti_x, 580-iti_y);
    text("時", 100+iti_x, 625-iti_y);
    text("分", 180+iti_x, 625-iti_y);
    if (h ==99 | m==99) {
      text("時刻を設定してください", 100+iti_x, 660-iti_y);
    } else {
      text(setteijikoku, 100+iti_x, 660-iti_y);
    }
    if (lightset_result == 9999) {
      text("明るさを設定してください", 50+iti_x, 850-iti_y);
    } else {
      text("明るさ調整"+lightset_result, 50+iti_x, 850-iti_y);
    }
    text(lightset+"％", 140+iti_x, 890-iti_y);
  }
}

void serverEvent(Server ConServer, Client ConClient)
{


  String[] ClientID = {"A", "B", "C", "D", "E"};
  for (int i=0; i< MAX_CLIENT; i++) {
    if (ConServer == myServer[i]) {
      myClient[i] = ConClient;
      ClientIP[i]= ConClient.ip();
      Message[i] = "クライアント"+ClientID[i]+"と接続";
      numClient++;
    }
  }



  if (numClient >= MAX_CLIENT) {
    State=1;

    delay(100);
    for (int i = 0; i < MAX_CLIENT; i++) {
      byte sendData = (byte)0;
      myServer[i].write(sendData);
    }
  }
}



int num=0;







void clientEvent(Client RecvClient) {
  int NumBytes = RecvClient.available();
  byte[]myBuffer = RecvClient.readBytes(NumBytes);

  String data_all0="";
  String data_all1="";
  String data_all2="";



  switch( State ) {
  case 0:
    break;
  case 1://時間判定
    if (startTime[0] < hour) {
      State =2;
    } else if (startTime[0] == hour) {
      if (startTime[1] < minute) {
        State =2;
      } else {
        State =1;
      }
    } else {
      State=1;
    }
    break;

  case 2://人感知判定

    hitokanti=0;

    if (RecvClient == myClient[2]) {
     

      for (int i=0; i<NumBytes; i++) {
        byte keyval2;
        keyval2=myBuffer[i];
        
        data_all2=data_all1+str(char(keyval2));
        hitokanti = Integer.parseInt(data_all2);
      }
    }else{
      break;
    }
    
    
    if (hitokanti == 1) {
      State = 3;
      Message2[2]="人を感知したので明るさ調整を行います";
    } else {
      State =2;
      Message2[2]="人がいないのでLEDを消します";

      myServer[3].write("0000");//LEDをけす
    }

    break;

  case 3://Client[0]からデータ取得

    if (RecvClient == myClient[0]) {

      for (int i=0; i<NumBytes; i++) {

        byte keyval0;
        keyval0= myBuffer[i];

        data_all0= data_all0+str(char(keyval0));
      }
    }else{
      break;
    }
    uti=0;

    uti=Integer.parseInt(data_all0);
    data_all0="";

    if (uti >0) {
      Message2[0] = "光センサ（内）から" + uti +"を受信";
      State =4;
    } else {
      Message2[0] = "光センサ（内）からデータを受信していません";
      State=3;
    }






    break;
  case 4://明かり調整


    if (RecvClient == myClient[1]) {

      for (int i=0; i<NumBytes; i++) {
        byte keyval1;
        keyval1=myBuffer[i];
        data_all1=data_all1+str(char(keyval1));
      }

      Message2[1]="光センサ（外）から"+data_all1+"を受信";
      
    }else{
      break;
    }
    soto=0;
    soto = Integer.parseInt(data_all1);
    data_all1="";

    if (soto > uti) {
      myServer[4].write("1");
      Message2[4]="カーテンをあける指示を送信";
    } else {
      myServer[4].write("0");
      Message2[4] = "カーテンを閉める指示を送信";
    }



    if (lightset_result == 9999) {
      Message2[3]="明るさを設定してください";
    } else  if (lightset_result > uti) {
      myServer[3].write("1111");
      Message2[3]="LEDを明るくする指示を送信";
     
    } else {
      myServer[3].write("0000");
      Message2[3]="LEDを暗くする指示を送信";
     
    }
    
    
    
    
    State =2;


    break;
  default:
    break;
  }
}

int h=99;
int m=99;

void startH(String hh) {
  h = Integer.parseInt(hh);
}

void startM(String mm) {
  m = Integer.parseInt(mm);
}

void schedule() {
  startTime[0]=h;
  startTime[1]=m;
  setteijikoku=("現在の設定時刻 : "+ startTime[0]+"時"+startTime[1]+"分");
}

void Lset() {
  lightset_result=(int)(1024*(double)lightset/100);
}
