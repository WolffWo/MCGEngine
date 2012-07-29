#include <MCGBotEngine.h>

MCGBotEngine mcgbotengine(2,3,6,7);

void setup(){

mcgbotengine.drive(true, 100);  
delay(3000);
mcgbotengine.drive(false, 100);

}


void loop() {
  
  
}
