#include <LiquidCrystal_I2C.h>

#include <Arduino.h>



class TemperaturaControl {
  private:


    //Simbolos de Temperatura
    byte TempEsqSup[3][8] = {
      //Calor
      {B00000, B00000, B00000, B00000, B00111, B01000, B11000, B11111},

      //Muito frio
      {B00000, B00111, B01000, B11000, B10000, B11111, B11010, B10010},

      //Ameno
      {B00000, B00000, B00000, B00000, B00111, B01000, B11010, B10010},
    };
    byte TempEsqInf[3][8] = {
      //Calor
      {B10110, B11000, B01000, B00111, B00000, B00000, B00000, B00000},

      //Muito frio
      {B10000, B11111, B10100, B11111, B10100, B10100, B10100, B11100},

      //Ameno
      {B10000, B11000, B01000, B00111, B00000, B00000, B00000, B00000},
    };


    byte TempDirSup[3][8] = {
      //Calor
      {B00000, B00000, B00000, B00000, B11100, B00010, B00011, B11111},

      //Muito frio
      {B01100, B10010, B00010, B00011, B00001, B11111, B01011, B01001},

      //Ameno
      {B00000, B00000, B00000, B00000, B11100, B00010, B01011, B01001},
    };


    byte TempDirInf[3][8] = {
      //Calor
      {B01101, B00011, B00010, B11100, B00000, B00000, B00000, B00000},

      //Muito frio
      {B00001, B11111, B10101, B11111, B00000, B00000, B00000, B00000},

      //Ameno
      {B00001, B00011, B00010, B11100, B00000, B00000, B00000, B00000},
    };

    String Unidade() {
      switch (alterador) {
        case 1: return "C";
        case 2: return "F";
        case 3: return "K";
      }
      return "C"; // Valor padrão
    }

    int alterador = 0; // Define a unidade de temperatura inicial
    bool buttonPressed = false; // Variável para armazenar o estado do botão
    unsigned long lastUpdateTime = 0; // Última vez que o display foi acionado
    bool showingTemperature = false; // Controla o nome da temperatura que mudou, o tempo que ela aparece

  public:

    void UI(float Temperatura, LiquidCrystal_I2C lcd, int botao) {

      unsigned long currentTime = millis();
      int buttonState = digitalRead(botao);
      int AvaliadorTempo = Temperatura;

      if (buttonState == HIGH && !buttonPressed) { // Se o botão foi pressionado e não estava pressionado antes
        buttonPressed = true; // Atualiza o estado do botão
        alterador++; // Muda a variável alterador
        if (alterador > 3) {
          alterador = 1; // Reinicia se passar do limite
        }
        lcd.clear(); // Limpa o LCD para a nova mensagem
        lastUpdateTime = currentTime;
        showingTemperature = false; // Começa como falso
      } else if (buttonState == LOW) {
        buttonPressed = false; // Reseta o estado do botão quando não está pressionado
      }

      //Formula que transforma a Temperatura de acordo com a última escolha de botão
      if (digitalRead(buttonPressed) == HIGH && alterador == 2) {
        Temperatura = (Temperatura * 9.0 / 5.0) + 32;
      } else if (digitalRead(buttonPressed) == HIGH && alterador == 3) {
        Temperatura += 273.15;
      }

      // Confere se passou 2 segundo
      if (currentTime - lastUpdateTime > 2000) {
        showingTemperature = true; // Troca a tela depois de 2 segundo
        lastUpdateTime = currentTime; // Reseta o temporizador
      }

      lcd.setCursor(1, 0);

      if (showingTemperature == false)
      {
        lcd.setCursor(1, 0);
        switch (alterador) {
          case 1:
            lcd.print("CELSIUS");
            break;
          case 2:
            lcd.print("FAHRENHEIT");
            break;
          case 3:
            lcd.print("KELVIN");
            break;
        }
      }
      else
      {
        // Exibe o valor da temperatura
        lcd.setCursor(0, 0);
        lcd.print("Clima:");
        lcd.print(String(Temperatura) + " " + Unidade());

        int simbTemp;

        lcd.setCursor(0, 1);
        // Maior que 25 será Quente
        if (AvaliadorTempo > 25)
        {
          lcd.print("Calor");
          simbTemp = 0;
        }
        else
        {
          //Entre 15 e 25 será Ameno
          if (AvaliadorTempo > 15)
          {
            lcd.print("Ameno");
            simbTemp = 2;
          }
          else
          {
            //Menor que 15 será Frio
            if (AvaliadorTempo < 15)
            {
              lcd.print("Frio");
              simbTemp = 1;
            }
          }
        }
        EscSimTemp(simbTemp, lcd);
      }
    }

    void EscSimTemp(int simbTemp, LiquidCrystal_I2C lcd)
    {
      lcd.createChar(0, TempEsqSup[simbTemp]);
      lcd.createChar(1, TempEsqInf[simbTemp]);
      lcd.createChar(2, TempDirSup[simbTemp]);
      lcd.createChar(3, TempDirInf[simbTemp]);

      lcd.setCursor(14, 0);

      lcd.write(byte(0));

      lcd.setCursor(14, 1);
      lcd.write(byte(1));

      lcd.setCursor(15, 0);
      lcd.write(byte(2));

      lcd.setCursor(15, 1);
      lcd.write(byte(3));
    }
};
