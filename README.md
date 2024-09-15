# Termo-Light: Sensores para controle de ambiente em laboratórios
![AnimaçãoTermoLightV2_002](https://github.com/user-attachments/assets/83fc4fdd-1994-43ca-bdcb-f5438aff60e5)


# Manual do Usuário
Este manual descreve o funcionamento do Termo-Light, um dispositivo que monitora temperatura, umidade e intensidade de luz e emite alertas sonoros e visuais quando os valores excedem os limites pré-definidos.

É um dispositivo desenvolvido com a missão de melhorar o controle de variáveis em laboratório.

## Componentes

<details open>

| Componente                     | Imagem                                         | Quantidade |
|---------------------------------|------------------------------------------------|------------|
| Arduino UNO                     | <div align="center">![Arduino UNO](./assets/arduino_uno.png)</div>       | 1          |
| Sensor de temperatura e umidade DHT22 | <div align="center">![DHT22](./assets/sensor_de_temperatura_e_umidade_dht22.png)</div> | 1          |
| Sensor de luz LDR               | <div align="center">![LDR](./assets/sensor_de_luz_ldr.png)</div>         | 1          |
| Buzzer                          | <div align="center">![Buzzer](./assets/buzzer.png)</div>                 | 1          |
| LED RGB                         | <div align="center">![LED RGB](./assets/led_rgb.png)</div>               | 1          |
| LCD I2C 16x2                    | <div align="center">![LCD I2C 16x2](./assets/lcd_i2c_16x2.png)</div>     | 1          |
| Botões                          | <div align="center">![Botões](./assets/botoes.png)</div>                 | 3          |
| Relógio de Tempo Real RTC DS3231 | <div align="center">![RTC DS3231](./assets/relogio_de_tempo_real_rtc_ds3231.png)</div> | 1          |
| Placa de prototipagem            | <div align="center">![Placa](./assets/placa_de_prototipagem.png)</div>   | 1          |
| Resistores                      | <div align="center">![Resistores](./assets/resistores.png)</div>         | 6          |
| Cabos de ligação (Jumpers)       | <div align="center">![Jumpers](./assets/cabos_de_ligacao_jumpers.png)</div> | Vários     |

</details>


## Conexões

| **Componente**   | **Conexões** |
|------------------|--------------|
| **DHT22**        | - Pino de Dados (DHT22) -> Pino Digital 8 (Arduino UNO) <br> - VCC (DHT22) -> 5V (Arduino UNO) <br> - GND (DHT22) -> GND (Arduino UNO) |
| **LDR**          | - Pino de saída (LDR) -> A0 (Arduino UNO) |
| **Buzzer**       | - Pino de saída (Buzzer) -> Pino Digital 9 (Arduino UNO) |
| **LED RGB**      | - Vermelho (LED RGB) -> Pino Digital 7 (Arduino UNO) <br> - Verde (LED RGB) -> Pino Digital 6 (Arduino UNO) <br> - Azul (LED RGB) -> Pino Digital 5 (Arduino UNO) |
| **Botões**       | - Botão 1 -> Pino Digital 13 (Arduino UNO) <br> - Botão 2 -> Pino Digital 12 (Arduino UNO) <br> - Botão 3 -> Pino Digital 11 (Arduino UNO) |
| **RTC**          | - SDA (RTC) -> SDA (Arduino UNO) <br> - SCL (RTC) -> SCL (Arduino UNO) <br> - VCC (RTC) -> 5V (Arduino UNO) <br> - GND (RTC) -> GND (Arduino UNO) |
| **LCD**          | - SDA (LCD) -> SDA (Arduino UNO) <br> - SCL (LCD) -> SCL (Arduino UNO) <br> - VCC (LCD) -> 5V (Arduino UNO) <br> - GND (LCD) -> GND (Arduino UNO) |

# Configuração Inicial:
1. Conecte a bateria de 9 volts ao Arduino UNO.
2. Ligue o dispositivo.
3. O LCD mostrará a tela de introdução.
4. A luz verde do LED RGB acenderá, indicando que o dispositivo está pronto.

# Navegação pelas Telas:
- **Botão 1 (Pino 13)**: Troca entre as telas de temperatura, umidade, luz e tempo.
- **Botão 2 (Pino 12)**: Ajusta a unidade de medida da temperatura.
- **Botão 3 (Pino 11)**: Troca o tom do buzzer.

## Telas de Monitoramento:
- **Tela de Temperatura**:
    - Mostra a temperatura atual na unidade de medida selecionada (graus Celsius, Fahrenheit ou Kelvin).
- **Tela de Umidade**:
    - Mostra a umidade relativa atual.
- **Tela de Luz**:
    - Mostra a intensidade de luz em porcentagem (0-100).
- **Tela de Tempo**:
    - Mostra a data e hora atuais.

# Alertas:
- **Buzzer**: Toca um sinal sonoro quando os valores de temperatura, umidade ou luz excedem os limites pré-definidos.
- **LED RGB**: Mudará para vermelho quando os valores de temperatura, umidade ou luz excedem os limites pré-definidos.

## Limites Pré-Definidos:
- **Temperatura**: 15 - 25 graus Celsius
- **Umidade**: 30 - 50%
- **Luz**: 0.1 - 30%


# Aviso:
- Mantenha o dispositivo em um local seguro e seco.
- Não deixe o dispositivo exposto à água ou a altas temperaturas.

# Informações Adicionais:
- O Termo-Light salva os valores de alarme e os últimos eventos de alerta na memória.
- Ao reiniciar o dispositivo, ele carrega os dados salvos da memória.
- Para restaurar as configurações padrão, remova o dispositivo da fonte de alimentação e ligue-o novamente.
- Para utilizar o dispositivo com um computador, conecte-o ao computador via USB.
- Para usar o código original, basta conectar o Arduino UNO ao computador via USB e fazer o upload do código no Arduino IDE.
- Não retirar nenhum componente, todos são essenciais para o funcionamento.

---

# <center>Aproveite seu Termo-Light!</center>
