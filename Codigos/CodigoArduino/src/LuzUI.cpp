#include <LiquidCrystal_I2C.h>

#include <Arduino.h>



class LuzControl {
  private:


    byte SimboloCima01[4][8] = {
      //Lua
      {B00000, B00000, B00000, B00011, B01111, B01110, B11100, B11100},

      // Nublado
      {B00000, B00000, B00000, B00000, B00000, B00000, B00111, B01000},

      // Sol entre nuvens
      {B00000, B00000, B00100, B00010, B00000, B00001, B00111, B01000},
      // Sol
      {B00000, B00000, B00000, B00000, B01000, B00100, B00001, B11010},
    };

    byte SimboloBaixo01[4][8] = {
      //LUA
      { B11100, B11110, B01111, B01111, B00111, B00000, B00000, B00000},
      // Nublado
      {B01000, B10000, B01111, B00000, B00000, B00000, B00000, B00000},
      // Sol entre nuvens
      {B01000, B10000, B01111, B00000, B00000, B00000, B00000, B00000},
      // Sol
      {B00010, B00001, B00100, B01001, B00001, B00000, B00000, B00000},

    };


    byte SimboloCima02[4][8] = {
      //LUA
      {B00000, B00000, B00000, B10000, B00000, B00000, B00000, B00000},
      // Nublado
      {B00000, B00000, B00000, B00000, B00000, B00000, B01100, B11010},
      // Sol entre nuvens
      {B00000, B01000, B01001, B00010, B11000, B00101, B01100, B11010},
      // Sol
      {B00000, B00000, B00000, B10000, B10001, B00010, B10000, B01000},



    };

    byte SimboloBaixo02[4][8] = {
      //LUA
      { B00010, B00110, B11110, B11100, B11000, B00000, B00000, B00000},
      // Nublado
      {B00001, B00011, B11110, B00000, B00000, B00000, B00000, B00000},
      // Sol entre nuvens
      {B00001, B00011, B11110, B00000, B00000, B00000, B00000, B00000},
      // Sol
      {B01011, B10000, B00100, B00010, B00000, B00000, B00000, B00000},


    };

    void EscolherSimbolo(int simbolo,LiquidCrystal_I2C lcd)
    {
      /*Define onde estará guardado o desenho

        Como define:
        0 - Bloco esquerdo cima
        1 - Bloco esquerdo baixo
        2 - Bloco Direito Cima
        3 - Bloco Direito Baixo

        "simbolo" é o símbolo selecionado conforme explicado acima
      */

      lcd.createChar(0, SimboloCima01[simbolo]);
      lcd.createChar(1, SimboloBaixo01[simbolo]);
      lcd.createChar(2, SimboloCima02[simbolo]);
      lcd.createChar(3, SimboloBaixo02[simbolo]);

      lcd.setCursor(14, 0);

      //Neste caso o LCD write ira escever no bloco 14 de Cima o conteudo guardado no byte 0(Parte Cima esquerdo do desenho)
      lcd.write(byte(0));

      lcd.setCursor(14, 1);
      lcd.write(byte(1));

      lcd.setCursor(15, 0);
      lcd.write(byte(2));


      lcd.setCursor(15, 1);
      lcd.write(byte(3));
    }

  public:
    void UI(float IntensidadeLuz, LiquidCrystal_I2C lcd) {
      lcd.setCursor(1, 0);
      lcd.print("Luz: " + String(IntensidadeLuz) + "%" );
      lcd.setCursor(1, 1);
      int simbolo;

      // Entre 100% e 10% será muito claro
      if (IntensidadeLuz > 35)
      {
        lcd.print("Muito Claro");

        simbolo = 3;
      }
      else
      {
        //Entre 10% e 1% será claro
        if (IntensidadeLuz > 2)
        {
          lcd.print("Claro");

          simbolo = 2;
        }
        else
        {
          //Entre 1% e 0.1% será escuro
          if (IntensidadeLuz > 0.5)
          {
            simbolo = 1;
            lcd.print("Escuro");

          }

          // Entre 0.1% e 0% será Muito Escuro
          else
          {
            lcd.print("Muito Escuro");
            simbolo = 0;
          }
        }
      }
      EscolherSimbolo(simbolo,lcd);
    }


};