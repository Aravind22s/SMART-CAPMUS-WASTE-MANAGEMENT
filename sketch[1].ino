// blynk app connection code to indicate the alerts 

#define BLYNK_TEMPLATE_ID "TMPL34aQC7Ic5"
#define BLYNK_TEMPLATE_NAME "Smart Waste dustbin Management System"
#define BLYNK_AUTH_TOKEN "XR4GhA62Rbh4ZOPuYoAA13Oj38RE52gP"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

#define PIN_TRIG 26
#define PIN_ECHO 25
#define LOWLED   18
#define MIDLED   19
#define HIGHLED  21
#define MOTOR    27
#define BUZZER_PIN 22

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

unsigned int level = 0;


void beepBuzzer(int frequency, int duration, int repeat) {
  // Function to produce beeping sounds
  for (int i = 0; i < repeat; i++) {
    tone(BUZZER_PIN, frequency, duration);
    delay(300); // Delay between consecutive beeps
  }
  delay(2000); // Delay after the entire sequence
}

void setup() {
  // Pin setup for LEDs, motor, and buzzer
  pinMode(LOWLED, OUTPUT);
  pinMode(MIDLED, OUTPUT);
  pinMode(HIGHLED, OUTPUT);
 
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LOWLED, HIGH);
  digitalWrite(MIDLED, HIGH);
  digitalWrite(HIGHLED, HIGH);
  

  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // WiFi connection
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Blynk initialization
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // Start a new measurement:
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read the result:
  int duration = pulseIn(PIN_ECHO, HIGH);
  Serial.print("Distance in CM: ");
  Serial.println(duration / 50);
  

  level = duration / 50;
  Blynk.virtualWrite(V4, level);

  // Distance-based actions
  if (level < 5) {
    // High bin level, collection required.
    digitalWrite(LOWLED, LOW);
    
    digitalWrite(HIGHLED, HIGH);
    digitalWrite(MIDLED, HIGH);
    beepBuzzer(3000, 100, 3); // Beep three times with a 3000 Hz tone
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V2, LOW);
   
  } else if ((level > 5) && (level < 50)) {
    // Medium level means ready to collect
    digitalWrite(LOWLED, HIGH);
    
    digitalWrite(HIGHLED, HIGH);
    digitalWrite(MIDLED, LOW);
    beepBuzzer(3000, 100, 2); // Beep once with a 3000 Hz tone
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, LOW);
    
  } else if (level >= 50) {
    // Bin is empty, no need to collect
    digitalWrite(HIGHLED, LOW);
    digitalWrite(MIDLED, HIGH);
    digitalWrite(LOWLED, HIGH);
    
    beepBuzzer(3000, 300, 1); // Beep once with a 3000 Hz tone
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V2, HIGH);
    
  }

  delay(1000);
  Blynk.run();
}
