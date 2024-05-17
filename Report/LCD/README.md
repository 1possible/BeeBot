# Communication entre un Arduino et un tft lcd shield

Ce projet démontre comment établir une communication I2C entre un Arduino et un ESP32. L'Arduino agira en tant que maître et l'ESP32 en tant qu'esclave. L'Arduino scanne d'abord les périphériques I2C, envoie un message à l'ESP32, et ensuite l'ESP32 répondra chaque seconde après avoir reçu le message initial.

## Matériel nécessaire

- 2x Arduino
- 1x tft lcd shield
- Câbles de connexion

## Connexions matérielles

![Schéma de connexion (src:https://docs.arduino.cc/learn/communication/wire/ )](image/CâblageLCD.png)
