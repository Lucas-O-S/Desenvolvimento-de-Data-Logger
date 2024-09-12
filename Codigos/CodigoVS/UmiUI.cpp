#include <LiquidCrystal_I2C.h>

#include <Arduino.h>



class UmidadeControl {
  private:

    byte UmiEsqSup[5][8] =
    {
      //Muito seco
      {B00000, B00001, B00001, B00011, B00010, B00110, B00100, B01100},

      //Seco
      {B00000, B00001, B00001, B00011, B00010, B00110, B00100, B01100},

      //Ameno
      {B00000, B00001, B00001, B00011, B00010, B00110, B00100, B01100},

      //Umido
      {B00000, B00001, B00001, B00011, B00010, B00110, B00100, B01111},

      //Super Umido
      {B00000, B00001, B00001, B00011, B00011, B00111, B00111, B01111},
    };


    byte UmiEsqInf[5][8] =
    {
      //Muito seco
      {B01000, B10000, B10000, B10000, B10000, B10000, B01000, B00111},

      //Seco
      {B01000, B10000, B10000, B11111, B11111, B11111, B01111, B00111},

      //Ameno
      {B01000, B11111, B11111, B11111, B11111, B11111, B01111, B00111},

      //Umido
      {B01111, B11111, B11111, B11111, B11111, B11111, B01111, B00111},

      //Super Umido
      {B01111, B11111, B11111, B11111, B11111, B11111, B01111, B00111},
    };


    byte UmiDirSup[5][8] =
    {
      //Muito seco
      {B00000, B10000, B10000, B11000, B01000, B01100, B00100, B00110},

      //Seco
      {B00000, B10000, B10000, B11000, B01000, B01100, B00100, B00110},

      //Ameno
      {B00000, B10000, B10000, B11000, B01000, B01100, B00100, B00110},

      //Umido
      {B00000, B10000, B10000, B11000, B01000, B01100, B00100, B11110},

      //Super Umido
      {B00000, B10000, B10000, B11000, B11000, B11100, B11100, B11110},
    };


    byte UmiDirInf[5][8] =
    {
      //Muito seco
      {B00010, B00001, B00001, B00001, B00101, B01001, B00010, B11100},

      //Seco
      {B00010, B00001, B00001, B11111, B11011, B10111, B11110, B11100},

      //Ameno
      {B00010, B11111, B11111, B11111, B11011, B10111, B11110, B11100},

      //Umido
      {B11110, B11111, B11111, B11111, B11011, B10111, B11110, B11100},

      //Super Umido
      {B11110, B11111, B11111, B11111, B11011, B10111, B11110, B11100},
    };

    void EscSimUmi(int simbUmi,  LiquidCrystal_I2C lcd)
    {
      lcd.createChar(0, UmiEsqSup[simbUmi]);
      lcd.createChar(1, UmiEsqInf[simbUmi]);
      lcd.createChar(2, UmiDirSup[simbUmi]);
      lcd.createChar(3, UmiDirInf[simbUmi]);

      lcd.setCursor(14, 0);
      lcd.write(byte(0));

      lcd.setCursor(14, 1);
      lcd.write(byte(1));

      lcd.setCursor(15, 0);
      lcd.write(byte(2));

      lcd.setCursor(15, 1);
      lcd.write(byte(3));

      lcd.setCursor(16, 0);
      lcd.write(byte(4));
    }


  public:
    void UI(float Umidade, LiquidCrystal_I2C lcd) {
      lcd.setCursor(0, 0);
      lcd.print("Umidade:" + String(Umidade) + "%" );
      lcd.setCursor(0, 1);
      int simbUmi;

      if (Umidade > 50)
      {
        lcd.print("Muito umido");
        simbUmi = 4;
      }
      else
      {
        if (Umidade > 40)
        {
          lcd.print("Umido");
          simbUmi = 3;
        }
        else
        {
          if (Umidade >= 30)
          {
            lcd.print("Baixa");
            simbUmi = 2;
          }
          else
          {
            if (Umidade > 10)
            {
              lcd.print("Seco");
              simbUmi = 1;
            }
            else
            {
              lcd.print("Muito seco");
              simbUmi = 0;
            }
          }
        }
      }
      EscSimUmi(simbUmi,lcd);
    }



};