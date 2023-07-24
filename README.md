# Level measurement with a levelsensor - via Wemos D1 Mini (MQTT) 
This sketch is used for level measurement, e.g. of a cistern with a level sensor.
It was created with the Arduino IDE.<br>
The following board was selected: Tools >> Board >> ESP8266 Boards (2.5.0) >> Wemos D1 R1.

Logic:
- a measurement is taken every 15 minutes
- the measured value is saved in the RTC memory and thereafter the Wemos will be set to the deep sleep mode
- the next measurement takes place after 15 minutes
- the new measured value is compared with the old value from the RTC memory
- if the difference between the old and new measured value is greater than a threshold, the WiFi and MQTT client is started and the value is transmitted
- if the difference between the two measured values ​​remains below the threshold, the Wemos is immediately switched back to deep sleep mode

Communication takes place via MQTT protocol.<br>
Since the WiFi and MQTT client is only started if the delta between the old and new measured value is greater than a threshold,
the structure is particularly energy-saving.

For more information, see https://smarthome.kaempf-nk.de/fuellstandsmessung/pegelsonde-messwertuebertragung.html (in German).

