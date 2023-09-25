# BTC-price-ticker-V2_1-T_Display_S3_AMOLED

A fork of the [BTC-price-ticker-V2_1-TTGO-T-Display](https://github.com/oxinon/BTC-price-ticker-V2_1-TTGO-T-Display) project, updated to work on the TTGO [T-Display-S3 AMOLED](https://www.lilygo.cc/products/t-display-s3-amoled) board.

TODO: insert pic

Before you begin, review and follow the instructions in <https://github.com/Xinyuan-LilyGO/T-Display-S3-AMOLED> to get the T-Display-S3 AMOLED board ready in the Arduino IDE. <b>The necesssary TFT_eSPI library is a pre-release so you won't find it in the Arduino IDE Library Manager. Please copy the contents of the /lib folder into Arduino/libraries. </b> 

In the Arduino board selector, select "ESP32 Dev Module". Then populate the configuration options as seen in the above project documentation.

Grab a free developer API key from [CoinMarketCap](https://pro.coinmarketcap.com/account) and replace the API key placeholder in the main sketch with it. Do the same for the WiFi SSID and passphrase placeholders.

* * *
