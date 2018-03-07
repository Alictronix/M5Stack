// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void M5Stack::begin() {
    
    // UART 
    Serial.begin(115200);
    Serial.flush();
    Serial.print("M5Stack initializing...");

    // I2C
    pinMode(SCL, OUTPUT);
    digitalWrite(SDA, 1);

    // TONE
    Speaker.begin();

    // Setup the button with an internal pull-up
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_C_PIN, INPUT_PULLUP);

    // TF Card
    SD.begin(TFCARD_CS_PIN);

    // M5 LCD INIT
    Lcd.begin();
    Lcd.fillScreen(BLACK);
    Lcd.setCursor(0, 0);
    Lcd.setTextColor(WHITE);
    Lcd.setTextSize(1);
    Lcd.setBrightness(50);

    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);
    Tft.setCursor(0, 0);
    Tft.setTextColor(WHITE);
    Tft.setTextSize(1);
   // Tft.setBrightness(50);

    // Set wakeup button
    setWakeupButton(BUTTON_A_PIN);

    // MPU9250
#ifdef MPU9250_INSDE
    Wire.begin();
#endif

    Serial.println("OK");
}

void M5Stack::update() {

    //Button update
    BtnA.read();
    BtnB.read();
    BtnC.read();

    //Speaker update
    Speaker.update();
}

void M5Stack::startupLogo() {
    static uint8_t brightness, pre_brightness;
    uint32_t length = strlen((char*)m5stack_startup_music);
   // Lcd.setBrightness(0);
   /* Lcd.drawBitmap(0, 0, (uint16_t *)gImage_logoM5, 128, 160, WHITE);
    Tft.setBrightness(0);
    Tft.drawBitmap(0, 0, (uint16_t *)gImage_logoM5, 128, 160, WHITE);
    for(int i=0; i<length; i++) {
        dacWrite(SPEAKER_PIN, m5stack_startup_music[i]>>2);
        delayMicroseconds(40);
        brightness = (i/157);
        if(pre_brightness != brightness) {
            pre_brightness = brightness;
            TFt.setBrightness(brightness);
        }
    }

    for(int i=255; i>=0; i--) {
        Tft.setBrightness(i);
        if(i<=32) {
            dacWrite(SPEAKER_PIN, i);
        }
        delay(2);
    }

    Tft.fillScreen(BLACK);
    delay(800);
    for(int i=0; i>=100; i++) {
        Tft.setBrightness(i);
        delay(2);
    }*/
}

void M5Stack::setWakeupButton(uint8_t button) {
    _wakeupPin = button;
}

void M5Stack::powerOFF() {
    
    // power off the Lcd
    //Tft.setBrightness(0);
    //Tft.sleep();

    // ESP32 into deep sleep
    USE_SERIAL.printf("Enabling EXT0 wakeup on pins GPIO%d\n", _wakeupPin);
    esp_deep_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin , LOW);
    
    while(digitalRead(_wakeupPin) == LOW) {
        delay(10);
    }
    USE_SERIAL.println("On deep sleep mode.");
    esp_deep_sleep_start();
    USE_SERIAL.println("On power OFF fail!");
}

M5Stack M5;
