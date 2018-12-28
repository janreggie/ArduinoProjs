#include <LiquidCrystal.h>
#include <Keypad.h>

// liquid crystal display
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

// numeric keypad
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Key map
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 14, 15, 16, 17 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 10, 9, 8 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// variables to be used later
unsigned int inp = 0;
char toPrint[16];  // the thing to be printed no shit sherlock
int op1 = 0, op2 = 0;
unsigned int opR = 0;  // 0 for +, 1 for -, 2 for *, 3 for /
double ans = 0;  // answer.
//unsigned int pos = 0;

void setup() {
  // set up LCD
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("TALAPINDUTAN");
  lcd.setCursor(0, 1);
  lcd.print("DELA CRUZ, ONG");
  delay(1000);
  Serial.begin(57600);

  lcd.clear();
  lcd.setCursor(0, 0);
  printMeTatay();
  lcd.print(toPrint);
  lcd.setCursor(0, 1);
  lcd.print(ans, 2);
}

void loop() {
  // determine the input
  char key = kpd.waitForKey();
  if (key) {
    switch (key) {
      case '*':  // operator
        opR += 1;
        opR = opR % 4;
        break;
      case '#':  // clear
        op1 = 0;
        op2 = 0;
        break;
      default:
        Serial.print("Input is ");
        Serial.println(key, DEC);
        op1 = op1 * 10 + floor(op2 / 10);
        op1 = op1 % 100;
        // check whether input is naught
        op2 = op2 * 10 + (key - 48);
        op2 = op2 % 100;
    }
  }

  // now print the result
  if (opR == 0) ans = op1 + op2;
  else if (opR == 1) ans =  op1 - op2;
  else if (opR == 2) ans = (double) op1 * (double) op2;
  else if (opR == 3)
    if (op2 != 0) ans = (double) op1 / (double) op2;

  lcd.clear();
  lcd.setCursor(0, 0);
  printMeTatay();
  lcd.print(toPrint);
  lcd.setCursor(0, 1);
  if (key) {
    Serial.print("Output is ");
    Serial.println(ans);
  }
  if (opR == 3 && op2 == 0) lcd.print("Undefined!");
  else lcd.print(ans, 2);

  delay(100);
}

void printMeTatay() {
  char opt;
  if (opR == 0) opt = '+';
  else if (opR == 1) opt = '-';
  else if (opR == 2) opt = '*';
  else if (opR == 3) opt = '/';
  else opt = '?';
  sprintf(toPrint, "%02d%c%02d", op1, opt, op2);
}
