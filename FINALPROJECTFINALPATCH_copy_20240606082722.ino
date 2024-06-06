#include <Adafruit_CircuitPlayground.h>

//Variable intiation for character stats | Got the struct idea from https://www.w3schools.com/cpp/cpp_structs.asp 
struct Character {
  const char* name; //
  int health;
  int minAttackPower;
  int maxAttackPower;
};

// Function declarations
void setup();
void loop();
void printStatus(Character& character); // printing the status of a character to the Serial Monitor
void attack(Character& attacker, Character& defender); //adjusts the defenders health
bool checkGameOver(Character& player, Character& boss); //Returns a true or false through checker player and boss health values
void flashRed(); //Calls the red flashing leds function
void playHitSound(); //Quick sound plays after hitting enemy
int getRandomAttackPower(int minPower, int maxPower); // generates a random attack power in the range
void playVictoryTheme(); //victory theme that plays when the player gets boss hp to 0
void playDefeatTheme(); //defeat theme that plays when the boss gets player hp to 0
void setAllLedsColor(uint8_t red, uint8_t green, uint8_t blue); //Got this from offfical arduino website

Character player = {"Player", 100, 25, 75}; // player name, HP value, minimum attack value, maximum attack value
Character boss = {"Boss", 200, 5, 30}; // boss name, HP value, minimum attack value, maximum attack value

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  while (!Serial) { //Got this idea of a while loop from the arduino official fourms it basically just waits for the serial monitor to connect
    ; 
  }
  Serial.println("Welcome to the Player vs Boss Battle Game!");
  printStatus(player);
  printStatus(boss);
  Serial.println("Press the right button to attack.");
  Serial.println("Slide the switch to the right to turn off the game.");
}

void loop() {
  // Check if the switch is turned off
  if (CircuitPlayground.slideSwitch()) {
    Serial.println("Game turned off.");
    while (true) {
      ; // Stop the game
    }
  }

  // Check if the right button is pressed
  if (CircuitPlayground.rightButton()) {
    attack(player, boss);
    if (checkGameOver(player, boss)) return;
    delay(500); // Delay to avoid multiple reads of the button press
    attack(boss, player);
    if (checkGameOver(player, boss)) return;
    delay(500); // Delay to simulate turn-based gameplay
  }
}

void printStatus(Character& character) {
  Serial.print(character.name);
  Serial.print(" - Health: ");
  Serial.println(character.health);
}

void attack(Character& attacker, Character& defender) {
  int attackPower = getRandomAttackPower(attacker.minAttackPower, attacker.maxAttackPower);
  Serial.print(attacker.name);
  Serial.print(" attacks ");
  Serial.print(defender.name);
  Serial.print(" for ");
  Serial.print(attackPower);
  Serial.println(" damage!");

  defender.health -= attackPower;

  if (defender.health < 0) defender.health = 0;

  printStatus(defender);

  if (defender.name == player.name) {
    flashRed(); // Flash red LEDs when the player gets hit
    playHitSound(); // Play sound when the player gets hit
  }
}

bool checkGameOver(Character& player, Character& boss) {
  if (player.health <= 0) {
    Serial.println("Boss wins!");
    setAllLedsColor(255, 0, 0); // Set all LEDs to red
    playDefeatTheme(); // Play defeat theme
    while (true) {
      ; // Stop the game
    }
  }
  if (boss.health <= 0) {
    Serial.println("Player wins!");
    setAllLedsColor(0, 255, 0); // Set all LEDs to green
    playVictoryTheme(); // Plays victory theme


    while (true) 
    {  
    }
  }
  return false;
}


//Function from my SmartPlant program
void flashRed() {
  for (int i = 0; i < 3; i++) {
    setAllLedsColor(255, 0, 0); 
    delay(100); 
    CircuitPlayground.clearPixels(); 
    delay(100); 
  }
}




//Short 55ms sound that plays after hitting the boss
void playHitSound() {
  CircuitPlayground.playTone(400, 55);
}





int getRandomAttackPower(int minPower, int maxPower) {
  return random(minPower, maxPower); 
}




void playVictoryTheme() { //Took from musicbox program but cut it down
  int melody[] = {525, 587};
  int noteDurations[] = {200, 200};
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    CircuitPlayground.playTone(melody[thisNote], noteDurations[thisNote]);
    delay(noteDurations[thisNote] * 1.30); 
  }
}





void playDefeatTheme() { //Took from my musicbox program but cut it down
  int melody[] = {980, 880};
  int noteDurations[] = {200, 200};
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    CircuitPlayground.playTone(melody[thisNote], noteDurations[thisNote]);
    delay(noteDurations[thisNote] * 1.30); 
  }
}





void setAllLedsColor(uint8_t red, uint8_t green, uint8_t blue) { //Took this function from the arduino website
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, red, green, blue);
  }
}
