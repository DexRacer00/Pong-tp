//bibliotek för displayen
#include "U8glib.h"
#include <math.h>
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

//variabler för att justera och hålla koll på saker som score och positioner
int  player2Input= 0;
int player1Input = 0;
int bat1Y = 0;
int bat2Y = 0;
float ballY = 30.5f;
float ballX = 62.5f;
float hVelocity = 1.0f;
float vVelocity = 0.0f;
int score1 = 0;
int score2 = 0;
int wScore = 2;
void setup() {
Serial.begin(9600);
}


 
funktion för att kolla vinnare
int checkWinner(float ballX) {
  if (ballX >= 127) {
    return 2;
  } else if (ballX <= 0) {
    return 1;
  } else {
    return 0;
  }
}

void loop() {
  //remappar potensiometer så den funkar med displayen och positionerna på displayen
  bat1Y = map(player1Input, 0, 1023, 0, 52);
  bat2Y = map(player2Input, 0, 1023, 0, 52);
  
  player1Input = map(player1Input, 0, 1023, 0, 52);
  player2Input = map(player2Input, 0, 1023, 0, 52);

 
  // resettar bollen om någon vinner genom att kolla om vinnar functionen retunerar 1 eller 2

  if (checkWinner(ballX) == 1) {
    
    delay(2000);
    score1++;
    ballY = 30.5f;
    ballX = 62.5f;
    hVelocity = 1.0f;
    vVelocity = 0.0f;
    

  } else if (checkWinner(ballX) == 2){

    delay(2000);
    score2++;
    ballY = 30.5f;
    ballX = 62.5f;
    hVelocity = 1.0f;
    vVelocity = 0.0f;

  }
  // kallar på functionen draw eller victory
  oled.firstPage();
  do {
    draw();
  } while ((score1 || score2 < wScore) && oled.nextPage());

  do {
    victory();
  } while ((score1 || score2 == wScore)&& oled.nextPage());
 
  player1Input = analogRead(A0);
  //Serial.println(player1Input);
  delay(1);        
  player2Input = analogRead(A1);
  //Serial.println(player2Input);
  delay(1);

  // kollar om bollen studsar på bat och vart. Beroende på detta ger den bollen reverced x värde och beroende på vart på batet den studsar ger den posetivt
  //nehativt eller 0 som y värde 
  if (ballX >= 121.0f && ballY >= bat1Y && ballY <= (bat1Y + 5.0f)) {
    hVelocity *= -1;
    vVelocity = -1;
  } else if (ballX <= 6.0f && ballY >= bat2Y && ballY <= (bat2Y + 5.0f)) {
    hVelocity *= -1;
    vVelocity = -1;
  } else if (ballX >= 121.0f && ballY == (bat1Y + 6.0f)) {
    hVelocity *=-1;
    vVelocity = 0;
  } else if (ballX <= 6.0f && ballY == (bat2Y + 6.0f)) {
    hVelocity *= -1;
    vVelocity = 0;
  } else if (ballX <= 6.0f && ballY >= (bat2Y + 7.0f) && ballY <= (bat2Y + 11.0f)) {
    hVelocity *= -1;
    vVelocity = 1;
  } else if (ballX >= 121.0f && ballY >= (bat1Y + 7.0f) && ballY <= (bat1Y + 11.0f)) {
    hVelocity *= -1;
    vVelocity = 1;
  }
        
// kollar om bollen studsar i taket eller golvet och vänder då på y värdet
  if (ballY <= 0 || ballY >= 63){
    vVelocity *= -1.0;
  }
     
// här är hastigheten och rörelsen för bollen
  
  ballX += hVelocity;
  ballY += vVelocity;
  //Serial.println(ballX);
  //Serial.println(hVelocity);
  Serial.println(bat1Y); //player2Input är inte y-värdet utan värdet på snurrspaken
  //Serial.println(ballY);
  delay (10);
}
  

// här printas allt som du ser på skärmen som boll och bat

void draw() {
    oled.setFont(u8g_font_helvB10);
    oled.drawLine(123, player1Input, 123, player1Input+11);
    oled.drawLine(5, player2Input, 5, player2Input+11);
    oled.drawPixel(ballX+1, ballY+1);
    oled.drawPixel(ballX, ballY);
    oled.drawPixel(ballX, ballY+1);
    oled.drawPixel(ballX+1, ballY);
  }
  // här är functionen för victory screan som kollar om score = vinst och visar då denna skärm
  void victory () {
      oled.setFont(u8g_font_helvB10);
     if (score1 == wScore) {
      oled.drawStr(0, 30, ("player 1 is the"));
      oled.drawStr(45, 45, ("WINNER"));
      hVelocity = 0;
      vVelocity = 0;
     } else if (score2 == wScore) {
      oled.drawStr(0, 30, ("player 2 is the"));
      oled.drawStr(45, 45, ("WINNER"));
      hVelocity = 0;
      vVelocity = 0;
      }
    
 }
