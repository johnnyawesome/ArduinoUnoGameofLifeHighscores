
// Display is a 84x48 pixel monochrome LCD display

//Include Libs
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//Variables
#define Delay 250

const byte SIZEX = 23;
const byte SIZEY = 23;

//bool grid[SIZEX][SIZEY];

bool grid[SIZEX][SIZEY] = {{1,0,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,1,},
{0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,0,0,1,1,1,0,},
{1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,},
{0,0,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,},
{1,1,0,1,0,1,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,0,1,},
{0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,},
{1,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,},
{1,1,1,0,1,1,1,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,0,},
{0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1,},
{0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,},
{1,1,0,1,1,0,1,0,0,1,1,1,0,1,1,0,1,0,0,0,0,1,0,},
{0,1,0,0,1,1,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,0,0,},
{0,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,0,0,0,},
{1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,1,},
{0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,},
{1,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,},
{0,0,1,1,0,1,1,1,1,0,1,1,1,0,1,0,1,0,0,1,0,0,1,},
{1,1,1,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,1,1,0,},
{0,1,0,1,0,1,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1,},
{1,1,1,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,},
{1,0,1,0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,0,1,0,1,1,},
{1,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,},
{1,1,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0,1,1,0,0,0,1,}

};




bool nextGrid[SIZEX][SIZEY];

byte difference = 0;
byte lastdifference = 0;
byte boring = 0;
byte alive;
byte lastalive = 0;

byte lifecount;

int generation;
int highscore;

void setup() {

  display.begin();
  // you can change the contrast around to adapt the display for the best viewing!
  display.setContrast(50);

  display.clearDisplay();
  display.display(); // show splashscreen
  display.clearDisplay();   // clears the screen and buffer  

// CONSTRAIN THE VALUES OF X AND Y because the display is a 84x48 pixel monochrome LCD display

 randomSeed(analogRead(0)); //Initialize Random Number

 clearScreen();

 displaystats();
 
 initialize();
 
 delay(Delay);
 
}

void loop() {

  difference = 0;
  alive = 0;

  clearScreen();

  lifecycle();

  drawPixels();
  
  displaystats();
  
  writedisplay();

  fillnextgrid();

  checksamegrid();

  delay(Delay);
}

  /* //draw a single pixel 
  display.drawPixel(pixx, pixy, BLACK);
  display.display(); 
  delay(2000);
  display.clearDisplay(); */

//Empty Screen
void clearScreen(){
  
 display.clearDisplay(); 
  
}

//Initializes the very first screen with random cells
void initialize(){

  clearScreen();
  displaystats();

  if (generation > highscore){
    highscore = (generation - 25);
  }
  
  generation = 0;

  display.display(); 
  delay(Delay * 10);
  
  for(int x = 0; x < SIZEX;x++){
     for(int y = 0; y < SIZEY;y++){

        //grid[x][y] = random(2);
        display.drawPixel(16 + x, 3 + y, grid[x][y]);
      
     }
  }  
  display.display(); 
  delay(Delay * 15);
}


void lifecycle(){

  for(int x = 0; x < SIZEX; x++){
  
     for(int y = 0; y < SIZEY; y++){

         
         //Count all the neighboring cells
         byte count = neighbours(x, y);


         // The gamerules
         
         // born
         if(grid[x][y] == 0 && count == 3){
          nextGrid[x][y] = 1;
          difference++;
          alive++;
         }


         // Alive and stay alive
         if(grid[x][y] == 1 && count == 2 || count == 3){
          nextGrid[x][y] = 1;
          alive++;
         }
         

         // Alive and die
         if(grid[x][y] == 1 && count < 2 || count > 3){
          nextGrid[x][y] = 0;
          difference++;
          alive--;
         }
    }    
  }   
}

//Writes back all the values that lifecycle() calculated into the temporary 'nextGrid-array' into 'grid-array'
void fillnextgrid(){

  for(int x = 0; x < SIZEX; x++){ 
     for(int y = 0; y < SIZEY; y++){

        grid[x][y] = nextGrid[x][y];      
    }    
  }   
}

//Counts the neighbours and gives back the number of neighbours
byte neighbours(byte x, byte y) {
  return grid[(x + 1) % SIZEX][y] + 
    grid[x][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][y] + 
    grid[x][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + SIZEY - 1) % SIZEY];
}


//Checks if the grid is the same. It does so by ckecking if a cell is born or has died (difference),
//and by comparing the alive count to a previous value.
void checksamegrid(){
  
  if (difference == lastdifference && alive == lastalive) boring++;  

  if (boring >= 25 || generation >= 3000){
    delay(Delay * 3);
    generation = generation + 25;
    initialize();
    boring = 0;
  }
  
  lastdifference = difference;
  lastalive = alive;

  //if the grid is completely empty, no need to wait for 'boring' to count up. initialize immediately.
  lifecount = 0;
  
  for(int x = 0; x < SIZEX; x++){
     
    for(int y = 0; y < SIZEY; y++){
      
      lifecount = lifecount + grid[x][y];  
    }
  }
  
  if (lifecount == 0){   
    clearScreen();
    delay(Delay * 3);
    initialize();
  }
  else generation++;
}

  //Displays stats
void displaystats(){

  //Normalstats
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,33);
  display.print("Alive:");
  display.println(lifecount);
  display.print("Gen:");
  display.print(generation);
  display.print(" ");

  //Highscore rotated 90° to display even large numbers
  display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(45,55);
  display.println("MaxGen:");
  display.print(highscore);
    // revert back to no rotation
  display.setRotation(0);

  //COSMETICS
  //Horizontalline
  display.drawLine(0,30,59,30,BLACK);
  //Verticalline
  display.drawLine(59,0,59,48,BLACK);

  //Box around the game
  display.drawRoundRect(13,0,29,29,2, BLACK);
}
  
//Shows the gamescreen
void drawPixels(){

  for(int x = 0; x < SIZEX; x++){
  
     for(int y = 0; y < SIZEY; y++){
    
      display.drawPixel(16 + x, 3 + y, grid[x][y]);
      
    }    
  }  
}

void writedisplay(){
 
  display.display(); 
}

  
