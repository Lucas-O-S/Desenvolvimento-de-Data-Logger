# Desenvolvimento de Data Logger para Monitoramento Ambiental

Objetivos: Criar um dispositivo de registro de dados (data logger) dedicado ao monitoramento de condições ambientais em espaços controlados. O equipamento deve realizar o monitoramento ambiental de temperatura, umidade relativa do ar e luminosidade.



Especificações:


Microcontrolador: Utilizar o ATMEGA 328P.
Armazenamento: Memória EEPROM para armazenar os dados coletados.
Relógio de Tempo Real: Incluir um módulo RTC para marcar as entradas de dados com timestamps precisos.
Interface de Usuário: Implementar um display LCD I2C para exibição de dados e status.
Controles: Incluir teclas de input para navegação e configuração do dispositivo.
Indicadores Visuais: Utilizar LEDs para indicar o status operacional e alertas.
Alertas Sonoros: Incorporar um buzzer para alertas audíveis.
Sensores: DHT 11 e LDR.


Níveis dos triggers (gatilhos):

Faixa de Temperatura: 15 < t < 25 ºC

Faixa de Luminosidade: 0 < l < 30%

Faixa de Umidade: 30% < u < 50%



Lista de Materiais:

1 MCU (Atmega 328P) - Arduino Uno R3

1 LDR + Resistor 10KOhm

1 DHT-11 (Sensor de temperatura e umidade)

1 LCD 16x2 - I2C

1 Bateria de 9V + suporte para bateria

1 RTC (Real Time Clock)

Protoboard, jumpers, LEDs e Resistores



Observações:

O projeto deve ser apresentado na forma de um hands-on no dia 16/9 (5 minutos / equipe).
Atenção as unidades de medida e precisão dos sensores (estas informações devem estar descritas na documentação).
Entregar o link do GitHub (público) com o arquivo readme.md contendo a documentação com as especificações técnicas do produto, diagrama elétrico e manual de operação, além do código fonte devidamente comentado.
Será necessário gerar evidências através de um vídeo demonstrativo com 3 minutos (time-lapse) apresentando as funcionalidades do produto e seus diferenciais na forma de um Pitch.
O vídeo deve estar no YouTube (link público).
