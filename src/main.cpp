#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

String pedirParaApiClima(String codigo);

const char *nomeRedeWiFi = "SENAI EDUCACIONAL"; // porque nao sao string?
const char *senhaRede = "info@134";
const String enderecoApi = "https://wttr.in/sao+caetano+do+sul?m&lang=pt&format=";

//* ----------------------------------SERVIDORES NTP----------------------------------
const char *NTP1 = "a.st1.ntp.br";
const char *NTP2 = "pool.ntp.org";
const char *FUSO = "<-03>3";
const char* DIAS[] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
//? colocamos primeiro o nosso fuso horario e, em seguida, quantas horas precisamos somar para
//? chegar no horario 00:00 (no nosso caso, +3).

void setup()
{
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();

    WiFi.begin(nomeRedeWiFi, senhaRede);

    Serial.print("\n\rIniciando conexao WiFi\n\r");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("#");
        delay(500);
    }

    Serial.print("\n\rConectado com sucesso!\n\r");
    Serial.printf("Endereco IP: %s\n\r", WiFi.localIP().toString().c_str());
    Serial.printf("Endereco MAC: %s\n\r", WiFi.macAddress().c_str());

    //* Configuracao de fuso e servidores NTP e inicia a sincronizacao
    //* Se o programa nao conseguir se conectar ao servidor 1 (NTP1), ele
    //* utilizará o servidor 2 (NTP2).
    configTzTime(FUSO, NTP1, NTP2); 

    struct tm agora;

    Serial.print("\n\rSincronizando hora\n\r");

    while (!getLocalTime(&agora)) //? o parametro é o endereco da estrutura tipo tm
    {
        Serial.print(".");
        delay(500);
    }

    Serial.print("\n\rHora sincronizada!");
}

void loop() // programa do lcd deve estar no loop
{
    String clima;
    String temperatura;
    String sensacaoTermica;

    struct tm agora;
    
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();
    
    if(getLocalTime(&agora))
    {
        lcd.setCursor(0,0);
        lcd.printf("%s-feira, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
        //? wday = weekday
    }

    lcd.setCursor(0, 1);
    lcd.printf("Clima: %s", clima.c_str());
    lcd.setCursor(0, 2);
    lcd.printf("Temperatura de %d C", temperatura.toInt());
    lcd.setCursor(0, 3);
    lcd.printf("Sensacao de %d C", sensacaoTermica.toInt());
}

String pedirParaApiClima(String codigo)
{
    HTTPClient http;

    http.begin(enderecoApi + codigo);

    int codigoHTTP = http.GET(); // http codes

    String resposta;

    if (codigoHTTP == 200)
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