// Bouncing Ball with Colour
// By Conor MacBride

#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 120 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    11
#define CLOCKPIN   13
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

uint32_t ballColor1 = 0xFF0000;; //Initial colour of ball
uint32_t ballColor2 = 0x00FF00;; //Initial colour of ball

//randomSeed(analogRead(1)); //Take a random seed from analog noise from A1 (A1 must NOT be connected to anything)
int ballX1; //Initial ball X position (random)
int ballX2;

//randomSeed(analogRead(1));
int ballY1; //Initial ball Y position (random)
int ballY2;

//randomSeed(analogRead(1));
int ballDir1; //Initially random
int ballDir2;

void setup() {
  strip.begin(); // Initialize pins for output
  strip.setBrightness(20);
  strip.show();  // Turn all LEDs off ASAP

  randomSeed(analogRead(0));
  ballX1 = random(0,14);
  ballY1 = random(0,7);
  ballDir1 = random(3);
  ballX2 = random(0,14);
  ballY2 = random(0,7);
  ballDir2 = random(3);

  Serial.begin(9600);
}

int board[8][15] = { //Coordinate matrix of light strip refering to individual LED id numbers
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
  {15,16,17,18,19,20,21,22,23,24,25,26,27,28,29},
  {30,31,32,33,34,35,36,37,38,39,40,41,42,43,44},
  {45,46,47,48,49,50,51,52,53,54,55,56,57,58,59},
  {60,61,62,63,64,65,66,67,68,69,70,71,72,73,74},
  {75,76,77,78,79,80,81,82,83,84,85,86,87,88,89},
  {90,91,92,93,94,95,96,97,98,99,100,101,102,103,104},
  {105,106,107,108,109,110,111,112,113,114,115,116,117,118,119}
};

/////Direction meanings
/// 0 -> NE
/// 1 -> SE
/// 2 -> SW
/// 3 -> NW
/// 4 -> STOPPED
///North, South, East and West not possible

//int batA = 4; //Player 1 bat postion
//int batB = 4; //Player 2 " "

void loop() {
  //Serial.print (ballX1);
  //Serial.print (", ");
  //Serial.print (ballY1);
  //Serial.print (", ");
  //Serial.print (ballDir1);
  //Serial.print ("\n");
  
  float red = ( (float)analogRead(1) / 1023.0 ) * 255.0;
  float green = ( (float)analogRead(2) / 1023.0 ) * 255.0;
  float blue = ( (float)analogRead(3) / 1023.0 ) * 255.0;
  uint32_t ballColor1 = strip.Color(red, green, blue);
  Serial.print (analogRead(1));
  Serial.print (", ");
  Serial.print (analogRead(2));
  Serial.print (", ");
  Serial.print (analogRead(3));
  Serial.print ("\n");

  moveBall(ballX1,ballY1,ballDir1,1,ballColor1); //Move the ball (variables stated to facilitate multiple balls
  moveBall(ballX2,ballY2,ballDir2,2,ballColor2);
  delay(250); //Wait 0.25 sec before moving ball again
}

//int startDir(void){
  //randomSeed(analogRead(0));
  //dir = random(5);
  //return dir;
//}

int moveBall(int ballXv,int ballYv,int ballDirv,int ballID,uint32_t ballColor){
  strip.setPixelColor(board[ballYv][ballXv], 0);//Turn off previous position
  
  if(ballYv == 0){ //Change direction if at top wall
    if(ballDirv == 0){ //from NE
      if(ballXv == 14){ //Traveling towards top right corner (to SW)
        ballDirv = 2;
      }
      else{
        ballDirv = 1; //to SE
      }
    }
    else if(ballDirv == 3){ //From NW
      if(ballXv == 0){ //Traveling towards top left corner
        ballDirv = 1; //To SE
      }
      else{
        ballDirv = 2; //To SW
      }
    }
    else{
      ballDirv = 4; //Something went wrong so stop ball
    }
  }
  else if(ballYv == 7){ //Change direction if at bottom wall
    if(ballDirv == 1){ //from SE
      if(ballXv == 14){ //Traveling towards bottom right corner
        ballDirv = 3; //to NW
      }
      else{
        ballDirv = 0; //to NE
      }
    }
    else if(ballDirv == 2){ //from SW
      if(ballXv == 0){ //Traveling towards bottom left corner
        ballDirv = 0; //to NE
      }
      else{
        ballDirv = 3; //to NW
      }
    }
    else{
      ballDirv = 4; //Something went wrong so stop ball
    }
  }
  else if(ballXv == 0){ //Ball at left wall
    if(ballDirv == 3){ //from NW
      ballDirv = 0; //to NE
    }
    else if(ballDirv == 2){ //from SW
      ballDirv = 1; //to SE
    }
    else{
      ballDirv = 4; //Something went wrong so stop ball
    }
  }
  else if(ballXv == 14){ //Ball at right wall
    if(ballDirv == 0){ //from NE
      ballDirv = 3; //to NW
    }
    else if(ballDirv == 1){ //from SE
      ballDirv = 2; //to SW
    }
    else{
      ballDirv = 4; //Something went wrong so stop ball
    }
  }
  
  if(ballDirv == 0){ //NE
    ballXv = ballXv + 1;
    ballYv = ballYv - 1;
  }
  //else if(ballDirv == 1){ //E
    //ballXv = ballXv + 1;
    //ballYv = ballYv + 0;
  //}
  else if(ballDirv == 1){ //SE
    ballXv = ballXv + 1;
    ballYv = ballYv + 1;
  }
  else if(ballDirv == 2){ //SW
    ballXv = ballXv - 1;
    ballYv = ballYv + 1;
  }
  //else if(ballDirv == 4){ //W
    //ballXv = ballXv - 1;
    //ballYv = ballYv + 0;
  //}
  else if(ballDirv == 3){ //NW
    ballXv = ballXv - 1;
    ballYv = ballYv - 1;
  }
  else if(ballDirv == 4){ //STOPPED
    ballXv = ballXv + 0;
    ballYv = ballYv + 0;
  }
  strip.setPixelColor(board[ballYv][ballXv], ballColor);//Turn on new position
  strip.show();                     // Refresh strip

  if(ballID == 1){
    ballX1 = ballXv;
    ballY1 = ballYv;
    ballDir1 = ballDirv;
  }
  else if(ballID == 2){
    ballX2 = ballXv;
    ballY2 = ballYv;
    ballDir2 = ballDirv;
  }
}

