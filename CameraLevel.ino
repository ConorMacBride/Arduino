// Camera Level
// By Conor MacBride

#define PI 3.14159265 //Define a value for Pi

int led[] = {2,3,4,5,6,7,8,9,10}; //Array of pins LEDs are connected to
//           N   E   S   W  |Balanced

int x_pin = 1; //Set the x pin of the accelerometer
int y_pin = 0; //Set the y pin of the accelerometer

void setup() { 
  int i;
  for( i = 0; i <= 8; i++ ){
    pinMode(led[i], OUTPUT); //Set all of the LED pins as outputs
  }
  
  Serial.begin (9600); //Open the serial port (for testing)
}

void loop() {
  float acc_x = analogRead (x_pin); // Get a number related to X acceleration
  float acc_y = analogRead (y_pin); // Get a number related to Y acceleration
  
  float x_max = 483.0; //Value of x when board is at the maximum angle
  float x_min = 153.0; //Value of x when board is at the minimum angle
  float x_level = 345.0; //Value of x when board is at 0°
  float x = ( ( acc_x - x_level ) / ( x_max - x_min ) ) * 180.0 ; //Value of x tilt in degrees

  float y_max = 509.0; //Value of y when board is at the maximum angle
  float y_min = 186.0; //Value of y when board is at the minimum angle
  float y_level = 322.0; //Value of y when board is at 0°
  float y = ( ( acc_y - y_level ) / ( y_max - y_min ) ) * 180.0 ; //Value of y tilt in degrees

  float compass;
  if( x > 5.0 ){
    if( abs(y) > 0.0 ){
      //Quadrant 1 or 4
      compass = ( PI / 2.0 ) - atan( y / x ); //Calculate bearing
    }
    else{
      compass = PI / 2.0; //Avoid divide by 0 error
    }
  }
  else if( x < -5.0 ){
    if( abs(y) > 0.0 ){
      //Quadrant 2 or 3
      compass = ( ( 3.0 / 2.0 ) * PI ) - atan( y / x ); //Calculate bearing
    }
    else{
      compass = ( 3.0 / 2.0 ) * PI; //Avoid divide by 0 error
    }
  }
  else{
    if( abs(y) < 5 ){
      compass = -1.0; //±5° in x and y region is taken to be balanced (-1.0 is not a bearing – just means balanced)
    }
    else if( y > 0 ){
      compass = 0.0;
    }
    else if( y < 0 ){
      compass = PI;
    }
  }

  //Write the current state to the pins
  //Compass points and ranges for these are specified:
  if( compass == -1.0 ){
    //BAL
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], HIGH);//BAL
  }
  else if( ( ( compass >= ( 1.875 * PI ) ) && ( compass <= ( 2.0 * PI ) ) ) || ( ( compass >= ( 0.0 * PI ) ) && ( compass < ( 0.125 * PI ) ) ) ){
    //N
    digitalWrite(led[0], HIGH);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 0.125 * PI ) ) && ( compass < ( 0.375 * PI ) ) ){
    //NE
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], HIGH);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 0.375 * PI ) ) && ( compass < ( 0.625 * PI ) ) ){
    //E
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], HIGH);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 0.625 * PI ) ) && ( compass < ( 0.875 * PI ) ) ){
    //SE
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], HIGH);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 0.875 * PI ) ) && ( compass < ( 1.125 * PI ) ) ){
    //S
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], HIGH);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 1.125 * PI ) ) && ( compass < ( 1.375 * PI ) ) ){
    //SW
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], HIGH);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 1.375 * PI ) ) && ( compass < ( 1.625 * PI ) ) ){
    //W
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], HIGH);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else if( ( compass >= ( 1.625 * PI ) ) && ( compass < ( 1.875 * PI ) ) ){
    //NW
    digitalWrite(led[0], LOW);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], LOW);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], LOW);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], LOW);//W
    digitalWrite(led[7], HIGH);//NW
    digitalWrite(led[8], LOW);//BAL
  }
  else{
    //ERROR (Just a pattern to show something is wrong)
    digitalWrite(led[0], HIGH);//N
    digitalWrite(led[1], LOW);//NE
    digitalWrite(led[2], HIGH);//E
    digitalWrite(led[3], LOW);//SE
    digitalWrite(led[4], HIGH);//S
    digitalWrite(led[5], LOW);//SW
    digitalWrite(led[6], HIGH);//W
    digitalWrite(led[7], LOW);//NW
    digitalWrite(led[8], HIGH);//BAL
  }

  //Some code to allow testing:
  
  int testing = 1; //Change to activate/deactivate serial output
  if( testing == 1 ){
    Serial.print (acc_x);
    Serial.print (", ");
  
    Serial.print (acc_y);
    Serial.print (", ");
  
    Serial.print (x);
    Serial.print (", ");
  
    Serial.print (y);
    Serial.print (", ");
    
    Serial.print (compass);
    Serial.println();
  }
}

