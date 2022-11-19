/*
 * Blink
 */

#include <Arduino.h>
#include <driver/rtc_io.h>

#define LED_BUILTIN     GPIO_NUM_4
#define WAKE_BTN_1      GPIO_NUM_0
#define WAKE_BTN_2      GPIO_NUM_35

int i = 0;

void go_deep_sleep()
{
  Serial.println("\nGoing sleep now!");

  // init gpio for deep sleep
  rtc_gpio_init(LED_BUILTIN);
  rtc_gpio_set_direction(LED_BUILTIN, RTC_GPIO_MODE_OUTPUT_ONLY);
  rtc_gpio_set_level(LED_BUILTIN, HIGH);
  gpio_hold_en(LED_BUILTIN);

  // init wake sources
  esp_sleep_enable_ext0_wakeup(WAKE_BTN_1, LOW);
  esp_sleep_enable_ext1_wakeup((uint64_t)1 << WAKE_BTN_2, ESP_EXT1_WAKEUP_ALL_LOW);

  // start deep sleep
  esp_deep_sleep_start();
}

void setup()
{
  // init RTC_GPIOs for normal operation
  rtc_gpio_deinit(WAKE_BTN_1);
  rtc_gpio_deinit(WAKE_BTN_2);
  gpio_hold_dis(LED_BUILTIN);

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Start demo");
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(500);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(500);
  Serial.print(".");

  i++;

  if (i == 10) {
    go_deep_sleep();
  }
}
