#include <Wire.h>
#include <Mouse.h>

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ;

long offX = 0, offZ = 0;
long maxX = 5000, minX = -5000, maxY = 5000, minY = -5000;

float filterWeight = 0.12; 
float smoothX = 0, smoothZ = 0;
int deadzone = 1500; 

float velX = 0, velY = 0;
float friction = 0.70;     
float acceleration = 0.2;   
int maxSpeedLimit = 12;

bool mouseActive = false;

void printMenu() {
  Serial.println("\n=============================================");
  Serial.println("[0] عرض القائمة");
  Serial.println("[C] تصفير المركز (المرجع)");
  Serial.println("[H] مسح أفقي (يمين/يسار)");
  Serial.println("[V] مسح رأسي (فوق/تحت)");
  Serial.println("[1] تشغيل (ON) | [2] إيقاف (OFF)");
  Serial.println("================================================");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); Wire.write(0); Wire.endTransmission(true);
  Mouse.begin();
  delay(1000);
  printMenu();
}
void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd == 'L') {
      Mouse.press(MOUSE_LEFT);    
      delay(50);                  
      Mouse.release(MOUSE_LEFT);
      Serial.println(">> Done: Left Click"); 
    } 
    else if (cmd == 'R') {
      Mouse.press(MOUSE_RIGHT);
      delay(50);
      Mouse.release(MOUSE_RIGHT);
      Serial.println(">> Done: Right Click");
    }
    
    
    readMPU();
    
    if (cmd == '0') printMenu();
    else if (cmd == 'C' || cmd == 'c') {
      offX = 0; offZ = 0;
      for(int i=0; i<15; i++) {
        readMPU(); offX += AcX; offZ += AcZ; delay(10);
      }
      offX /= 15; offZ /= 15;
      smoothX = offX; smoothZ = offZ;
      velX = 0; velY = 0;
      Serial.println(">> تم تصفير المركز.");
    }
    else if (cmd == '1') { mouseActive = true; Serial.println(">> تشغيل."); }
    else if (cmd == '2') { mouseActive = false; Serial.println(">> إيقاف."); }
    else if (cmd == 'H' || cmd == 'h') {
      Serial.println(">> مسح أفقي لـ 3 ثوانٍ...");
      maxY = 1000; minY = -1000;
      long s = millis(); while(millis()-s < 3000) {
        readMPU(); long d = AcZ-offZ; if(d>maxY) maxY=d; if(d<minY) minY=d; delay(10);
      }
      Serial.println(">> تم.");
    }
    else if (cmd == 'V' || cmd == 'v') {
      Serial.println(">> مسح رأسي لـ 3 ثوانٍ...");
      maxX = 1000; minX = -1000;
      long s = millis(); while(millis()-s < 3000) {
        readMPU(); long d = AcX-offX; if(d>maxX) maxX=d; if(d<minX) minX=d; delay(10);
      }
      Serial.println(">> تم.");
    }
  }

  
  if (mouseActive) {
    readMPU();
    smoothX = (AcX * filterWeight) + (smoothX * (1.0 - filterWeight));
    smoothZ = (AcZ * filterWeight) + (smoothZ * (1.0 - filterWeight));

    float dV = smoothX - offX; 
    float dH = smoothZ - offZ;

    float targetVX = 0;
    float targetVY = 0;

    if (abs(dH) > deadzone) {
      float rangeH = (dH > 0) ? (maxY - deadzone) : (abs(minY) - deadzone);
      if (rangeH < 500) rangeH = 2000;
      targetVX = (constrain(dH/rangeH, -1.0, 1.0)) * maxSpeedLimit;
    }

    if (abs(dV) > deadzone) {
      float rangeV = (dV > 0) ? (maxX - deadzone) : (abs(minX) - deadzone);
      if (rangeV < 500) rangeV = 2000;
      targetVY = (constrain(dV/rangeV, -1.0, 1.0)) * maxSpeedLimit * -1;
    }

    velX = (velX * friction) + (targetVX * acceleration);
    velY = (velY * friction) + (targetVY * acceleration);

    if (abs(velX) > 0.2 || abs(velY) > 0.2) {
      Mouse.move((int)velX, (int)velY, 0);
    }
  }
  delay(12);
}

void readMPU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  if (Wire.available() >= 6) {
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
  }
}