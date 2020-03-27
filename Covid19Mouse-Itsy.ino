#include <stdio.h>

#include "Mouse.h"

#define MAX_DEST_X        1000
#define MAX_DEST_Y        1000

#define MAX_MOVE_X        250
#define MAX_MOVE_Y        250

#define MIN_TIME_GRAIN   5

int randomMouseMoveTimer;
int randomMouseMovePeriod;
int posX, posY;
int deltaX, deltaY;
int destX, destY;
bool moveOn = false;
bool stopX = false, stopY = false;
int moveEventTimer;
int moveSpeed = 1;
int moveTimeGrain = 10;    // 1 pixel per 100ms


//////////////////////////////////////////////////////////
// INIT
//////////////////////////////////////////////////////////
void setup() {
  // initialize the digital pin as an output.
  int bootDuration = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Serial port init
  Serial1.begin(115200);

  // Debug to see what's happening
  //delay(2000);
  
  Mouse.begin();    
  Serial1.println("Apocalypse Mouse");
  Serial1.println("v0.1 - 03/2020");

  // Place Mouse in center of screen
  posX = posY = 200;
  Mouse.move(0, 0, 0);
  // set new event in future time
  randomMouseMovePeriod = 5000;
  deltaX = moveSpeed + random(0,3);
  deltaY = moveSpeed + random(0,3);
  moveTimeGrain = 80 + random(0,150);
  moveEventTimer = millis();

}


//////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
//////////////////////////////////////////////////////////////////////////////////
void loop() { 

  if((millis() - randomMouseMoveTimer) > randomMouseMovePeriod && !moveOn) {
    randomMouseMoveTimer = millis();
    // set new dest
    int rnd = (MAX_MOVE_X/2) + random(MAX_MOVE_X);
    if(random(1000) > 500)
      rnd = -rnd;
    destX = posX + rnd;
    destX = constrain(destX, 0, MAX_DEST_X);
    rnd = (MAX_MOVE_Y/2) + random(MAX_MOVE_Y);
    if(random(1000) > 500)
      rnd = -rnd;
    destY = posY + rnd;
    destY = constrain(destY, 0, MAX_DEST_Y);
    deltaX = moveSpeed + random(0,3);
    if(destX < posX)
      deltaX = -deltaX;
    deltaY = moveSpeed + random(0,3);
    if(destY < posY)
      deltaY = -deltaY;

    moveTimeGrain = MIN_TIME_GRAIN + random(0,MIN_TIME_GRAIN/2);
    
    // set new event in future time
    randomMouseMovePeriod = 5000 + random(1000, 10000);
    Serial1.print("next mouse even in ");
    Serial1.print(randomMouseMovePeriod);
    Serial1.println("ms");
    moveOn = true;
    stopX = false;
    stopY = false;
    
  }

  // Process move of mouse to destination
  if((millis() - moveEventTimer) > moveTimeGrain && moveOn)  {
    moveEventTimer = millis();
    
    if(abs(posX - destX) > (10))
      posX += deltaX;
    else
      stopX = true;
      
    if(abs(posY - destY) > (10))
      posY += deltaY;
    else
      stopY = true;

    if(stopX && stopY)
      moveOn = false;
    else {
      Mouse.move(stopX ? 0 : deltaX, stopY ? 0 : deltaY, 0);
      Serial1.print("Mouse pos: ");
      Serial1.print(posX);
      Serial1.print(" ");
      Serial1.println(posY);
    }
  }
}
