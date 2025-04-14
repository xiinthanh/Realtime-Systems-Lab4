#include <scheduler.h>
#define LED_PIN 48
#define D3 6
#define D4 7
#define D5 8
#define D6 9

int led_status = 0;
void test_blinky(){
  led_status = 1 - led_status;
  digitalWrite(LED_PIN, led_status);
}

#define OFF 0
#define RED 1
#define GRE 2
#define YEL 3


void setLightOff(int ID1, int ID2) {
  digitalWrite(ID1, LOW);
  digitalWrite(ID2, LOW);
}

void setLightRed(int ID1, int ID2) {
  digitalWrite(ID1, HIGH);
  digitalWrite(ID2, HIGH);
}

void setLightGreen(int ID1, int ID2) {
  digitalWrite(ID1, HIGH);
  digitalWrite(ID2, LOW);
}

void setLightYellow(int ID1, int ID2) {
  digitalWrite(ID1, LOW);
  digitalWrite(ID2, HIGH);
}


#define INIT -1
#define RED_GRE 0
#define RED_YEL 1
#define GRE_RED 2
#define YEL_RED 3

int counter = 0;
int two_way_traffic_light_status = INIT;  // INIT will start the traffic light with RED_GRE status


void twoWayTrafficLight() {
  if (counter == 0 && (two_way_traffic_light_status == INIT || two_way_traffic_light_status == YEL_RED)) {
    two_way_traffic_light_status = RED_GRE;
    setLightRed(D3, D4);     // Road A = RED
    setLightGreen(D5, D6);   // Road B = GREEN
    counter = 3;
  } else if (counter == 0 && two_way_traffic_light_status == RED_GRE) {
    two_way_traffic_light_status = RED_YEL;
    setLightRed(D3, D4);     // Road A = RED
    setLightYellow(D5, D6);  // Road B = YELLOW
    counter = 2;
  } else if (counter == 0 && two_way_traffic_light_status == RED_YEL) {
    two_way_traffic_light_status = GRE_RED;
    setLightGreen(D3, D4);   // Road A = GREEN
    setLightRed(D5, D6);     // Road B = RED
    counter = 3;
  } else if (counter == 0 && two_way_traffic_light_status == GRE_RED) {
    two_way_traffic_light_status = YEL_RED;
    setLightYellow(D3, D4);  // Road A = YELLOW
    setLightRed(D5, D6);     // Road B = RED
    counter = 2;
  }

  counter -= 1;
}


void TIMER_ISR(void *pvParameters) {
  
  while(1) {
    SCH_Update();
    vTaskDelay(10);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);

  SCH_Init();
  SCH_Add_Task(test_blinky, 0, 100);

  SCH_Add_Task(twoWayTrafficLight, 0, 100);
  
}

void loop() {
  SCH_Dispatch_Tasks();
}
