    #include <Adafruit_NeoPixel.h>
    
    
    /*  PINOUT  */
    //0 tx
    //1 rx
    const uint8_t LIGHTPIN = 2;
    //3 unused
    const uint8_t RINGRPIN = 4;
    const uint8_t DOORPPIN = 5;
    const uint8_t DOOR2PIN = 6;
    const uint8_t DOOR1PIN = 7;
    const uint8_t BUTN1PIN = 8;
    const uint8_t BUTN2PIN = 9;
    const uint8_t DOORSEN1 = 10;
    const uint8_t DOORSEN2 = 11;

    //Player states
    bool player1ready = false;
    bool player2ready = false;

    //Round Stuff
    const long ROUNDLENGHT =  180000; //3 Minutes
    const long DOOROPENTIME = 120000; //2 Minutes
    long currentroundtime = 0;
    
    //Door Stuff
    bool door1open = false;
    bool door2open = false;

    //Light Stuff
    typedef uint32_t Color;
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LIGHTPIN, NEO_GRB + NEO_KHZ800);
    Color RED = strip.Color(255, 0, 0);
    Color YELLOW = strip.Color(255,255,0);
    Color GREEN = strip.Color(255, 0, 0);
    
    //Functions
void ringBell(){
  digitalWrite(RINGRPIN,HIGH);
  delay(1000);
  digitalWrite(RINGRPIN,LOW);
}

void doCountdownLights(){
  //strip.setPixelColor(11,color);
  //strip.fill(color, first, count);
  //strip.show();
}

void checkPlayer(const uint8_t PIN,bool& isready){
  if(isready){
    return;
  }
  if(digitalRead(PIN) == HIGH){
    //Set Lights
    isready = true;
  }
}

void openTrapDoors(){
  digitalWrite(DOORPPIN,HIGH); //Energise Door
  delay(500);
  digitalWrite(DOORPPIN,LOW); //
}

bool closeTrapDoor(const uint8_t door,const uint8_t sensor,bool& closed){
  if(closed){
    return true;
  }
  if(digitalRead(sensor) == HIGH){
    //It just closed
    delay(10);
    digitalWrite(door,HIGH);
    delay(100);
    digitalWrite(door,LOW);
    closed = true;
  }
  //Still open
}

void closeTrapDoors(){
  while(door1open || door2open){
    closeTrapDoor(DOOR1PIN,DOORSEN1,door1open);
    closeTrapDoor(DOOR2PIN,DOORSEN2,door2open);
  }
}

void setup() {
  pinMode(LIGHTPIN,OUTPUT);
  pinMode(RINGRPIN,OUTPUT);
  pinMode(DOORPPIN,OUTPUT);
  pinMode(DOOR2PIN,OUTPUT);
  pinMode(DOOR1PIN,OUTPUT);
  pinMode(BUTN1PIN,INPUT_PULLUP);
  pinMode(BUTN2PIN,INPUT_PULLUP);
  pinMode(DOORSEN1,INPUT_PULLUP);
  pinMode(DOORSEN2,INPUT_PULLUP);

  digitalWrite(RINGRPIN,LOW);
  digitalWrite(DOORPPIN,LOW);
  digitalWrite(DOOR1PIN,LOW);
  digitalWrite(DOOR2PIN,LOW);
  strip.begin();
  strip.show();
  closeTrapDoors(); //Ensure doors are closed before game begins.
}

void loop() {
  if(currentroundtime != 0){              //We are in a round
    delay(1);
    currentroundtime++;
    if(currentroundtime == DOOROPENTIME){ //Time to open the doors.
      openTrapDoors();
    }
    if(currentroundtime == DOOROPENTIME){ //Round end
      ringBell();
    }
  }
  else {                                  //We are not in a round.
    checkPlayer(BUTN1PIN,player1ready);
    checkPlayer(BUTN2PIN,player2ready);
    if(player1ready && player2ready){
        doCountdownLights();
        currentroundtime = 1;
    }
  }
}
