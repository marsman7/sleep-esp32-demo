# ESP32 Deep-Sleep Demo

---

In dieser Demo wird gezeigt, wie der Pegel eines Pins im Deep-Sleep-Modus gesetzt wird und wie der ESP32 z.B. mittels eines Tastendrucks aus dem Deep-Sleep aufgeweckt wird. Meine Platine besitz eine LED an GPIO 4 und zwei Tasten an GPIO 0 und 35. Dieser Demo liegt die Blinky Demo zu grunde. 

Nicht für alle Pins kann der Pegel im Deep-Sleep gesetzt werden und auch zum Aufecken sind nicht alle Pins geeignet. (Siehe Links unten)

## Status der Ausgänge im Deep-Sleep setzen

Im normal Betrieb blinkt die LED und nach 10 mal Blinken geht der ESP32 in den Deep-Sleep-Modus. Die LED soll aber in diesem Modus aber weiterhin leuchten. In der Funktion `go_deep_sleep()` wird nacheinander der GPIO-Pin initialisiert, als Ausgang konfiguriert, Pegel gesetzt und für Deep-Sleep aktiviert.

Nach dem Aufwecken behällt der Pin aber seinen Status. Um ihn wieder also normalen GPIO im Hauptprogramm nutzen zu können wird der Status des GPIO in der `setup()` Funktion mit `gpio_hold_dis()` zurück gesetzt.

## Eingänge für Aufwecken aus dem Deep-Sleep konfigurieren

In dieser Demo zeige ich zwei Möglichkeiten den ESP32 durch ein externes Ereignis aufzuwecken. 

**Achtung! Der GPIO 0 wird auf vielen Platinen auch durch die RTS und DTR Leitungen der serielle Schnitstelle gesteuert, die zum Flashen des ESP32 benutzt wird. Dadurch kann evtl. der ESP32 auch ohne Tastendruck aufgeweckt werden.**

Wird nur ein einzelner GPIO zum Aufwecken benötigt, kann der externe Wakeup 0 genutzt werden. Mit der Funktion `esp_sleep_enable_ext0_wakeup()` wird dem ESP32 die GPIO Nummer und der Pegel, der das Aufwecken auslösen soll, mitgeteilt.

Werden mehrere GPIOs zum Aufwecken benötigt, kommt der externe Wakeup 1 zum Einsatzt. Im Funktionsaufruf von `esp_sleep_enable_ext0_wakeup()` wird dem ESP32 eine Bitmaske und der Modus übergeben. Bei der Bitmaske entspricht jedes Bit einem GPIO. Für der GPIO 35 muss also das 35. Bit gesetzt werden. Mit dem Schiben eines Bits (`1 << GPIO_NUM_x` / bei GPIO 35 also `(uint64_t)1 << GPIO_NUM_35`) ist dies einfach erledigt. Bei den höheren GPIOs muss darauf geachtet werden, dass der Wert 64-Bit lang ist. Mehrere GPIO werden "ODER" verknüpft ( in C und C++ also mit `|` ). Bei dem Modus gibt es zwei Möglichkeiten. Entweder wird das Auswecken dadurch ausgelöst, dass alle GPIOs, in der Bitmaske angegeben sind LOW-Pegel haben (`ESP_EXT1_WAKEUP_ALL_LOW`) oder mindestens einer aus der Bitmaske HIGH-Pegel (`ESP_EXT1_WAKEUP_ANY_HIGH`).

In dieser Demo schalten beide Tasten nach GND, die GPIOs haben also LOW-Pegel solange die entsprechende Taste gedrückt ist. Bei nicht gedrückter Taste wird der GPIO über einen Pullup-Widerstand auf HIGH-Pegel gezogen. Beim externen Wakeup 0 wird also `LOW` als zweiter Parameter angegeben und beim externen Wakeup 1 `ESP_EXT1_WAKEUP_ALL_LOW`.

## Links

[GPIOs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)

[Sleep Modes](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html)

[ESP32 External Wake Up from Deep Sleep](https://randomnerdtutorials.com/esp32-external-wake-up-deep-sleep/)

[ESP32 Deep Sleep Wake Up Sources](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/)