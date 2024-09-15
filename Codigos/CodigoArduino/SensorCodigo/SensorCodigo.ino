#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <RTClib.h>
#include "TempUI.cpp"
#include "UmiUI.cpp"
#include "LuzUI.cpp"
#include "TempoUI.h"



//Set up do lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Define o pin do dht
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

//Chamar as classe d cada tela
TemperaturaControl temperaturaControl;
UmidadeControl umidadeControl;
LuzControl luzControl;
Tempo tempo;

//Tela de Introdução 
void Intro() {
  //escreve termo light e pisca varias vezes
  for (int i = 0; i < 3; i++) {
    lcd.backlight();
    lcd.setCursor(4, 0);
    lcd.print("Termo -");
    lcd.setCursor(5, 1);
    lcd.print("Light");
    delay(500);
    lcd.noBacklight();
    lcd.clear();
    delay(250);
  }
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Termo -");
  lcd.setCursor(5, 1);
  lcd.print("Light");
  delay(2000);
  lcd.clear();
}



void setup()
{
  //Inicia do lcd
  lcd.init();
  lcd.backlight();
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

  //Carreha memoria
  Carregar();

  //Inicia luz
  Luz_RGB(0, 255, 0);

  //Roda a intro
  Intro();

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


///Variavel de escolha de tom buzzer
int escolhaTom = 1;

////Tom inicial do buzzer
int tomBuzzer = 440;

///função de mudar buzzer
void MudarBuzzer() {
  ////Leitura do botão do buzzer
  EstadoBotao03 = digitalRead(EntradaBotao03);

  ///Se apertar o botão ativar troca o tom
  if (EstadoBotao03 == HIGH) {
    //delay anti debounce
    delay(50);
    escolhaTom++;

    if (escolhaTom == 5) {
      escolhaTom = 1;

    }
    switch (escolhaTom) {
      case 1:
        tomBuzzer = 440;
        break;

      case 2:
        tomBuzzer = 500;

        break;
      case 3:
        tomBuzzer = 700;

        break;
      case 4:
        tomBuzzer = 250;

        break;
      case 5:
        tomBuzzer = 3020;

        break;
    }
    //Toca buzzer para teste
    TocarBuzzer();
  }
}

///Toca o buzzer
void TocarBuzzer() {
  tone(buzzer, tomBuzzer, 300);
  delay(400);
  tone(buzzer, tomBuzzer, 300);
  delay(400);
  tone(buzzer, tomBuzzer, 300);

}



//Qual tela está atualmente
int telaAtual = 1;

//Função de mudar inidice de tela
void MudarTela() {
  //Pega estado do botão 1
  EstadoBotao01 = digitalRead(EntradaBotao01);

  //Se botão for ativado muda indice
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


//Atravez da letra inicial da variavel, o valor dela e o tempo 
//escreve uma mensagem com as informações recebidas
void MostrarInfo(char variavel, float valor, DateTime tempo) {

  //Escreve tempo
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

  //Verifica qual a variavel
  Serial.print("- Limite de ");
  if (variavel == 'T') {
    Serial.print("Temperatura ");

  }
  else {
    if (variavel == 'L') {
      Serial.print("Luz ");

    }
    else {
      Serial.print("Umidade ");

    }
  }
  Serial.println("foi atingido");

  //Escreve valor
  Serial.print("Valor Lido de ");
  Serial.println(valor);
  Serial.println("------------------------------------------------------");
}


///Ultima execução verifica quando foi executado a função LIMIt Break pela ultima vez
unsigned long ultimaExecucao = 0;

//Intervalo mostra o tempo minimo de execução da função
const int intervalo = 60000;

///Função que avisa em caso de trigger de limite quebrado
void LimitBreak(char variavel, float valor, DateTime tempo) {
  
  //Verifica se passou tempo minimo
  if ( millis() -  ultimaExecucao> intervalo) {
    ultimaExecucao = millis();
    //Luz vermelha
    Luz_RGB(255, 0, 0);
    ///buzzer avisa
    TocarBuzzer();
 

    //Mostra o que aconteceu
    MostrarInfo(variavel, valor, tempo);
    //Salva a memoria
    Salvar(variavel, valor, tempo);
    Serial.println("Salvo na Memoria");
    delay(1000);

    //Liga a luz verde
    Luz_RGB(0, 255, 0);

  }



}

//Posicao Atual da memoria para salvar
int posicaoMemoria = 0;

//Salva uma serie de informações no emprom para ser lidas ao reiniciar
void Salvar(char variavel, float valor, DateTime tempo) {
  EEPROM.write(posicaoMemoria, variavel);
  //Float Ocupa 4 bytes
  EEPROM.put(posicaoMemoria + 1, valor);
  EEPROM.write(posicaoMemoria + 5,  tempo.hour());
  EEPROM.write(posicaoMemoria + 6, tempo.minute());
  EEPROM.write(posicaoMemoria + 7, tempo.second());
  EEPROM.write(posicaoMemoria + 8, tempo.day());
  EEPROM.write(posicaoMemoria + 9, tempo.month());

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
    
    //Le a memoria e salva o valor em variaveis para depois ser escrito no console
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

//Trigger de cada variavel
float triggerTemperaturaMAX = 25;
float triggerTemperaturaMIN = 15;

float triggerUmidadeMAX = 50;
float triggerUminadeMIN = 30;

float triggerLuzMAX = 30;
float triggerLuzMIN = 0.1;



void loop()
{
  //Verifica o tempo agora
  DateTime agora = rtc.now();

  //Espera pelo botão 03 ser ativado
  MudarBuzzer();

  //Busca o estado de cada variavel analisada
  float Umidade = dht.readHumidity();
  float Temperatura = dht.readTemperature();
  ValorLDR = analogRead(pinoLDR);
  //Pega o valor da luz e converte em porcentagem

  IntensidadeLuz = map(ValorLDR, 0, 400, 0, 100); //Converte o valor para uma escala de 0 a 100

  //Verifica LDR
  if (isnan(Umidade) || isnan(Temperatura))
  {
    Serial.println("Falha ao ler do DHT11!");
    return;
  }

  //Espera pelo botão 1 ser ativado
  MudarTela();

  switch (telaAtual) {

    //Tela 1 Luz
    case 1:

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
      luzControl.UI(IntensidadeLuz, lcd);



      break;

    //Tela 2 Umidade
    case 2:


      if (Umidade != umidadeAntiga) {
        Serial.print("Umidade: ");
        Serial.println(Umidade);
      }


      umidadeControl.UI(Umidade, lcd);
      break;

    //Tela 3 temperatura
    case 3:
      temperaturaControl.UI(Temperatura, lcd, EntradaBotao02);

      if (temperaturaAntiga != Temperatura) {
        Serial.print("%  Temperatura: ");
        Serial.print(Temperatura);
        Serial.println("C");
      }

      break;

    //Tela 4 tempo
    case 4:
      tempo.UI(agora, lcd, EntradaBotao02);
      break;
  }


  //Se alguma variavel mudar
  if (  temperaturaAntiga != Temperatura || umidadeAntiga != Umidade || ldrAntigo != ValorLDR) {
    delay(200);
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