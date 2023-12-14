#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C


#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_ADDR);

// Bestämmer vilken pin som knappen ska vara på 
const int buttonPin2 = 2;  // Knapp för att mata in morsekod

// Skapar de behövande variablerna och ger de värden
unsigned long startTime;
bool button2Set = false;
bool button2NotPressed = true;
int button2PressedTimer = 0;
int button2NotPressedTimer = 0;
int x = 0;
int i = 0;

// Skapar arryer för tecken och vilket mönster av siffra som passar till det tecknet
char alpha[43] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789+/=?!-";
short morse[43]={12000, 21110, 21210, 21100, 10000, 11210, 22100, 11110, 11000, 12220, 21200, 12110, 22000, 21000, 22200, 12210, 22120, 12100, 11100, 20000, 11200, 11120, 12200, 21120, 21220, 22110, 22220, 22222, 12222, 11222, 11122, 11112, 11111, 21111, 22111, 22211, 22221, 12121, 21121, 21112, 11221, 21212, 21111};

// Skapar arryen som får in värden som sedan ska matas ut på skärmen
long feedMorse = -1; //nnnn
char feedAlpha[99] = "";

// Skapar variabler för alfha-arryens och för morse-arryens index
int feedAlphaIndex = 0;
int feedMorseIndex = 0;

void setup() {
  // Pinnen för knappen (2)
  pinMode(buttonPin2, INPUT);

  // Visar adafruits intro och rensar skärmen
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Kolla om knapp 2 är nedtryckt
  if (digitalRead(buttonPin2) == HIGH) {
    button2PressedTimer += 1;  
    
    button2NotPressedTimer = 0;
    
    }
  else {
    button2NotPressedTimer += 1;
    }
  // Kolla om knappen varit nertryckt i 1 kodgenomkörningar
  if (button2PressedTimer == 1){
    button2Set = true;   
    button2NotPressed = false;
    }
  
  // Utvärderar om lång eller kort och lägger till i feedMorse
  if ((digitalRead(buttonPin2) != HIGH) and (button2Set)){
    button2Set = false;
    button2NotPressed = true;
    display.setTextSize(1);
    display.setCursor(x, 0);
    Serial.println(button2PressedTimer);

    // Kolla om knapp nertryckt är större eller lika med 3 kodgenomkörningar
    if (button2PressedTimer >= 3) {
      if (feedMorseIndex == 0){ 
        feedMorse += 20000 ;}
      if (feedMorseIndex == 1){ 
        feedMorse += 2000 ;}
      if (feedMorseIndex == 2){ 
        feedMorse += 200 ;}
      if (feedMorseIndex == 3){ 
        feedMorse += 20 ;}
      if (feedMorseIndex == 4){ 
        feedMorse += 2 ;}
        display.setTextColor(SSD1306_WHITE);
        display.print("_");
        display.display();  
        // Ger mellanrum åt bokstäverna
        x += 10; 
        // Ökar indexet för att ge en ny siffra på en ny plats       
        feedMorseIndex += 1; 
        // Nollställer nertryck-timmern
      button2PressedTimer = 0;
      }
    else {
      if (feedMorseIndex == 0){ 
        feedMorse += 10000 ;}
      if (feedMorseIndex == 1){ 
        feedMorse += 1000 ;}
      if (feedMorseIndex == 2){ 
        feedMorse += 100 ;}
      if (feedMorseIndex == 3){ 
        feedMorse += 10 ;}
      if (feedMorseIndex == 4){ 
        feedMorse += 1 ;}
        display.setTextColor(SSD1306_WHITE);
        display.print(".");
        display.display();
        // Ger mellanrum åt bokstäverna
        x += 10;   
        // Ökar indexet för att ge en ny siffra på en ny plats
        feedMorseIndex += 1;
        // Nollställer nertryck-timmern
      button2PressedTimer = 0;               
      }


      }
    
    
    
     
    // Kollar om knapp har varit nertryckt i 10 kodgenomkörningar eller då alla platser har blivit fyllda i indexet
    if ((button2NotPressedTimer == 10) or (feedMorseIndex > 4)) {
      display.setCursor(0, 15);
      // Kollar igenom arrayen morseCode för att se om den matchar med något värde
      for (i = 0; i < sizeof(morse)/sizeof(morse[0]); i++ ){
         // Om värde är hittat - lägg till tecken i feedAlfha och sedan printa ut de på skärmen
         if (morse[i] == feedMorse){
            // Rensar diplayen
            display.clearDisplay();
            feedAlpha[feedAlphaIndex] = alpha[i];
            feedAlphaIndex += 1;
            display.setTextSize(2);
            display.print(feedAlpha);
            display.setTextColor(SSD1306_WHITE);
            display.display();
            button2NotPressedTimer = 0;
        }  
        

      }
      feedMorse = 0;
      feedMorseIndex = 0;
      x = 0;

    }
  if (button2NotPressedTimer == 50) {
    // Rensar Dispalyen
    display.clearDisplay();
    display.display();

    // Nollställer variablerna
    feedAlphaIndex -= 1;
    feedMorseIndex = 0;
    feedMorse = 0;
    x = 0;

    // Visar ett medelande på skärmen
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("Change your latest");
    display.display();
    
    // Väntar 2 sekunder innan den resnar displayen
    delay(2000);

    // Rensar displayen igen
    display.clearDisplay();
    display.display();
  }
    if (button2NotPressedTimer == 80) {

          // Rensar displayen
    display.clearDisplay();
    display.display();

    // Resetar variablerna
    feedAlphaIndex = 0;
    feedMorseIndex = 0;
    feedMorse = 0;
    x = 0;

    // Visar ett medelande
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("Start from position 1");
    display.display();

    // Väntar 2 sekunder innan den resnar displayen
    delay(2000);

    // Rensar displayen igen
    display.clearDisplay();
    display.display();
    }
    
  



      

      



      
  
  // Saktar ner hastigheten som koden loppas igenom 
  delay(100);
}
