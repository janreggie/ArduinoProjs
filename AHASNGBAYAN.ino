#include <SparkFunColorLCDShield.h>
#include <LinkedList.h>

/*
   AHAS NG BAYAN
   DELA CRUZ, ONG
   BROUGHT TO YOU BY PISTING SAWA
*/

/*
   A BRIEF NOTE ON DIRECTION
   WHEN WE TALK OF INCREASING LENGTH (x), WE TALK OF MOVING DOWNWARDS
   WHEN WE TALK OF INCREASING HEIGHT (Y), WE TALK OF MOVING RIGHTWARDS
   THIS MAY SEEM UNINTUITIVE TO THE UNFAMILIAR READER, BUT PLEASE BEAR WITH US.
   HENCEFORTH, IF THE SNAKE IS MOVING "TO THE LEFT", THE NEW HEAD OF THE SNAKE
   WILL HAVE ITS Y-COORDINATE DECREASED.
*/

// length ("x") and height ("y") as number of blocks
// as well as width per block
#define LENGTH 26
#define HEIGHT 26
#define WIDTH 5
#define DARKTHEME 1
#if DARKTHEME == 0
  #define DARKGREEN 0x051
  #define FOREGROUND BLACK
  #define BACKGROUND WHITE
  #define PEACHCOLOR RED
#else
  #define DARKGREEN 0x051
  #define FOREGROUND WHITE
  #define BACKGROUND BLACK
  #define PEACHCOLOR BLUE
#endif
LCDShield lcd;
const unsigned int S1 = 2;  // pin for S1
const unsigned int JOYX = A0, JOYY = A1;  // pins for josytick X and Y
unsigned int del;  // delay
bool obstacles[LENGTH][HEIGHT] = {0};  // an array of obstacles (1 is impassable)
struct Node {
  unsigned int xpos, ypos;
  Node (unsigned int x, unsigned int y) {
    this->xpos = x;
    this->ypos = y;
  }
  Node () {
    this->xpos = 0;
    this->ypos = 0;
  }
  bool operator== (const Node& d) {
    return (this->xpos == d.xpos && this->ypos == d.ypos);
  }
  bool operator!= (const Node& d) {
    return !(*this == d);
  }
};
LinkedList<Node> snek;  // snek[0] is HEAD; snek[snek.length()-1] is TAIL
Node prut;  // the prut
bool isLoss = 0;  // check whether lost or not
enum Direction  {LEFT, RIGHT, UP, DOWN, CENTER};
Direction dir;  // determine direction of the snek
unsigned int score;  // score (no shit)
char buff[16];  // char buffer for later.

void setup() {
  // put your setup code here, to run once:
  // define obstacles
  for (int i = 0; i < HEIGHT; ++i) {
    obstacles[0][i] = 1;
    obstacles[LENGTH - 1][i] = 1;
  }
  for (int i = 0; i < LENGTH; ++i) {
    obstacles[i][0] = 1;
    obstacles[i][HEIGHT - 1] = 1;
  }

  // display the text
  lcd.init(PHILIPS);
  lcd.contrast(-58);
  lcd.clear(BACKGROUND);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(S1), startGame, RISING);
}

void drawNode(const Node & n, const int & color) {
  // draw the Node object n in the board
  // dependent on the WIDTH variable
  lcd.setRect(n.xpos * WIDTH, n.ypos * WIDTH, (n.xpos + 1)*WIDTH, (n.ypos + 1)*WIDTH - 1, 1, color);
}

Direction whatDir() {
  // determine first whether it is UP or DOWN
  // then determine whether LEFT or RIGHT
  // which means joystick on upper-left will register as UP
  if (analogRead(JOYY) > 767) return UP;
  else if (analogRead(JOYY) < 255) return DOWN;
  else if (analogRead(JOYX) > 767) return LEFT;
  else if (analogRead(JOYX) < 255) return RIGHT;
  else return CENTER;
}

void startGame() {
  detachInterrupt(digitalPinToInterrupt(S1));
  // clear everything bitch
  Serial.println("A NEW CHALLENGER HAS ENTERED THE RANKS");
  lcd.clear(BACKGROUND);
  del = 1200000;
  delay(1000);
  score = 0;
  isLoss = 0;
  randomSeed(analogRead(A5));  // set random seed for randomness
  // draw the obstacles ;)
  for (unsigned int i = 0; i < LENGTH; ++i) {
    for (unsigned int j = 0; j < HEIGHT; ++j) {
      if (obstacles[i][j]) drawNode(Node(i, j), FOREGROUND);
    }
  }
  // initialize the snake
  snek.clear();
  // first three elements
  snek.add(Node(floor(LENGTH / 2), floor(HEIGHT / 2)));
  snek.add(Node(floor(LENGTH / 2), floor(HEIGHT / 2) + 1));
  snek.add(Node(floor(LENGTH / 2), floor(HEIGHT / 2) + 2));
  dir = LEFT;

  // initialize prut
  do {
    prut = Node(random(1, LENGTH), random(1, HEIGHT));
  } while (!isValid(prut.xpos, prut.ypos));

  // now print the first snek
  for (int i = 0; i < snek.size(); ++i) {
    // print using WIDTH and stuff
    Node tmp = snek.get(i);
    drawNode(snek.get(i), DARKGREEN);
  }
  // and print the peach
  drawNode(prut, PEACHCOLOR);

  // now let's start playing
  bool isEaten = 0;
  while (!isLoss) {
    // first determine the new Direction dir
    Direction newDir = whatDir();
    if (newDir == CENTER);
    else if (newDir < 2 xor dir < 2) dir = newDir;
    // then create the new snek by creating a new node
    Node newHead;
    switch (dir) {
      case LEFT:
        newHead = Node(snek.get(0).xpos, snek.get(0).ypos - 1);
        break;
      case RIGHT:
        newHead = Node(snek.get(0).xpos, snek.get(0).ypos + 1);
        break;
      case UP:
        newHead = Node(snek.get(0).xpos - 1, snek.get(0).ypos);
        break;
      case DOWN:
        newHead = Node(snek.get(0).xpos + 1, snek.get(0).ypos);
        break;
    };
    snek.unshift(newHead);
    // and then pop the last element
    Node byeTail;
    if (prut != newHead) {
      byeTail = snek.pop();  // goodbye~~~
    }
    else {
      isEaten = 1;
      do {
        prut = Node(random(1, LENGTH), random(1, HEIGHT));
      } while (!isValid(prut.xpos, prut.ypos));
    }
    // then check for validity and whatnot
    // first check the new head if it collides
    if (obstacles[newHead.xpos][newHead.ypos]) isLoss = 1;
    // then check for EVERY GODDAMN ELEMENT IN THE SNEK
    for (int i = 1; i < snek.size(); ++i) {
      if (snek.get(0) == snek.get(i)) isLoss = 1;
    }
    // then display the snek
    if (isEaten) {
      // display the new fruit
      drawNode(prut, PEACHCOLOR);
      isEaten = not isEaten;
      ++score;
    }
    else {
      drawNode(byeTail, BACKGROUND);  // remove the byeTail
    }
    drawNode(newHead, DARKGREEN);

    // and then delay
    delay(del);
  }
  // and then initiate game over
  lcd.setStr("GAME OVER", 43, 25, FOREGROUND, BACKGROUND);
  sprintf(buff, "Score: %03d", score);
  lcd.setStr(buff, 64, 20, FOREGROUND, BACKGROUND);
  lcd.setStr("S1 to play", 80, 20, FOREGROUND, BACKGROUND);
  Serial.println("Ooh game over.");
  while (1) {
    if (digitalRead(S1) == LOW) {
      Serial.println("Okay escape.");
      break;
    }
    delay(1000);
  }
  lcd.clear(BACKGROUND);
  delay(10000);
  attachInterrupt(digitalPinToInterrupt(S1), startGame, RISING);
}

bool isValid (unsigned int x, unsigned int y) {
  // determine whether (x,y) is a valid place to put a fruit
  if (obstacles[x][y] == 1) return 0;
  for (int i = 0; i < snek.size(); ++i) {
    if (snek.get(i) == Node(x, y)) return 0;
  }
  return 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setStr("SNEK", 23, 43, DARKGREEN, BACKGROUND);
  lcd.setStr("Version 1.69", 43, 18, DARKGREEN, BACKGROUND);
  lcd.setStr("Ong, Dela Cruz", 63, 7, DARKGREEN, BACKGROUND);
  lcd.setStr("S1 to play", 80, 20, FOREGROUND, BACKGROUND);
  delay(1000);
}
