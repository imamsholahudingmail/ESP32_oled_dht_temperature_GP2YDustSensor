/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <GP2YDustSensor.h>

const uint8_t SHARP_LED_PIN = 25;   // Sharp Dust/particle sensor Led Pin
const uint8_t SHARP_VO_PIN = 34;    // Sharp Dust/particle analog out pin used for reading 

GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1014AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 15  // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22  // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

int relayPin1 = 13;
int relayPin2 = 12;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(relayPin1, OUTPUT); // Set relay as an output
  pinMode(relayPin2, OUTPUT); // Set relay as an output

  dht.begin();
  dustSensor.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  delay(1000);

  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  if (dustSensor.getDustDensity() < 1000) {
    // code to execute if condition is true
    digitalWrite(relayPin1, HIGH); // Tu
    digitalWrite(relayPin2, HIGH); // Tu
  } else {
    // code to execute if condition is false
    digitalWrite(relayPin1, LOW); // Tu
    digitalWrite(relayPin2, LOW); // Tu
  }

  // clear display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Suhu: ");
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(1);
  display.print("C");

  // display humidity DHT22
  display.setTextSize(1);
  display.setCursor(0, 21);
  display.print("Kelembaban udara: ");
  display.setTextSize(1);
  display.setCursor(0, 31);
  display.print(h);
  display.print(" %");

  // display humidity Soil
  display.setTextSize(1);
  display.setCursor(0, 42);
  display.print("Dust density: ");
  display.setTextSize(1);
  display.setCursor(0, 52);
  display.print(dustSensor.getDustDensity());
  display.print(" ug/m3");

  display.display();
}
