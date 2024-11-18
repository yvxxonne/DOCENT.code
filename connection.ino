#include "FirebaseESP8266.h" // 반드시 파이어베이스 라이브러리를 먼저 추가해야 합니다.
#include <ESP8266WiFi.h>
 
#define FIREBASE_HOST "-" // http 제외.
#define FIREBASE_AUTH "-" // 데이터베이스 비밀번호
#define WIFI_SSID "-" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "-" // wifi 비밀번호

FirebaseData firebaseData;
FirebaseJson json;
 
void printResult(FirebaseData &data);
int count = 4,flag=0,flag2=1,target = 4,memo=4, value= 4;
void setup() // wifi 접속 과정.
{
  Serial.begin(115200);
  pinMode(D4,INPUT);// 센서 핀
  pinMode(D5,OUTPUT);// 메가 연결할 핀


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
 
void loop()
{
   if( digitalRead(D4) == LOW )
  {
    if(flag == 1)
    {
       flag = 0;
       if( count < 4 ) count ++;
       else count = 1;
    }
  }
  else flag = 1;

  if (Firebase.getInt(firebaseData, "-")) // 해당하는 key가 있으면 (앱 인벤터에서는 tag)
  {
    target = firebaseData.intData(); // receive 'target' state
    if( target != count ) digitalWrite(D5,LOW);
    else digitalWrite(D5,HIGH);
  }
  else Serial.println(firebaseData.errorReason()); // print error message

  Serial.print("count: ");
  Serial.println(count);
  Serial.print("target: ");
  Serial.println(target);
  Serial.println();

  delay(50); // 0.1초마다 반복
}
