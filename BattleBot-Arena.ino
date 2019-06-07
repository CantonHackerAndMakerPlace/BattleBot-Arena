    const int P1Pin =4;
    const int P1Light=8;
    const int P2Pin =2;
    const int P2Light=9;
    const int RefPin =3;
    const int RefLight=10;
    const int MainLightPin = 11;
    const int TrapDoor1Pin1 =12;   //7654
    const int TrapDoor1Pin2 =13;
    const int TrapDoor2Pin1 =34;
    const int TrapDoor2Pin2 =36;
    
    const int TrapSensor1 = 30;
    const int TrapSensor2 = 32;

    const int MatchSecondsPart1 = 60;
    const int MatchSecondsPart2 = 120;
    
    boolean Player1Ready = false;
    boolean Player2Ready = false;
    boolean StartRound   = false;
    boolean Trap1Ready = false;
    boolean Trap2Ready = false;

    int thousand=0;

void openTrapDoor(const int door){
  digitalWrite(doorp,HIGH); //Energise Door
  digitalWrite(door,HIGH);
  delay(500);
  digitalWrite(door,LOW);
  digitalWrite(doorp,LOW); //
}

void openTrapDoor1(){
    digitalWrite(TrapDoor1Pin2,LOW);
    delay(500);
    digitalWrite(TrapDoor1Pin1,HIGH);
    digitalWrite(TrapDoor1Pin2,HIGH);
  }//openTrapDoor()
void closeTrapDoor1(){
    digitalWrite(TrapDoor1Pin1,LOW);
    delay(500);
    digitalWrite(TrapDoor1Pin1,HIGH);
    digitalWrite(TrapDoor1Pin2,HIGH);
  }//closeTrapDoor1
void openTrapDoor2(){
    digitalWrite(TrapDoor2Pin2,LOW);
    delay(500);
    digitalWrite(TrapDoor2Pin1,HIGH);
    digitalWrite(TrapDoor2Pin2,HIGH);
  }//openTrapDoor2()
void closeTrapDoor2(){
    digitalWrite(TrapDoor2Pin1,LOW);
    delay(500);
    digitalWrite(TrapDoor2Pin1,HIGH);
    digitalWrite(TrapDoor2Pin2,HIGH);
  }//closeTrapDoor2

void setMainLights(boolean inState) {
    digitalWrite(MainLightPin,inState?HIGH:LOW);
  }//SetMainLights()

void setPlayerLights(int player , boolean inState) {
    digitalWrite( (player==1)?P1Light:P2Light,inState?HIGH:LOW);  
  }//setPlayerLights()

void setRefLight(boolean inState ) {
    digitalWrite(RefLight,inState?HIGH:LOW);
  }//setRefLight()

void setup() {
  pinMode(P1Pin,INPUT_PULLUP);
  pinMode(P2Pin,INPUT_PULLUP);
  pinMode(RefPin,INPUT_PULLUP);
  pinMode(P1Light,OUTPUT);
  pinMode(P2Light,OUTPUT);
  pinMode(RefLight,OUTPUT);
  pinMode(MainLightPin,OUTPUT);
  pinMode(TrapDoorPin1,OUTPUT);
  pinMode(TrapDoorPin2,OUTPUT);

  
  setMainLights(false);
  setPlayerLights(1,false);
  setPlayerLights(2,false);
  setRefLight(false);
  closeTrapDoor();
  Serial.begin(9600);
}//setup

void loop() {
  
  while ( (!Player1Ready) || (!Player2Ready)) {
      if (digitalRead(P1Pin) == HIGH) {
          Serial.println("Player 1 is pushing button");
          Player1Ready= true;
          setPlayerLights(1,true);
      }//if
      if (digitalRead(P2Pin) == HIGH) {
          Serial.println("Player 2 is pushing button");
          Player2Ready= true;
          setPlayerLights(2,true);
      }//if
  
    thousand++; if (thousand>1000) {thousand=0;}
    
    Serial.print  (Player1Ready?"Y":"N" );
    Serial.println(Player2Ready?"Y":"N" );
    
  }//while
  Serial.println("Both Players are ready, awaiting Ref:");
  while ( ! StartRound  ) {
    if (digitalRead(RefPin)==HIGH){
      Serial.println("The Ref is pushing button");
      StartRound= true;
      setRefLight(true);
    }  //if
  }//while
  Serial.println("Ref is ready, starting match...");
  setMainLights(true);
  
  for(int x=0;x<MatchSecondsPart1;x++){
        delay(100);     //1000
        Serial.println(x);
        //do something cool?
      }//for

openTrapDoor(); Serial.println("Trap Door open");
  
  for(int x=0;x<MatchSecondsPart1;x++){
        delay(100);//1000
        Serial.println(x);
        //do something cool?     
      }//for


Serial.println("Round Over, waiting for Ref to reset");
  while (digitalRead(RefPin)==LOW) {}//Wait for someone to press the ref button to reset

  while ( (!Trap1Ready) || (!Trap2Ready) ) {
      if digitalRead(TrapSensor1==LOW) {
            Trap1Ready = true;
            closeTrapDoor1();
          }
      if digitalRead(TrapSensor2==LOW) {
            Trap2Ready = true;
            closeTrapDoor2();
          }
    }
    
    setPlayerLights(1,false);
    Player1Ready=false;
    setPlayerLights(2,false);
    Player2Ready=false;
    setRefLight(false);
    StartRound=false;
    closeTrapDoor();
Serial.println("Arena Reset");

}//loop
