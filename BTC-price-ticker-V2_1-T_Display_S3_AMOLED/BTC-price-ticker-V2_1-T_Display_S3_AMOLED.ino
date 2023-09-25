
#pragma mark - Depend ArduinoJson 6.x and CoinMarketCapApi2 libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/lewisxhe/CoinMarketCapApi2
git clone -b 6.x https://github.com/bblanchon/ArduinoJson.git
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "CoinMarketCapApi.h"

#include "alert.h"
#include "info.h"

#include "btclogo.h"
#include "btc.h"

#define TFT_AQUA  0x04FF
#define TFT_GRAY  0x8410

//---------------------------------------//

// Edit this for your network
#define wifi_ssid "SSID"
#define wifi_password "PASSWORD"

// The new api requires a developer key, so you must apply for a key to use
// https://pro.coinmarketcap.com/account
#define APIKEY "YOUR API KEY"

//---------------------------------------//

WiFiClientSecure client;
CoinMarketCapApi api(client, APIKEY);

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.
unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;

#include "rm67162.h"
#include <TFT_eSPI.h>
#include "NotoSansMonoSCB20.h"
#include "Latin_Hiragana_24.h"

// Initialize display and sprites
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite bgr = TFT_eSprite(&tft);
TFT_eSprite sprite = TFT_eSprite(&tft);

float RSSI = 0.0;

// begin setup
void setup()
{
    Serial.begin(115200);
    Serial.println(F("Boot Ticker"));

    rm67162_init();
    lcd_setRotation(1);
    sprite.setSwapBytes(1);

    sprite.createSprite(536, 240);
    sprite.fillScreen(TFT_BLACK);
    
    /*
    sprite.loadFont(NotoSansMonoSCB20);
    sprite.setTextDatum(TL_DATUM);  // Center the text
    sprite.setTextColor(TFT_GREEN);
    const int centerX = 170;
    const int centerY = 240 / 2;
    sprite.drawString("HELLO WORLD!", centerX, centerY, 2);
    */
    // Push the sprite to the display
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
    
    uint16_t time = millis();
    time = millis() - time;

    sprite.pushImage(0, 0, btclogoWidth, btclogoHeight, btclogo);
    // Push the sprite to the display
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
    delay(3000);

    // Startup
    sprite.fillScreen(TFT_BLACK);
    Serial.print("Connecting to ");
    sprite.drawString("Connecting to ", 15, 10, 2);;
    Serial.println(wifi_ssid);
    sprite.drawString(wifi_ssid, 15, 25, 2);
    sprite.pushImage(200, 2, infoWidth, infoHeight, info);
    // Push the sprite to the display
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
    delay(1000);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    sprite.setTextColor(TFT_GREEN);
    sprite.drawString("WiFi connected", 15, 40, 2);
    sprite.setTextColor(TFT_WHITE);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
    sprite.fillRect(0, 0, 240, 135, TFT_BLACK);

    sprite.pushImage(5, 15, btcWidth, btcHeight, btc);
    // Push the sprite to the display
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
}

void loop()
{
    unsigned long timeNow = millis();
    if ((timeNow > api_due_time))  {
        // int signal bars
        Serial.print("WiFi Signal strength: ");
        Serial.print(WiFi.RSSI());
        sprite.fillRect(200, 2, 40, 32, TFT_BLACK); //wifi RSSI and alert

        int bars;
        RSSI = WiFi.RSSI();

        if (RSSI >= -55) {
            bars = 5;
            Serial.println(" 5 bars");
        } else if (RSSI < -55 & RSSI >= -65) {
            bars = 4;
            Serial.println(" 4 bars");
        } else if (RSSI < -65 & RSSI >= -70) {
            bars = 3;
            Serial.println(" 3 bars");
        } else if (RSSI < -70 & RSSI >= -78) {
            bars = 2;
            Serial.println(" 2 bars");
        } else if (RSSI < -78 & RSSI >= -82) {
            bars = 1;
            Serial.println(" 1 bars");
        } else {
            bars = 0;
            Serial.println(" 0 bars");
        }

        // signal bars
        for (int b = 0; b <= bars; b++) {
            sprite.fillRect(202 + (b * 6), 23 - (b * 4), 5, b * 4, TFT_GRAY);
        }
        printTickerData("BTC");
        api_due_time = timeNow + api_mtbs;
    }
}

void printTickerData(String ticker)
{
    Serial.println("---------------------------------");
    Serial.println("Getting ticker data for " + ticker);

    //For the new API, you can use the currency ID or abbreviated name, such as
    //Bitcoin, you can view the letter after Circulating Supply at https://coinmarketcap.com/, it is BTC

    CMCTickerResponse response = api.update(ticker, "USD");
    if (response.error == "") {
        Serial.print("ID: ");
        Serial.println(response.id);
        Serial.print("Name: ");
        Serial.println(response.name);
        Serial.print("Symbol: ");
        Serial.println(response.symbol);

        Serial.print("Rank: ");
        Serial.println(response.cmc_rank);

        Serial.print("Price: ");
        Serial.println(response.price);

        Serial.print("24h Volume: ");
        Serial.println(response.volume_24h);
        Serial.print("Market Cap: ");
        Serial.println(response.market_cap);

        Serial.print("Circulating Supply: ");
        Serial.println(response.circulating_supply);
        Serial.print("Total Supply: ");
        Serial.println(response.total_supply);

        Serial.print("Percent Change 1h: ");
        Serial.println(response.percent_change_1h);
        Serial.print("Percent Change 24h: ");
        Serial.println(response.percent_change_24h);
        Serial.print("Percent Change 7d: ");
        Serial.println(response.percent_change_7d);
        Serial.print("Last Updated: ");
        Serial.println(response.last_updated);

        sprite.setTextColor(TFT_GRAY);
        sprite.drawString("BTC USD Price", 100, 11, 2);

        sprite.setTextColor(TFT_WHITE);

        sprite.fillRect(117, 37, 123, 38, TFT_BLACK); //price
        sprite.fillRect(185, 80, 55, 20, TFT_BLACK); //rank

        sprite.setTextColor(TFT_YELLOW);

        if (response.percent_change_1h < 0) {
            sprite.setTextColor(TFT_RED);
        }
        
        if (response.percent_change_1h > 0) {
            sprite.setTextColor(TFT_GREEN);
        }

        sprite.drawString(String(response.price).c_str(), 100, 37, 6);
        sprite.setTextColor(TFT_AQUA);
        sprite.drawString("Rank:", 100, 80, 4);

        sprite.drawString(String(response.cmc_rank).c_str(), 183, 80, 4);

        sprite.drawLine(11, 106, TFT_WIDTH-11, 106, TFT_GRAY);

        // hours change
        sprite.fillRect(100, 110, 140, 25, TFT_BLACK);
        sprite.setTextColor(TFT_YELLOW);

        if (response.percent_change_1h < 0) {
            sprite.setTextColor(TFT_RED);
        }
        if (response.percent_change_1h > 0) {
            sprite.setTextColor(TFT_GREEN);
        }
        sprite.drawString("% Price 1h:", 11, 110, 4);
        sprite.drawString(String(response.percent_change_1h).c_str(), 156, 110, 4);
        // Push the sprite to the display
        lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
        delay(90000);

        // 24 hours change
        sprite.fillRect(100, 110, 140, 25, TFT_BLACK);
        sprite.setTextColor(TFT_YELLOW);

        if (response.percent_change_24h < 0) {
            sprite.setTextColor(TFT_RED);
        }
        if (response.percent_change_24h > 0) {
            sprite.setTextColor(TFT_GREEN);
        }
        sprite.drawString("% Price 24h:", 11, 110, 4);
        sprite.drawString(String(response.percent_change_24h).c_str(), 156, 110, 4);
        // Push the sprite to the display
        lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
        delay(90000);

        // 7d hours change
        sprite.fillRect(100, 110, 140, 25, TFT_BLACK);
        sprite.setTextColor(TFT_YELLOW);

        if (response.percent_change_7d < 0) {
            sprite.setTextColor(TFT_RED);
        }
        if (response.percent_change_7d > 0) {
            sprite.setTextColor(TFT_GREEN);
        }
        sprite.drawString("% Price 7d:", 11, 110, 4);
        sprite.drawString(String(response.percent_change_7d).c_str(), 156, 110, 4);
        // Push the sprite to the display
        lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
        delay(90000);
    }
    else {
        Serial.print("Error getting data: ");
        Serial.println(response.error);
        sprite.fillRect(200, 2, 40, 32, TFT_BLACK); //wifi RSSI and alert
        sprite.pushImage(203, 2, alertWidth, alertHeight, alert);
        // Push the sprite to the display
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
    }

    Serial.println("---------------------------------");
}