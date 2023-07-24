# Füllstandsmessung mit einer Pegelsonde - per Wemos D1 Mini (MQTT) 
Dieser Sketch dient zur Füllstandsmessung z.B. einer Zisterne mit einer Pegelsonde.
Er wurde mit der Arduino IDE erstellt.<br>
Folgendes Board wurde dabei gewählt: Werkzeuge >> Bord >> ESP8266 Boards (2.5.0) >> Wemos D1 R1.

Logik:
- es erfolgt eine Messung alle 15 Minuten
- der Messwert wird im RTC-Memory gespeichert und der Wemos danach in den DeepSleep-Modus versetzt
- nach 15 Minuten erfolgt die nächste Messung
- der neue Messwert wird mit dem alten Messwert aus dem RTC-Memory verglichen
- ist der Unterschied zwischen altem und neuem Messwert größer eines Schwellwertes, wird der WiFi und MQTT-Client gestartet und der Messwert übermittelt
- bleibt der Unterschied zwischen den beiden Messwerten unterhalb des Schwellwertes, wird der Wemos direkt wieder in den DeepSleep-Modus versetzt

Die Kommunikation erfolgt über das MQTT-Protokoll.<br>
Da der WiFi und MQTT-Client nur dann gestartet wird, wenn das Delta zwischen dem altem und neuen Messwert größer eines Schwellwertes ist,
ist der Aufbau besonders stromsparend.

Weiterführende Informationen sind auf https://smarthome.kaempf-nk.de/fuellstandsmessung/pegelsonde-messwertuebertragung.html zu finden.
