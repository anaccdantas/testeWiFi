#include <Arduino.h>
#include <WiFi.h>

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRede = "info@134";

void setup() 
{
    Serial.begin(115200);
    WiFi.begin(nomeRedeWiFi, senhaRede);

    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print("#");
        delay(500);
    }

    Serial.printf("\n\r Conectado com sucesso!");
}

void loop() 
{

}
