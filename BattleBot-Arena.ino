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
    const uint8_t DOORSEN1 = 11;
    const uint8_t DOORSEN2 = 13;

    //Player states
    bool player1ready = false;
    bool player2ready = false;

    //Round Stuff
    const long ROUNDDELAY =   10000;  //10 seconds
    const long ROUNDLENGHT =  180000; //3 Minutes
    const long DOOROPENTIME = 120000; //2 Minutes
    const long DOLIGHTDIM =   105000; //1 Minute 45 Seconds
    long currentroundtime = 0;
    
    //Door Stuff
    bool door1open = true;
    bool door2open = true;

    //Light Stuff
    const uint8_t STRIP_SIZE = 40;
    typedef uint32_t Color;
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_SIZE, LIGHTPIN, NEO_GRB + NEO_KHZ800);
    Color RED = strip.Color(0, 255, 0);
    Color YELLOW = strip.Color(200,255,0);
    Color GREEN = strip.Color(255, 0, 0);
    Color WHITE = strip.Color(255,255,255);
    Color BLUE = strip.Color(0,0,255);
    
    //Functions
void ringBell(){
  strip.fill(RED, 0, STRIP_SIZE);
  strip.show();
  Serial.println("RING");
  digitalWrite(RINGRPIN,HIGH);
  delay(1000);
  digitalWrite(RINGRPIN,LOW);
}

void doCountdownLights(){
  delay(ROUNDDELAY);
  Serial.println("Countdown");
  Serial.println("3...");
  strip.fill(RED, 0, STRIP_SIZE);
  strip.show();
  delay(1000);
  Serial.println("2...");
  strip.fill(YELLOW, 0, STRIP_SIZE);
  strip.show();
  delay(1000);
  Serial.println("1...");
  strip.fill(GREEN, 0, STRIP_SIZE);
  strip.show();
  delay(1000);
  Serial.println("Go!");
  strip.fill(WHITE, 0, STRIP_SIZE);
  strip.show();
  //strip.setPixelColor(11,color);
  //strip.fill(color, first, count);
  //strip.show();
}

void checkPlayer(const uint8_t PIN,bool& isready){
  Serial.println("CheckPlayer");
  if(isready){
    return;
  }
  if(digitalRead(PIN) == LOW){
    //Set Lights
    isready = true;
    if(PIN == BUTN1PIN){
      strip.fill(GREEN, 0, STRIP_SIZE / 2);
    }
    else{
      strip.fill(GREEN, STRIP_SIZE / 2, STRIP_SIZE / 2);
    }
    strip.show();
  }
}

void openTrapDoors(){
  strip.fill(WHITE, 0, STRIP_SIZE);
  strip.show();
  Serial.println("Open Trap Doors");
  digitalWrite(DOORPPIN,HIGH); //Energise Door
  delay(200);
  digitalWrite(DOORPPIN,LOW); //
}

bool closeTrapDoor(const uint8_t door,const uint8_t sensor,bool& dooropen){
  if(!dooropen){
    return true;
  }
  if(digitalRead(sensor) == LOW){
    Serial.print(sensor);
    Serial.println(" trap door closed.");
    //It just closed
    delay(10);
    digitalWrite(door,HIGH);
    delay(100);
    digitalWrite(door,LOW);
    dooropen = false;
  }
  //Still open
}

void closeTrapDoors(){
  while(door1open || door2open){
    Serial.println("Wainting for doors to close");
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
  Serial.begin(9600);
  strip.fill(RED, 0, STRIP_SIZE);
  strip.show();
  closeTrapDoors(); //Ensure doors are closed before game begins.
  strip.fill(BLUE, 0, STRIP_SIZE);
  strip.show();
}

void loop() {
  if(currentroundtime != 0){              //We are in a round
    delay(1);
    currentroundtime++;
    if(currentroundtime == DOOROPENTIME){ //Time to open the doors.
      openTrapDoors();
    }
    if(currentroundtime == DOLIGHTDIM){   //Warn players of doors opening
       strip.fill(YELLOW, 0, STRIP_SIZE);
       strip.show();
    }
    if(currentroundtime == ROUNDLENGHT){ //Round end
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
