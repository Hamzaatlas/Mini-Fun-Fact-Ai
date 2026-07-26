# ESP32-AI-Fun-Fact-Button 🤖🪐

An ESP32-powered hardware companion that fetches and dynamically paginates random AI fun facts, quotes, and stories from the Groq API onto an OLED display at the press of a button.

## What this project does
1. Connects your ESP32 microcontroller to Wi-Fi and securely queries the Groq API (using the Llama 3 model) via HTTPS.
2. Handles physical button presses with built-in software debouncing for reliable triggering.
3. Dynamically calculates the exact pixel height of the incoming AI text using text-bound metrics.
4. Seamlessly paginates long stories and paragraphs across a 128x32 I2C OLED display so nothing gets cut off.

## Setup
1. Open the code in the Arduino IDE.
2. Install the required libraries via the Arduino Library Manager:
   - `Adafruit_GFX`
   - `Adafruit_SSD1306`
   - `ArduinoJson`
3. Update your Wi-Fi credentials (`ssid`, `password`) and your free Groq API key (`apiKey`) in the code.

## Run
1. Select your ESP32 board and correct COM port in the Arduino IDE.
2. Compile and upload the sketch to your ESP32.
3. Press the physical button connected to pin 4 to generate and read your AI stories!

## Roadmap
- [x] Integrate I2C OLED display (128x32)
- [x] Implement hardware button debouncing logic
- [x] Connect to Groq Cloud API for free, fast LLM text generation
- [x] Build automated multi-page text pagination
- [ ] Add deep-sleep power-saving mode for battery operation

## Hardware & Data Sources
- [ESP32 Development Board](https://www.espressif.com/)
- [0.91 inch I2C OLED Display (128x32)](https://www.adafruit.com/)
- [Groq Cloud Console](https://console.groq.com/)
