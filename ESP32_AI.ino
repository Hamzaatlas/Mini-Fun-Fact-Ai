#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configuration ---
const char* ssid = "TOPNET"; 
const char* password = "YOUR_WIFI_PASSWORD"; 
const char* apiKey = "gsk_YOUR_NEW_GROQ_API_KEY"; 

// --- Hardware Pins ---
#define BUTTON_PIN 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 
#define OLED_RESET -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- State Variables ---
int buttonState = LOW;            
int lastButtonState = LOW;        
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

// Function prototypes
String generateAIText();
void displayMessage(String text);
void displayPaginatedMessage(String text);

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Wire.begin(21, 22);
  Wire.setClock(100000); 
  pinMode(BUTTON_PIN, INPUT);
  delay(100);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.display();
  
  displayMessage("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  displayMessage("Ready! Press button.");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == HIGH) {
        displayMessage("Thinking...");
        
        String aiText = generateAIText();
        
        // This will now dynamically generate as many pages as needed!
        displayPaginatedMessage(aiText);
        
        displayMessage("Ready! Press button.");
      }
    }
  }
  
  lastButtonState = reading;
}

String generateAIText() {
  if (WiFi.status() != WL_CONNECTED) {
    return "Error: No WiFi";
  }

  WiFiClientSecure client;
  client.setInsecure(); 

  HTTPClient http;
  http.begin(client, "https://api.groq.com/openai/v1/chat/completions");
  http.addHeader("Content-Type", "application/json");
  
  String authHeader = "Bearer ";
  authHeader += apiKey;
  http.addHeader("Authorization", authHeader);

  // Increased max_tokens to 100 so the AI can finish longer fun facts/stories
  String prompt = "Give me a highly obscure, random fun fact. Request ID: " + String(millis());
  String payload = "{\"model\": \"llama-3.1-8b-instant\", \"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}], \"max_tokens\": 100, \"temperature\": 0.9}";

  int httpResponseCode = http.POST(payload);
  String result = "";

  if (httpResponseCode == 200) {
    String response = http.getString();
    
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (error) {
      result = "JSON Error";
    } else {
      const char* content = doc["choices"][0]["message"]["content"];
      result = String(content);
    }
  } else {
    result = "API Error: " + String(httpResponseCode);
  }
  
  http.end();
  result.trim();
  return result;
}

// Standard function for short status messages
void displayMessage(String text) {
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);     
  display.setTextWrap(true);   
  display.print(text);
  display.display();
}

// Flawless dynamic pagination function
void displayPaginatedMessage(String text) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(true);

  // 1. Measure the exact pixel height of the fully wrapped text
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // 2. Divide total text height by screen height (32). 
  // Adding 31 before dividing is a standard integer math trick to always round UP to the next full page.
  int numPages = (h + 31) / 32;

  // 3. Print out every page, no matter how many there are
  for (int i = 0; i < numPages; i++) {
    display.clearDisplay();
    
    // Shifts the text upwards by exactly one screen height per page
    display.setCursor(0, -(i * 32)); 
    
    display.print(text);
    display.display();
    
    delay(4000); 
  }
}
