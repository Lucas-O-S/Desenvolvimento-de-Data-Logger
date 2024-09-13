#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Arduino.h>

class Tempo
{
  public:

    int alterador = 0; // Define a unidade de temperatura inicial
    bool buttonPressed = false; // Variável para armazenar o estado do botão
    unsigned long lastUpdateTime = 0; // Última vez que o display foi acionado
    bool informacaotempo = false; // Controla o nome da temperatura que mudou, o tempo que ela aparece

    void UI(DateTime agora, LiquidCrystal_I2C lcd, int botao) {

      unsigned long currentTime = millis();
      int buttonState = digitalRead(botao);

      if (buttonState == HIGH && !buttonPressed) { // Se o botão foi pressionado e não estava pressionado antes
        buttonPressed = true; // Atualiza o estado do botão
        alterador++; // Muda a variável alterador
        if (alterador > 2) {
          alterador = 1; // Reinicia se passar do limite
        }
        lcd.clear(); // Limpa o LCD para a nova mensagem
        lastUpdateTime = currentTime;
        informacaotempo = false; // Começa como falso
      } else if (buttonState == LOW) {
        buttonPressed = false; // Reseta o estado do botão quando não está pressionado
      }

      // Confere se passou 2 segundo
      if (currentTime - lastUpdateTime > 2000) {
        informacaotempo = true; // Troca a tela depois de 2 segundo
        lastUpdateTime = currentTime; // Reseta o temporizador
      }

      //Altera entre 24horas e AM/PM de acordo com o botão sendo apertado
      if (informacaotempo == false)
      {
        lcd.setCursor(1, 0);
        switch (alterador)
        {
          case 1:
            lcd.print("24 horas");
            break;

          case 2:
            lcd.print("AM/PM");
            break;
        }
      }
      else
      {
        String AMPM;//No texto final coloca o AM ou PM
        lcd.setCursor(1,0);//Limpa o resto das letras que não foram sobrepostas
        lcd.print("  ");

        // Se 'informacaotempo' for verdadeiro, exibe data e hora
        lcd.setCursor(3, 0);
        lcd.print(agora.day(), DEC);
        lcd.print("/");
        lcd.print(agora.month(), DEC);
        lcd.print("/");
        lcd.print(agora.year(), DEC);

        lcd.setCursor(4, 1);
        if (alterador == 1) {  // Formato 24 horas
          lcd.print(agora.hour(), DEC);
        } else if (alterador == 2) {  // Formato AM/PM
          int hour = agora.hour() % 12;
          if (hour == 0) hour = 12;  // Converte para 12 horas
          lcd.print(hour, DEC);
          if (agora.hour() >= 12) {
            AMPM = "PM";
          } else {
             AMPM = "AM";
          }
        }
        lcd.print(":");
        lcd.print(agora.minute(), DEC);
        lcd.print(":");
        lcd.print(agora.second(), DEC);
        lcd.print(AMPM);
      }
    }
};
