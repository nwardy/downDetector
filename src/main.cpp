//pretty simple just HTTP GET, retry a few times, flip a pin
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

static bool githubIsDead = false;

void setRelay(bool on) {
    digitalWrite(RELAY_PIN, on ? LOW : HIGH);
}

void ledBlink(uint32_t onMs, uint32_t offMs, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        digitalWrite(STATUS_LED_PIN, HIGH); delay(onMs);
        digitalWrite(STATUS_LED_PIN, LOW);  delay(offMs);
    }
}

bool checkSite() {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    String url = TARGET_URL;
    url.startsWith("https://") ? http.begin(url, nullptr) : http.begin(url);
    http.setTimeout(HTTP_TIMEOUT_MS);
    http.addHeader("User-Agent", "ESP32-DownDetector/1.0");

    int vibeCheck = http.GET();
    http.end();
    return vibeCheck == HTTP_CODE_OK;
}

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    uint32_t startedBegging = millis();
    while (WiFi.status() != WL_CONNECTED) {
        ledBlink(500, 500, 1);
        if (millis() - startedBegging > 30000UL) ESP.restart();
    }
    ledBlink(100, 100, 3);
    digitalWrite(STATUS_LED_PIN, HIGH);
}

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);      digitalWrite(RELAY_PIN, HIGH);
    pinMode(STATUS_LED_PIN, OUTPUT); digitalWrite(STATUS_LED_PIN, LOW);
    connectWiFi();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();

    bool githubAlive = false;
    for (int attempt = 1; attempt <= RETRY_COUNT; attempt++) {
        if (checkSite()) { githubAlive = true; break; }
        if (attempt < RETRY_COUNT) delay(RETRY_DELAY_MS);
    }

    if (githubAlive && githubIsDead)  Serial.println("back up");
    if (!githubAlive && !githubIsDead) Serial.println("down atomizer on");

    setRelay(!githubAlive);
    digitalWrite(STATUS_LED_PIN, githubAlive ? HIGH : LOW);
    githubIsDead = !githubAlive;

    uint32_t napTime = millis(), blinkToggle = millis();
    bool ledState = true;
    while (millis() - napTime < CHECK_INTERVAL_MS) {
        if (!githubAlive && millis() - blinkToggle >= 200) {
            ledState = !ledState;
            digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
            blinkToggle = millis();
        }
        delay(10);
    }
}
