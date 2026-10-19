#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 20, 4);

String pedirParaApiClima(String codigo);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL"; //porque nao sao string?
const char* senhaRede = "info@134";
const String enderecoApi = "https://wttr.in/sao+caetano+do+sul?m&lang=pt&format=";

void setup() 
{
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();

    WiFi.begin(nomeRedeWiFi, senhaRede);

    Serial.print("\n\rIniciando conexao WiFi\n\r");

    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print("#");
        delay(500);
    }

    Serial.print("\n\rConectado com sucesso!\n\r");
    Serial.printf("Endereco IP: %s\n\r", WiFi.localIP().toString().c_str());
    Serial.printf("Endereco MAC: %s\n\r", WiFi.macAddress().c_str());

}

void loop() //programa do lcd deve estar no loop
{
    String clima;
    String temperatura;
    String sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();
    
    
    lcd.setCursor(0,0);
    lcd.printf("Clima hoje: ");
    lcd.setCursor(0,1);
    lcd.printf("%c\n\r", clima.c_str());

    //lcd.printf("Temperatura Atual %d C e sensacao térmica de %d C", temperatura, sensacaoTermica);
}

String pedirParaApiClima(String codigo)
{
    HTTPClient http;

    http.begin(enderecoApi + codigo);

    int codigoHTTP = http.GET(); // http codes

    String resposta;

    if(codigoHTTP == 200)
    {
        resposta = http.getString(); // pega o texto da pagina
    }
    else
    {
        Serial.printf("Deu erro. Código: %d", codigoHTTP);
    }

    http.end();

    return resposta;
}