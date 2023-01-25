import processing.net.*;

Client myClient1,myClient2,myClient3,myClient4,myClient5;


int State;
String Message;

void setup(){
  size(400,200);
  
  
  PFont font = createFont("BIZ UDゴシック",16,true);
  textFont(font);
  
    //myClient1 = new Client(this,"localhost",1111);
    myClient2 = new Client(this,"localhost",3333);
    //myClient3 = new Client(this,"localhost",3333);
    myClient4 = new Client(this,"localhost",4444);
    myClient5 = new Client(this,"localhost",5555);
    
    
  
  State = 0;
  Message="";
}

void draw(){
  background(#2D3986);
  
  text("クライアントA",30,20);
  text(Message,30,40);
  



      
}

void clientEvent(Client c){
  int NumBytes =c.available();
  
  byte[] myBuffer = c.readBytes(NumBytes);
  
  switch(State){
    case 0:
      Message ="サーバーと接続完了"+str(myBuffer[0]);
      State =  1;
      break;
    case 1:
     Message = "クライアントAは"+str(key)+"をおした";
      

      break;
  }
  c.clear();
}

void keyTyped()
{
      byte sendData;
          // myClient1.write("500");
      delay(10);
      myClient2.write("80");
      delay(10);
      sendData = byte(key);
      Message=str(key);
      
      String dataa=str(sendData);
     //myClient3.write(dataa);
      delay(10);

 


      myClient4.write(dataa);
      delay(10);
      myClient5.write(dataa);
      delay(10);      
      
}
