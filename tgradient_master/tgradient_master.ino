
//#include <Wire.h>
//#include <SFE_BMP180.h>
//SFE_BMP180 pressure;
//#define ALTITUDE 1655.0
//char status;
double tempvacd, pressurevacd;
int getvacpressureflag = 0;
int processvacpressureflag = 0;
int getprgpressureflag = 0;
int getoverallstatusflag = 0;
int vacpressurepin = A2;

int prgpressurepin = A1;
float prgpresval1 = 0;
float prgpresval2 = 0;
float prgpresvalbuffer = 0;

int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int buttonlockstate = 0;
int buttonlockonflag = 0;
int buttonlockoffflag = 0;

const byte numChars = 64;
char receivedChars[numChars];
int len = 0;

const int pin1 = 2; //rele 1, topeni
const int pin2 = 3; // ultrasound
const int pin3 = 4; // vacuum, plate
const int pin4 = 5; // light
const int pin5 = 6; // --------- vac pull solenoid
const int pin6 = 7; // vacu um, port
const int pin7 = 8; // ---------- purge push gas
const int pin8 = 9;

int but1iter = 0;
int but2iter = 0;
int but3iter = 0;

int vacplatedecayon = 0;
int vacplatedecayoff = 0;

int lighttoggleflag = 0;
int lightoffflag = 0;
int lightonflag = 0;

int lightstate = 0;

int pulsepin = 10;
int vacpin = 11;
int lightpin = 12;
int pulsetime = 4000;
int vactime = 7000;

unsigned long vacplatemil = 0;
unsigned long vacportmil = 0;
unsigned long ultrasoundmil = 0;

unsigned long vacplatepass = 0;
unsigned long vacportpass = 0;
unsigned long ultrasoundpass = 0;

float vacplatepass2 = 0;
float vacportpass2 = 0;
float ultrasoundpass2 = 0;

int vacplatestate = 0;
int vacportstate = 0;
int ultrasoundstate = 0;

unsigned long pulvacmil = 0;
unsigned long pullvacpass = 0;
float pullvacpass2 = 0;
int pulvacstate = 0;
int pullvacflag = 0;

int vacplateflag = 0;
int vacportflag = 0;
int ultrasoundflag = 0;
int vacplateonflag = 0;
int vacplateoffflag = 0;
int purgeplateonflag = 0;
int purgeplateoffflag = 0;

int debouncetime = 1000;
unsigned long pulsebnc = 0;
unsigned long vacbnc = 0;
unsigned long lightbnc = 0;

unsigned long timelastoff = 0;
int printflag = 0;
int thermostatsetflag = 0;
float thermostatsettemp = 0;
int thermostatsetflagacq = 1;
boolean newData = false;
int thermostatsetflagoffacq = 1;
int heatingon = 0;
float tempbuffer = 0;
float tempbuffer2 = 0;
int sensormalfunctionlimit = -50;

int thermostatpin = pin1; // number of pin for thermostat relay
int ondelaysec = 5; // minimum secs to remain off
float hysteresisvalue = 0.2;

int pushbutton1flag = 0;
int pushbutton2flag = 0;
int pushbutton3flag = 0;
int pushbutton4flag = 0;
int pushbutton5flag = 0;
int pushbutton6flag = 0;

int pin1val = LOW;
int pin2val = LOW;
int pin3val = LOW;
int pin4val = LOW;
int pin5val = LOW;
int pin6val = LOW;
int pin7val = LOW;
int pin8val = HIGH;

float prgvalbuf1 = 0;
float prgvalbuf2 = 0;
float prgvalbuf3 = 0;
float prgvalbuf4 = 0;
float prgvalbuf5 = 0;
float prgvalbuf6 = 0;
float prgvalbuf7 = 0;
float prgvalbuf8 = 0;
float prgvalbuf9 = 0;
float prgvalbuf10 = 0;
float prgvalbuf11 = 0;

float vacvalbuf1 = 0;
float vacvalbuf2 = 0;
float vacvalbuf3 = 0;
float vacvalbuf4 = 0;
float vacvalbuf5 = 0;
float vacvalbuf6 = 0;
float vacvalbuf7 = 0;
float vacvalbuf8 = 0;
float vacvalbuf9 = 0;
float vacvalbuf10 = 0;
float vacvalbuf11 = 0;
unsigned long cumulmil = 0;

void setup() {
  Serial.begin(9600);
  //pinMode(prgpressurepin, INPUT_PULLUP);  // use pullup????????????,
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, LOW);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, LOW);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, LOW);
  pinMode(pin4, OUTPUT);
  digitalWrite(pin4, LOW);
  pinMode(pin5, OUTPUT);
  digitalWrite(pin5, LOW);
  pinMode(pin6, OUTPUT);
  digitalWrite(pin6, LOW);
  pinMode(pin7, OUTPUT);
  digitalWrite(pin7, LOW);
  pinMode(pin8, OUTPUT);
  digitalWrite(pin8, LOW);
  pinMode(pulsepin, INPUT_PULLUP);
  pinMode(vacpin, INPUT_PULLUP);
  pinMode(lightpin, INPUT_PULLUP);
  heatingon = 0;
  memset(receivedChars, 0, sizeof(receivedChars));

  /* while (!Serial) {
     // wait for serial port to connect. Needed for native USB port only
    }*/
  //if (pressure.begin()) {
  Serial.println("Thermostat module ready. BMP180 init success");
  //}
  //else {
  //   Serial.println("Thermostat module ready. BMP180 init failure");
  //}

}

void loop() {
  recvWithEndMarker();
  showNewData();

  
  processPins();


}

//float prgvalbuf1 = 0;
//float prgvalbuf2 = 0;
//float prgvalbuf3 = 0;
//float prgvalbuf4 = 0;
//float prgvalbuf5 = 0;
//float prgvalbuf6 = 0;
//
//float vacvalbuf1 = 0;
//float vacvalbuf2 = 0;
//float vacvalbuf3 = 0;
//float vacvalbuf4 = 0;
//float vacvalbuf5 = 0;
//float vacvalbuf6 = 0;
//unsigned long cumulmil = 0;
//readvacpressure(), 2);readprgpressure();

void cumulateAnalogs() {
  if ((cumulmil + 500) < millis()) {
    cumulmil = millis();
    prgvalbuf1 = prgvalbuf2;
    prgvalbuf2 = prgvalbuf3;
    prgvalbuf3 = prgvalbuf4;
    prgvalbuf4 = prgvalbuf5;
    prgvalbuf5 = prgvalbuf6;
    prgvalbuf6 = prgvalbuf7;
    prgvalbuf7 = prgvalbuf8;
    prgvalbuf8 = prgvalbuf9;
    prgvalbuf9 = prgvalbuf10;
    prgvalbuf10 = prgvalbuf11;
    prgvalbuf11 = readprgpressure();

    vacvalbuf1 = vacvalbuf2;
    vacvalbuf2 = vacvalbuf3;
    vacvalbuf3 = vacvalbuf4;
    vacvalbuf4 = vacvalbuf5;
    vacvalbuf5 = vacvalbuf6;
    vacvalbuf6 = vacvalbuf7;
    vacvalbuf7 = vacvalbuf8;
    vacvalbuf8 = vacvalbuf9;
    vacvalbuf9 = vacvalbuf10;
    vacvalbuf10 = vacvalbuf11;
    vacvalbuf11 = readvacpressure();
  }
}

float readCumVac() {
  return ((vacvalbuf1+vacvalbuf2+vacvalbuf3+vacvalbuf4+vacvalbuf5+vacvalbuf6+vacvalbuf7+vacvalbuf8+vacvalbuf9+vacvalbuf10+vacvalbuf11)/11.0);
}
float readCumPrg() {
  return ((prgvalbuf1+prgvalbuf2+prgvalbuf3+prgvalbuf4+prgvalbuf5+prgvalbuf6+prgvalbuf7+prgvalbuf8+prgvalbuf9+prgvalbuf10+prgvalbuf11)/11.0);
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void processPins() {
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, pin1val);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, pin2val);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, pin3val);
  pinMode(pin4, OUTPUT);
  digitalWrite(pin4, pin4val);
  pinMode(pin5, OUTPUT);
  digitalWrite(pin5, pin5val);
  pinMode(pin6, OUTPUT);
  digitalWrite(pin6, pin6val);
  pinMode(pin7, OUTPUT);
  digitalWrite(pin7, pin7val);
  pinMode(pin8, OUTPUT);
  digitalWrite(pin8, pin8val);
}

void setPinVal(int pinnumber, int pinstate) {
  switch (pinnumber) {
    case pin1:
      pin1val = pinstate;
      digitalWrite(pin1, pin1val);
      break;
    case pin2:
      pin2val = pinstate;
      digitalWrite(pin2, pin2val);
      break;
    case pin3:
      pin3val = pinstate;
      digitalWrite(pin3, pin3val);
      break;
    case pin4:
      pin4val = pinstate;
      digitalWrite(pin4, pin4val);
      break;
    case pin5:
      pin5val = pinstate;
      digitalWrite(pin5, pin5val);
      break;
    case pin6:
      pin6val = pinstate;
      digitalWrite(pin6, pin6val);
      break;
    case pin7:
      pin7val = pinstate;
      digitalWrite(pin7, pin7val);
      break;
    case pin8:
      pin8val = pinstate;
      digitalWrite(pin8, pin8val);
      break;
    default:
      // nope
      break;
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;


  while (Serial.available() > 0 && newData == false) {
    // turn off thermostat
    if (receivedChars[0] == 'O' && receivedChars[1] == 'F' && receivedChars[2] == 'F') { // && receivedChars[1] == '1'
      thermostatsetflag = 0;
      thermostatsetflagoffacq = 0;
      // thermostatsettemp = 0;
    }
    // set temperature
    if (receivedChars[0] == 'S' && receivedChars[1] == 'E' && receivedChars[2] == 'T' && receivedChars[3] == ' ') {
      thermostatsettemp = Serial.parseFloat();
      thermostatsetflag = 1;
      thermostatsetflagacq = 0;
      timelastoff = 0;
    }
    // print command  = read temperature
    if (receivedChars[0] == 'R' && receivedChars[1] == 'E' && receivedChars[2] == 'A' && receivedChars[3] == 'D') {
      printflag = 1;
    }

    if (receivedChars[0] == 'V' && receivedChars[1] == 'P' && receivedChars[2] == 'L' && receivedChars[3] == ' ') {
      vacplatepass2 = Serial.parseFloat();
      vacplatepass = (unsigned long) round(vacplatepass2);
      vacplateflag = 1;
      //vacplatemil = millis();
    }
    if (receivedChars[0] == 'V' && receivedChars[1] == 'W' && receivedChars[2] == 'P' && receivedChars[3] == ' ') {
      vacportpass2 = Serial.parseFloat();
      vacportpass = (unsigned long) round(vacportpass2);
      vacportflag = 1;
      //vacportmil = millis();
    }
    if (receivedChars[0] == 'U' && receivedChars[1] == 'S' && receivedChars[2] == 'D' && receivedChars[3] == ' ') {
      ultrasoundpass2 = Serial.parseFloat();
      ultrasoundpass = (unsigned long) round(ultrasoundpass2);
      ultrasoundflag = 1;
      //ultrasoundmil = millis();
    }
     if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'L' && receivedChars[3] == 'L' && receivedChars[4] == ' ') {
      pullvacpass2 = Serial.parseFloat();
      pullvacpass = (unsigned long) round(pullvacpass2);
      pullvacflag = 1;
      //ultrasoundmil = millis();
    }   
    if (receivedChars[0] == 'L' && receivedChars[1] == 'T' && receivedChars[2] == 'T' && receivedChars[3] == 'G') {
      lighttoggleflag = 1;
    }
    if (receivedChars[0] == 'L' && receivedChars[1] == 'T' && receivedChars[2] == 'O' && receivedChars[3] == 'N') {
      lightonflag = 1;
    }
    if (receivedChars[0] == 'L' && receivedChars[1] == 'T' && receivedChars[2] == 'O' && receivedChars[3] == 'F') {
      lightoffflag = 1;
    }
    if (receivedChars[0] == 'V' && receivedChars[1] == 'A' && receivedChars[2] == 'C' && receivedChars[3] == 'P' && receivedChars[4] == 'O' && receivedChars[5] == 'N') {
      vacplateonflag = 1;
    }
    if (receivedChars[0] == 'V' && receivedChars[1] == 'A' && receivedChars[2] == 'C' && receivedChars[3] == 'P' && receivedChars[4] == 'O' && receivedChars[5] == 'F') {
      vacplateoffflag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'R' && receivedChars[2] == 'G' && receivedChars[3] == 'P' && receivedChars[4] == 'O' && receivedChars[5] == 'N') {
      purgeplateonflag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'R' && receivedChars[2] == 'G' && receivedChars[3] == 'P' && receivedChars[4] == 'O' && receivedChars[5] == 'F') {
      purgeplateoffflag = 1;
    }
    if (receivedChars[0] == 'B' && receivedChars[1] == 'L' && receivedChars[2] == 'C' && receivedChars[3] == 'K' && receivedChars[4] == 'O' && receivedChars[5] == 'N') {
      buttonlockonflag = 1;
    }
    if (receivedChars[0] == 'B' && receivedChars[1] == 'L' && receivedChars[2] == 'C' && receivedChars[3] == 'K' && receivedChars[4] == 'O' && receivedChars[5] == 'F') {
      buttonlockoffflag = 1;
    }
    if (receivedChars[0] == 'G' && receivedChars[1] == 'E' && receivedChars[2] == 'T' && receivedChars[3] == 'V' && receivedChars[4] == 'A' && receivedChars[5] == 'C') {
      getvacpressureflag = 1;
    }
    if (receivedChars[0] == 'G' && receivedChars[1] == 'E' && receivedChars[2] == 'T' && receivedChars[3] == 'P' && receivedChars[4] == 'R' && receivedChars[5] == 'G') {
      getprgpressureflag = 1;
    }
    if (receivedChars[0] == 'G' && receivedChars[1] == 'E' && receivedChars[2] == 'T' && receivedChars[3] == 'A' && receivedChars[4] == 'L' && receivedChars[5] == 'L') {
      getoverallstatusflag = 1;
    }

    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '1') {
      pushbutton1flag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '2') {
      pushbutton2flag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '3') {
      pushbutton3flag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '4') {
      pushbutton4flag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '5') {
      pushbutton5flag = 1;
    }
    if (receivedChars[0] == 'P' && receivedChars[1] == 'U' && receivedChars[2] == 'S' && receivedChars[3] == 'H' && receivedChars[4] == '6') {
      pushbutton6flag = 1;
    }
            


    
    if (receivedChars[0] == 'R' && receivedChars[1] == 'E' && receivedChars[2] == 'S' && receivedChars[3] == 'E' && receivedChars[4] == 'T' && receivedChars[5] == 'A') {
      resetFunc(); 
    }
    



    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      len = ndx;
      ndx = 0;
      newData = true;
    }



  }
}

void showNewData() {
  if (newData == true) {
    // turn off thermostat
    if (thermostatsetflag == 0 && thermostatsetflagoffacq == 0) {
      Serial.print("Command received: '");
      Serial.print(receivedChars);
      Serial.println("'.");
     // digitalWrite(thermostatpin, HIGH);
      heatingon = 0;
      thermostatsettemp = 0;
      Serial.println("Thermostat deactivated.");
      thermostatsetflagoffacq = 1;
    }
    // tripwires GND-pin
    else if (thermostatsetflagacq == 0 && thermostatsetflag == 1) {
      thermostatsetflagacq = 1;
      Serial.print("Command received: '");
      Serial.print(receivedChars);
      Serial.print(thermostatsettemp);
      Serial.println("'.");

      if (thermostatsettemp > 100 || thermostatsettemp < 0) {
        thermostatsetflag = 0;
        thermostatsettemp = 0;
        Serial.println("Cannot comply, temperature out of range.");
      }
      else {
        Serial.print("Thermostat set to ");
        Serial.print(thermostatsettemp);
        Serial.println(" °C.");
      }

    }
    // print command = read temperature
    else if (printflag) {
      Serial.print("Command received: '");
      Serial.print(receivedChars);
      Serial.println("'.");
      tempbuffer2 = readtemperaturemulti();
      if (tempbuffer2 < -50) {
        Serial.println("Warning: Sensor disconnected!");
      }
      else {
        Serial.print("Temperature: '");
        Serial.print(tempbuffer2);
        Serial.println(" °C'");
      }
    }
    else if (vacplateflag == 1) {
      vacplateflag = 0;
      vacplatemil = millis();
      setPinVal(pin3, HIGH);
      vacplatestate = 1;
      Serial.print("Purging plate for ");
      Serial.print(vacplatepass);
      Serial.println(" ms.");
    }
    else if (vacportflag == 1) {
      vacportflag = 0;
      vacportmil = millis();
      pinMode(pin6, OUTPUT);
      setPinVal(pin6, HIGH);


      vacportstate = 1;
      Serial.print("Purging port for ");
      Serial.print(vacportpass);
      Serial.println(" ms.");
    }
    else if (ultrasoundflag == 1) {
      ultrasoundflag = 0;
      ultrasoundmil = millis();
      setPinVal(pin2, HIGH);
      ultrasoundstate = 1;
      Serial.print("Ultrasound pulsing for ");
      Serial.print(ultrasoundpass);
      Serial.println(" ms.");
    }
    else if (pullvacflag == 1) {
      pullvacflag = 0;
      pulvacmil = millis();
      setPinVal(pin5, HIGH);
      pulvacstate = 1;
      Serial.print("Vacuum pulsing for ");
      Serial.print(pullvacpass);
      Serial.println(" ms.");
    }    
    else if (lighttoggleflag == 1) {
      lighttoggleflag = 0;
      if (lightstate == 0) {
        setPinVal(pin4, LOW);
        Serial.println("Light toggled on.");
        lightstate = 1;
      }
      else {
        setPinVal(pin4, HIGH);
        Serial.println("Light toggled off.");
        lightstate = 0;
      }
    }
    else if (lightoffflag == 1) {
      lightoffflag = 0;
      setPinVal(pin4, HIGH);
      Serial.println("Light turned off.");
      lightstate = 0;
    }
    else if (lightonflag == 1) {
      lightonflag = 0;
      setPinVal(pin4, LOW);
      Serial.println("Light turned on.");
      lightstate = 1;
    }




    
    else if (pushbutton1flag == 1) {
      pushbutton1flag = 0;
      setPinVal(pin1, HIGH);
      delay(200);
      setPinVal(pin1, LOW);
      Serial.println("Button 1 pushed.");
    }
    else if (pushbutton2flag == 1) {
      pushbutton2flag = 0;
      setPinVal(pin2, HIGH);
      delay(200);
      setPinVal(pin2, LOW);
      Serial.println("Button 2 pushed.");
    }
    else if (pushbutton3flag == 1) {
      pushbutton3flag = 0;
      setPinVal(pin3, HIGH);
      delay(200);
      setPinVal(pin3, LOW);
      Serial.println("Button 3 pushed.");
    }
     else if (pushbutton4flag == 1) {
      pushbutton4flag = 0;
      setPinVal(pin4, HIGH);
      delay(200);
      setPinVal(pin4, LOW);
      Serial.println("Button 4 pushed.");
    }
    else if (pushbutton5flag == 1) {
      pushbutton5flag = 0;
      setPinVal(pin5, HIGH);
      delay(200);
      setPinVal(pin5, LOW);
      Serial.println("Button 5 pushed.");
    }
     else if (pushbutton6flag == 1) {
      pushbutton6flag = 0;
      setPinVal(pin6, HIGH);
      delay(200);
      setPinVal(pin6, LOW);
      Serial.println("Button 6 pushed.");
    }



    
                  
    else if (vacplateonflag == 1) {
      setPinVal(pin3, HIGH);
      Serial.println("Vacuum purging plate now.");
      vacplatedecayon = 1;
      vacplateonflag = 0;
    }
    else if (vacplateoffflag == 1) {
      setPinVal(pin3, LOW);
      Serial.println("Vacuum purging plate stopped.");
      vacplatedecayoff = 1;
      vacplateoffflag = 0;
    }
    else if (purgeplateonflag == 1) {
      setPinVal(pin7, HIGH);
      Serial.println("Gas purging plate now.");
      purgeplateonflag = 0;
    }
    else if (purgeplateoffflag == 1) {
      setPinVal(pin7, LOW);
      Serial.println("Gas purging plate stopped.");
      purgeplateoffflag = 0;
    }
    else if (buttonlockonflag == 1) {
      buttonlockstate = 1;
      Serial.println("Buttons locked.");
      buttonlockonflag = 0;
    }
    else if (buttonlockoffflag == 1) {
      buttonlockstate = 0;
      Serial.println("Buttons unlocked.");
      buttonlockoffflag = 0;
    }
    else if (getvacpressureflag == 1) {
      getvacpressureflag = 0;
      Serial.print("Reading vacuum pressure. Wait: ");
      Serial.print("Vacuum pressure: '");
      Serial.print(readvacpressure(), 2);
      Serial.println(" mb'");
    }
    else if (getprgpressureflag == 1) {
      getprgpressureflag = 0;
      Serial.print("Command received: '");
      Serial.print(receivedChars);
      Serial.println("'.");
      prgpresvalbuffer = readprgpressure();
      if (prgpresvalbuffer < -2) {
        Serial.println("Warning: Sensor disconnected!");
      }
      else {
        Serial.print("Purge gas pressure: '");
        Serial.print(prgpresvalbuffer);
        Serial.println("' bar");
      }
    }
    else if (getoverallstatusflag == 1) {
      getoverallstatusflag = 0;
      Serial.print("Command received: '");
      Serial.print(receivedChars);
      Serial.println("'.");
      prgpresvalbuffer = readCumPrg(); //readprgpressure();
      if (prgpresvalbuffer < -2) {
        Serial.print("Warning: PRG Sensor disconnected!, ");
      }
      else {
        Serial.print("Purge gas pressure: '");
        Serial.print(prgpresvalbuffer);
        Serial.print(" bar', ");
      }
      tempbuffer2 = readtemperaturemulti();
      if (tempbuffer2 < -50) {
        Serial.print("Warning: TEMP Sensor disconnected!, ");
      }
      else {
        Serial.print("Temperature: '");
        Serial.print(tempbuffer2);
        Serial.print(" °C', ");
      }

      Serial.print("Reading vacuum pressure. Wait: ");
      Serial.print("Vacuum pressure: '");
      Serial.print(readCumVac(), 2);
      Serial.println(" mb'");

    }
    // echo
    else if (receivedChars[0] == '\0') {
      Serial.println("Ready.");
    }
    // unknown command
    else {
      Serial.print("Unknown command: '");
      Serial.print(receivedChars);
      Serial.println("', input dropped.");
    }
    printflag = 0;
    newData = false;
    len = 0;
    memset(receivedChars, 0, sizeof(receivedChars));
  }
}

float readprgpressure() {
  int i = 0;
  prgpresval1 = 0;
  for (i = 0; i < 100; i++) {
    prgpresval1 = prgpresval1 + analogRead(prgpressurepin);
    //delay(5);
  }
  prgpresval1 = 1.0 * (float)prgpresval1 / 100.0;
  //prgpresval1 = (analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin)+analogRead(prgpressurepin))/10.0;
  prgpresval2 = ((((float)prgpresval1 / 1023.0) * 5.0) - 0.496) * 5.171;
  return prgpresval2;
  //return analogRead(prgpressurepin);
}

float readvacpressure() {
  int i = 0;
  float vacpresval1 = 0;
  float vacpresval2 = 0;
  for (i = 0; i < 100; i++) {
    vacpresval1 = vacpresval1 + analogRead(vacpressurepin);
  }
  vacpresval1 = 1.0 * (float)vacpresval1 / 100.0;
  vacpresval2 = ((((float)vacpresval1 / 1023.0) * 5.0) * 56.818 * 3.333) + 50.0;
  return vacpresval2;
}


float readtemperature() {

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5.0 + 32.0;

  // Serial.print("Temperature: ");
  //Serial.print(Tf);
  //Serial.print(" F; ");
  //Serial.print(Tc);
  //Serial.println(" C");

  //delay(500);
  return Tc;
}
float readtemperaturemulti() {

  return (readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature() + readtemperature()) / 10;
}

