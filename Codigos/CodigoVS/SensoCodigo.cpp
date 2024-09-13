#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <RTClib.h>
#include "TempUI.cpp"
#include "UmiUI.cpp"
#include "LuzUI.cpp"
#include "TempoUI.cpp"



LiquidCrystal_I2C lcd(0x27, 16, 2);

#define pinoDHT 8

//Altere para DHT11 no real
//Inicia o DHT
DHT dht(pinoDHT, DHT22);

//Define as entradas de cada cor do led
#define Red 7
#define Green 6
#define Blue 5

//Define entrada do buzzer
#define buzzer 9

//Define as variaveis do Sensor LDR e sua entrada

int pinoLDR = A0;

//Entradas dos botões
int EntradaBotao01 = 13;
int EntradaBotao02 = 12;
int EntradaBotao03 = 11;



//Inicia variavel rtc
RTC_DS3231 rtc;

//Chamar classes uteis
TemperaturaControl temperaturaControl;
UmidadeControl umidadeControl;
LuzControl luzControl;
Tempo tempo;


//Bytes dos Simbolos de Luz
/**Cada byte é um vetor que cabe 4 conjuntos de 8 bits
   Nesse caso o  primeiro é a lua;
   a esquerda é a linha mais a acima e o da direita é o mais abaixo baixo**/



/////////////////////////////////////////////////////////////////
//Simbolos de Temperatura

/////////////////////////////////////////////////////////////////
//Simbolos de Umidade

/////////////////////////////////////////////////////////////////////////////////



void setup()
{
  //Inicia do lcd
  lcd.init();
  lcd.backlight();

  //Inicia o console
  Serial.begin(9600);
  pinMode(pinoLDR, INPUT);

  //Inicia RGB
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);


  //Inicia DHT
  dht.begin();

  //Inicia Botões
  pinMode(EntradaBotao01, INPUT);
  pinMode(EntradaBotao02, INPUT);

  //Inicia CLock
  rtc.begin();
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    // Ajustar o RTC para a data e hora da compilação
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Carregar();
  Luz_RGB(0, 255, 0);


}


//Função para mudar cor do LED usando sistema RGB
void Luz_RGB(int valorR, int valorG, int valorB)
{
  analogWrite(Red, 255 - valorR);
  analogWrite(Green, 255 - valorG);
  analogWrite(Blue, 255 - valorB);
}

///////////////////////////////////////////////////////////////////////
//Funções relacionadas a botão


//Se o botão foi precionado
int EstadoBotao01 = LOW;
int EstadoBotao02 = LOW;
int EstadoBotao03 = LOW;

void MudarBuzzer() {

}



//Qual tela está atualmente
int telaAtual = 1;
void MudarTela() {
  EstadoBotao01 = digitalRead(EntradaBotao01);

  if (EstadoBotao01 == HIGH) {
    //delay anti debounce
    delay(50);
    telaAtual++;

    if (telaAtual > 4) {
      telaAtual = 1;

    }
    lcd.clear();
  }
}





//SetCursor define onde deseja começar nesse caso é no bloco 1 cima e depois o de baixo
//lcd.print escreve uma mensagem diretamente

//Tela de luz
void Ui(int IntensidadeLuz)
{

}

/**Escolher simbolo é uma função que escolhe o simbolo com base do vetor dos bytes
   0 - Muito escuro
   1 - escuro
   2 - claro
   3 - Muito claro
*/

//Escolhe os simbolos de luz


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Temperatura

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Umidade


/////////////////////////////////////////////////////////////

void TempoUI(DateTime agora) {
  lcd.setCursor(3, 0);

  lcd.print(agora.day(), DEC);
  lcd.print("/");
  lcd.print(agora.month(), DEC);
  lcd.print("/");
  lcd.print(agora.year(), DEC);

  lcd.setCursor(4, 1);

  lcd.print(agora.hour(), DEC);
  lcd.print(":");
  lcd.print(agora.minute(), DEC);
  lcd.print(":");
  lcd.print(agora.second(), DEC);



}

/////////////////////////////////////////

void MostrarInfo(char variavel, float valor, DateTime tempo) {

  Serial.print(tempo.hour(), DEC);
  Serial.print(":");
  Serial.print(tempo.minute(), DEC);
  Serial.print(":");
  Serial.print(tempo.second(), DEC);
  Serial.print("-");
  Serial.print(tempo.day(), DEC);
  Serial.print("/");
  Serial.print(tempo.month(), DEC);
  Serial.print("/");
  Serial.println(tempo.year(), DEC);


  Serial.print("- Limite de ");
  if (variavel == 'T') {
    Serial.print("Temperatura ");

  }
  else {
    if (variavel == 'L') {
      Serial.print("Luz ");

    }
    else {
      Serial.print("Humidade ");

    }
  }
  Serial.println("foi atingido");

  Serial.print("Valor Lido de ");
  Serial.println(valor);
  Serial.println("------------------------------------------------------");
}

void LimitBreak(char variavel, float valor, DateTime tempo) {

  tone(buzzer, 440, 10);
  Luz_RGB(255, 0, 0);

  MostrarInfo(variavel, valor, tempo);

  Salvar(variavel, valor, tempo);
  Serial.println("Salvo na Memoria");
  delay(1000);
  Luz_RGB(0, 0, 255);



}

//Posicao Atual da memoria para salvar
int posicaoMemoria = 0;

//Salva uma serie de informações no emprom para ser lidas ao reiniciar
void Salvar(char variavel, float valor, DateTime tempo) {
  EEPROM.write(posicaoMemoria, variavel);
  //Float Ocupa 4 bytes
  EEPROM.put(posicaoMemoria + 1, valor);
  EEPROM.write(posicaoMemoria + 2,  tempo.hour());
  EEPROM.write(posicaoMemoria + 3, tempo.minute());
  EEPROM.write(posicaoMemoria + 4, tempo.second());
  EEPROM.write(posicaoMemoria + 5, tempo.day());
  EEPROM.write(posicaoMemoria + 6, tempo.month());

  // Armazenar o ano (2 bytes)
  int year = tempo.year();
  EEPROM.write(posicaoMemoria + 10, year & 0xFF);
  EEPROM.write(posicaoMemoria + 11, (year >> 8) & 0xFF);

  //Usamos 12 bytes para salvar os dados
  posicaoMemoria += 12;

  if (posicaoMemoria > 85) {
    posicaoMemoria = 0;
  }


}

///Carrega os dados salvos do EEPROM ao reiniciar
void Carregar() {

  Serial.println("Dados Salvos Anteriormente na memoria: ");
  for (int i = 0; i <= 85; i += 12) {

    char variavel = EEPROM.read(i);
    float valor;
    EEPROM.get(i + 1, valor);

    byte hour = EEPROM.read(i + 5);
    byte minute = EEPROM.read(i + 6);
    byte second = EEPROM.read(i + 7);
    byte day = EEPROM.read(i + 8);
    byte month = EEPROM.read(i + 9);
    int year = EEPROM.read(i + 10) | (EEPROM.read(i + 11) << 8);

    // Criar um objeto DateTime com os valores lidos
    DateTime tempo(year, month, day, hour, minute, second);
    MostrarInfo(variavel, valor, tempo);

  }
}





//Variaveis de Luminosidade
int ValorLDR;
int IntensidadeLuz;

//valores antigos para não repetir o clear
float temperaturaAntiga = 0;
float umidadeAntiga = 0;
int ldrAntigo = 0;

float triggerTemperaturaMAX = 25;
float triggerTemperaturaMIN = 15;

float triggerUmidadeMAX = 50;
float triggerUminadeMIN = 30;

float triggerLuzMAX = 30;
float triggerLuzMIN = 0.1;



void loop()
{
  DateTime agora = rtc.now();



  //Busca o estado de cada variavel analisada
  float Umidade = dht.readHumidity();
  float Temperatura = dht.readTemperature();
  ValorLDR = analogRead(pinoLDR);
  IntensidadeLuz = map(ValorLDR, 0, 400, 0, 100); //Converte o valor para uma escala de 0 a 100

  //Verifica LDR
  if (isnan(Umidade) || isnan(Temperatura))
  {
    Serial.println("Falha ao ler do DHT11!");
    return;
  }

  MudarTela();

  switch (telaAtual) {

    case 1:
      //Luz
      //Pega o valor da luz e converte em porcentagem

      if (IntensidadeLuz > 100)
      {
        IntensidadeLuz = 100;
      }

      if (ldrAntigo != ValorLDR) {
        Serial.print("Intensidade de Luz 0 - 1023 = ");
        Serial.println(ValorLDR);
        Serial.print("Intensidade de Luz 0 - 100% = ");
        Serial.println(IntensidadeLuz);
      }

      //Passa para o LCD
      luzControl.UI(IntensidadeLuz,lcd);



      break;

    case 2:


      if (Umidade != umidadeAntiga) {
        Serial.print("Umidade: ");
        Serial.println(Umidade);
      }


      umidadeControl.UI(Umidade,lcd);
      break;

    case 3:

      temperaturaControl.UI(Temperatura, lcd, EntradaBotao02);
      
      if (temperaturaAntiga != Temperatura) {
        Serial.print("%  Temperatura: ");
        Serial.print(Temperatura);
        Serial.println("C");
      }
      break;

    case 4:
        tempo.UI(agora, lcd, EntradaBotao02);
      break;
  }


  //Se alguma variavel mudar
  if (  temperaturaAntiga != Temperatura || umidadeAntiga != Umidade || ldrAntigo != ValorLDR) {
    lcd.clear();

    ///Triggers de limites ultrapassados

    if ( Temperatura > triggerTemperaturaMAX || Temperatura < triggerTemperaturaMIN) {
      LimitBreak('T', Temperatura, agora);
    }

    if ( IntensidadeLuz > triggerLuzMAX || IntensidadeLuz < triggerLuzMIN) {
      LimitBreak('L', IntensidadeLuz, agora);


    }

    if ( Umidade > triggerUmidadeMAX || Umidade < triggerUminadeMIN) {
      LimitBreak('H', Umidade, agora);

    }
  }
  //Atualiza variaveis antigas, para evitar repetição no console
  temperaturaAntiga = Temperatura;
  umidadeAntiga = Umidade;
  ldrAntigo = ValorLDR;
  //////////////////////////////////////////////////////////////////////////////////////////////////////




}
