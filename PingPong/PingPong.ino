// HASN'T BEEN TESTED ON A LED MATRIX YET

// Arduino Ping Pong
// By Conor MacBride

#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <elapsedMillis.h>

elapsedMillis timeElapsed;

#define WIDTH 8
#define HEIGHT 15

// Here's how to control the LEDs from any two pins:
#define DATAPIN    11
#define CLOCKPIN   13
Adafruit_DotStar strip = Adafruit_DotStar(
  WIDTH * HEIGHT, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

uint32_t paddleColor = 0x00FFFF; // Paddle color (yellow)
uint32_t ballColor = 0xFFFFFF; // Ball color (white)

int player1 = 0; // Initial scores
int player2 = 0;

int i, j;
int board[HEIGHT][WIDTH];
int walls[HEIGHT][WIDTH];
int buildBoard () {
  for ( i = 0; i < HEIGHT; i++ ) {
    for ( j = 0; j < WIDTH; j++ ) {
      board[i][j] = ( i * WIDTH ) + j;
    }
  }
  for ( i = 0; i < HEIGHT; i++ ) {
    walls[i][0] = 270;
    walls[i][HEIGHT - 1] = 90;
    for ( j = 1; j < WIDTH - 1; j++ ) {
      walls[i][j] = 999;
    }
  }
}

int ballX, ballY, ballDir;
int startBall () { // Set initial ball variables
  ballX = floor( 0.5 * WIDTH );
  ballY = floor( 0.5 * HEIGHT);
  randomSeed(analogRead(0));
  int z = random(1,4);
  if ( z == 1 ) {
    ballDir = 45;
  } else if ( z == 2 ) {
    ballDir = 135;
  } else if ( z == 3 ) {
    ballDir = 225;
  } else if ( z == 4 ) {
    ballDir = 315;
  }
}

int updatePaddle (int paddle, float width) {
  float location = analogRead(paddle) / 1023.0; // Read loaction from potentiometer
  int wid = floor(WIDTH * width);               // Width in pixels
  int loc = WIDTH - wid;                        // First pixel
  
  int dir, wall;
  if ( paddle == 1 ) { // Player 1
    i = 0;
    dir = 0;
    wall = 441;
  } else {             // Player 2
    i = HEIGHT - 1;
    dir = 180;
    wall = 442;
  }
  
  for ( j = 0; j < WIDTH; j++ ) {           // Reset paddle
    strip.setPixelColor(board[i][j], 0);           // Turn off previous
    walls[i][j] = wall;
  }
  for ( j = loc - 1; j < loc + wid; j++ ) { // Set current paddle
    strip.setPixelColor(board[i][j], paddleColor); // Turn on new
    walls[i][j] = dir;
  }
  
  if ( walls[0][0] == 0 ) { // Check and change walls at corners
    walls[0][0] = 315;
  } else if ( walls[0][WIDTH - 1] == 0 ) {
    walls[0][WIDTH - 1] = 45;
  } else if ( walls[HEIGHT - 1][0] == 180 ) {
    walls[HEIGHT - 1][0] = 225;
  } else if ( walls[HEIGHT - 1][WIDTH - 1] == 180 ) {
    walls[HEIGHT - 1][WIDTH - 1] = 135;
  }
}

int bounce (int wall, int dir) { // Calculate the new direction based on wall orientation and ball direction
  int a = 2 * wall - dir;
  int bearing = a % 360;
  if(bearing < 0) {
    bearing+=360;
  }
  return bearing; // In degrees
}

int moveBall () {
  int prev = walls[ballY][ballX]; // Check if at a wall
  if ( prev == 0 || prev == 45 || prev == 90 || prev == 135 || prev == 180 || prev == 225 || prev == 270 || prev == 315 ) {
    //if ( prev == 0 || prev == 45 || prev == 315 ) {
    //  player1++;
    //  Serial.print ("Current score: " + player1 + ":" + player2);
    //} else if ( prev == 135 || prev == 180 || prev == 225 ) {
    //  player2++;
    //  Serial.print ("Current score: " + player1 + ":" + player2);
    //}
    ballDir = bounce(prev, ballDir);
  }
  
  strip.setPixelColor(board[ballY][ballX], 0); // Turn off previous position
  
  if ( ballDir == 0 ) { // Change coordiate according to direction of motion
    //ballX;
    ballY--;
  } else if ( ballDir == 45 ) {
    ballX++;
    ballY--;
  } else if ( ballDir == 90 ) {
    ballX++;
    //ballY;
  } else if ( ballDir == 135 ) {
    ballX++;
    ballY++;
  } else if ( ballDir == 180 ) {
    //ballX;
    ballY++;
  } else if ( ballDir == 225 ) {
    ballX--;
    ballY++;
  } else if ( ballDir == 270 ) {
    ballX--;
    //ballY;
  } else if ( ballDir == 315 ) {
    ballX--;
    ballY--;
  }
  
  strip.setPixelColor(board[ballY][ballX], ballColor); // Turn on new position
  strip.show(); // Refresh strip
}

int currentScore () {
  Serial.print ("Current score: ");
  Serial.print (player1);
  Serial.print (":");
  Serial.print (player2);
  Serial.print ("\r\n");
}

void setup() {
  strip.begin(); // Initialize pins for output
  strip.setBrightness(20);
  strip.show();  // Turn all LEDs off ASAP

  buildBoard();
  startBall();

  Serial.begin(9600);

  Serial.print ("╔═════════════════════════════════════════════╗\r\n");
  Serial.print ("║ Ping Pong for Arduino                       ║\r\n");
  Serial.print ("║                           by Conor MacBride ║\r\n");
  Serial.print ("╚═════════════════════════════════════════════╝\r\n");
  Serial.print ("\r\nNEW GAME\r\n");
  currentScore();
}

void loop() {
  updatePaddle(1, 0.5);
  updatePaddle(2, 0.5);
  if ( timeElapsed < 500 ) {
    moveBall();
    if ( walls[ballY][ballX] == 441 ) {
      player2++;
      currentScore();
      startBall();
    } else if ( walls[ballY][ballX] == 442 ) {
      player1++;
      currentScore();
      startBall();
    }
    timeElapsed = 0;
  }
  strip.show(); // Update strip
  if ( ( player1 >= 10 || player2 >= 10 ) && player1 != player2 ) {
    if ( player1 > player2 ) {
      Serial.print ("The winner is player 1!\r\n");
    } else {
      Serial.print ("The winner is player 2!\r\n");
    }
    Serial.print ("\r\nDo you want to play again? (Y/n) \r\n");
    if ( Serial.available() > 0 ) {
      char val = Serial.read();
      if ( val == 89 || val == 121 ) {
        Serial.print ("\r\nNEW GAME\r\n");
        
        player1 = 0;
        player2 = 0;
        currentScore();
        
        strip.begin(); // Initialize pins for output
        strip.setBrightness(20);
        strip.show();  // Turn all LEDs off ASAP

        buildBoard();
        startBall();
      } else {
        exit(0);
      }
    }
  }
}

