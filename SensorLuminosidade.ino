#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int ValorLDR; 
int IntensidadeLuz; 
int pinoLDR = A1; 

//Bytes dos Simbolos 
/**Cada byte é um vetor que cabe 4 conjuntos de 8 bits
 * Nesse caso o  primeiro é a lua; 
 * a esquerda é a linha mais a acima e o da direita é o mais abaixo baixo**/
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

////////////////////////////


void setup()
{
  //Inicia do lcd
  lcd.begin(16,2);              
  lcd.clear(); 
  
  //Inicia o console
  Serial.begin(9600); 
  pinMode(pinoLDR, INPUT); 

  
}


//SetCursor define onde deseja começar nesse caso é no bloco 1 cima e depois o de baixo
//lcd.print escreve uma mensagem diretamente

void Ui(int IntensidadeLuz){
  lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Luz: " + String(IntensidadeLuz) + "%" );
    lcd.setCursor(1,1);
  int simbolo;
  
  // Entre 100% e 10% será muito claro
  if(IntensidadeLuz > 35){
    lcd.print("Muito Claro");

    simbolo = 3;
  }
  else{
    //Entre 10% e 1% será claro 
    if(IntensidadeLuz > 2){
      lcd.print("Claro");

      simbolo = 2;
    }
    else{
      //Entre 1% e 0.1% será escuro
      if(IntensidadeLuz > 0.5){
       simbolo = 1;
       lcd.print("Escuro");

      }
      
      // Entre 0.1% e 0% será Muito Escuro
      else{
        lcd.print("Muito Escuro");
        simbolo = 0;
      }
    }
  }

    EscolherSimbolo(simbolo);

}

    /**Escolher simbolo é uma função que escolhe o simbolo com base do vetor dos bytes
     * 0 - Muito escuro
     * 1 - escuro
     * 2 - claro
     * 3 - Muito claro
    */

void EscolherSimbolo(int simbolo){

    /*Define onde estará guardado o desenho

    Como defini:
    0 - Bloco esquerdo cima
    1 - Bloco esquerdo baixo
    3 - Bloco Direito Cima
    4 - Bloco Direito Baixo
    
    Simbolo é o simbolo selecionado conforme explicado acima
    */

  lcd.createChar(0,SimboloCima01[simbolo]);
  lcd.createChar(1,SimboloBaixo01[simbolo]);
  lcd.createChar(2,SimboloCima02[simbolo]);
  lcd.createChar(3,SimboloBaixo02[simbolo]);
    

  lcd.setCursor(14,0);

  //Neste caso o LCD write ira escever no bloco 14 de Cima o conteudo guardado no byte 0(Parte Cima esquerdo do desenho)
  lcd.write(byte(0));

  lcd.setCursor(14,1);
   lcd.write(byte(1));

  lcd.setCursor(15,0);
  lcd.write(byte(2));


  lcd.setCursor(15,1);
  lcd.write(byte(3));
}


void SerialDisplay(int ValorLDR, int IntensidadeLuz){
  Serial.print("Intensidade de Luz 0 - 1023 = "); 
  Serial.println(ValorLDR); 
  Serial.print("Intensidade de Luz 0 - 100% = ");
  Serial.println(IntensidadeLuz);
  
}

void loop()
{
  //Pega o valor da luz e converte em porcentagem
  ValorLDR = analogRead(pinoLDR); 
  IntensidadeLuz = map(ValorLDR, 0, 400, 0, 100); //Converte o valor para uma escala de 0 a 100
  
  //Escreve no console serial os valores
  SerialDisplay(ValorLDR,IntensidadeLuz);
  
  
  //Passa para o LCD
  Ui(IntensidadeLuz);


  
  delay(1000);
 
}
