//-------------------------------------------- pin numbering
int P_driveMotorSpeed = 13;
int P_turnMotorSpeed = 12;
int P_driveMotorForward = 11;
int P_driveMotorReverse = 10;
int P_turnMotorRight = 9;
int P_turnMotorLeft = 8;


int P_frontLeftSonicTrigger = 22;
int P_frontLeftSonicEcho = 23;
int P_frontLeadSonicTrigger = 24;
int P_frontLeadSonicEcho = 25;
int P_frontRightSonicTrigger = 26;
int P_frontRightSonicEcho = 27;

int P_beep = 7;

//-------------------------------------------- consts





//-------------------------------------------- params
int V_frontLeadDistace = 1000;
int V_frontLeadDistacePrev = 1000;
int V_frontLeadDistacePrevPrev = 1000;


bool V_goingForward = false;
bool V_frontLeadObstacle = false;


void setup() {
  
  Serial.begin(9600);
  //------------------------------------------ pin assignment
  pinMode(P_driveMotorSpeed, OUTPUT);
  pinMode(P_turnMotorSpeed, OUTPUT);
  pinMode(P_driveMotorForward, OUTPUT);
  pinMode(P_driveMotorReverse, OUTPUT);
  pinMode(P_turnMotorRight, OUTPUT);
  pinMode(P_turnMotorLeft, OUTPUT);

  pinMode(P_frontLeftSonicTrigger, OUTPUT);
  pinMode(P_frontLeftSonicEcho, INPUT);
  pinMode(P_frontLeadSonicTrigger, OUTPUT);
  pinMode(P_frontLeadSonicEcho, INPUT);
  pinMode(P_frontRightSonicTrigger, OUTPUT);
  pinMode(P_frontRightSonicEcho, INPUT);

  
  pinMode(P_beep, OUTPUT);

  //------------------------------------------ initializations
  setTurnMotorSpeed();
}


//-------------------------------------------- pin controls

void setSpeedLevel (int level) {
  if (level > 3) {
    level = 3;
  }
  if (level < 1) {
    level = 1;
  }
  analogWrite(P_driveMotorSpeed, (level*50)+100);
}

void beep (int count) {
  for (int i=1; i<=count; i++) {
    digitalWrite(P_beep, HIGH);
    delay(40);
    digitalWrite(P_beep, LOW);
    delay(100);
  }
}

void setTurnMotorSpeed () {
  digitalWrite(P_turnMotorSpeed, HIGH);
}

void turnLeft () {
  digitalWrite(P_turnMotorLeft, HIGH);
  digitalWrite(P_turnMotorRight, LOW);
}

void turnRight () {
  digitalWrite(P_turnMotorLeft, LOW);
  digitalWrite(P_turnMotorRight, HIGH);
}

void turnStraight () {
  digitalWrite(P_turnMotorLeft, LOW);
  digitalWrite(P_turnMotorRight, LOW);
}

void forward () {
  digitalWrite(P_driveMotorForward, HIGH);
  digitalWrite(P_driveMotorReverse, LOW);
}

void reverse () {
  digitalWrite(P_driveMotorForward, LOW);
  digitalWrite(P_driveMotorReverse, HIGH);
}

void halt() {
  digitalWrite(P_driveMotorForward, LOW);
  digitalWrite(P_driveMotorReverse, LOW);
  digitalWrite(P_turnMotorLeft, LOW);
  digitalWrite(P_turnMotorRight, LOW);
}

void triggerSonics() {
  digitalWrite(P_frontLeadSonicTrigger, HIGH);
}

void stopSonics() {
  digitalWrite(P_frontLeadSonicTrigger, LOW);
}

int listenTo(int listner) {
  return pulseIn(listner, HIGH);
}

//-------------------------------------------- ground level operations

void neutral() {
  V_goingForward = false;
  halt();
}

void goStraight(int speedLevel) {
  V_goingForward = true;
  setSpeedLevel(speedLevel);
  turnStraight();
  forward();
}

void reverseStraight(int speedLevel) {
  setSpeedLevel(speedLevel);
  turnStraight();
  reverse();
}

void goRight(int speedLevel) {
  V_goingForward = true;
  setSpeedLevel(speedLevel);
  turnRight();
  forward();
}

void goLeft(int speedLevel) {
  V_goingForward = true;
  setSpeedLevel(speedLevel);
  turnLeft();
  forward();
}

void reverseLeft(int speedLevel) {
  V_goingForward = false;
  setSpeedLevel(speedLevel);
  turnRight();
  reverse();
}

void reverseRight(int speedLevel) {
  V_goingForward = false;
  setSpeedLevel(speedLevel);
  turnLeft();
  reverse();
}

int convertToDistance(int durartion) {
  return (durartion/2)/29.1;
}

void listenToSonics() {
  V_frontLeadDistacePrevPrev = V_frontLeadDistacePrev;
  V_frontLeadDistacePrev = V_frontLeadDistace;
  V_frontLeadDistace = convertToDistance(listenTo(P_frontLeadSonicEcho));
}

void whatIsArroundMe() {
  if (V_frontLeadDistace < 50) {
    V_frontLeadObstacle = true;
  } else {
    V_frontLeadObstacle = false;
  }
}



void imABat() {
  triggerSonics();
  delay(200);
  stopSonics();
  listenToSonics();
}




//-------------------------------------------- smart operations


void avoidFrontImpact() {
  int momentumSpeedPrev = V_frontLeadDistacePrevPrev - V_frontLeadDistacePrev;
  int momentumSpeed = ((V_frontLeadDistacePrev - V_frontLeadDistace) + momentumSpeedPrev)/2;
  if (momentumSpeed > 5) {
    reverseStraight(3);
  } else {
    if (momentumSpeed > 2) {
      reverseStraight(1);
    }
  }
}

void go() {
  if (V_frontLeadObstacle) {
    if (V_goingForward) {
      neutral();
    } else {
      avoidFrontImpact();
    }
  } else {
    goStraight(1);
  }
  
}

void watch() {
  imABat();
  whatIsArroundMe();
}




//-------------------------------------------- loop

void loop() {
  watch();
  go();
  
  
  /*beep(2);
  goStraight(2,1);
  goStraight(2,2);
  goStraight(2,3);
  goRight(1, 2);
  reverseRight(1, 2);
  goRight(1, 2);
  reverseRight(1, 2);
  reverseStraight(2,3);
  reverseStraight(2,2);
  reverseStraight(2,1);
  halt();
  beep(3);
  delay(5000);*/
}
