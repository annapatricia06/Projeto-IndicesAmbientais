/*
  Projeto: Índices Ambientais
  Curso: Tecnologia em Saneamento Ambiental
  Autores: Anna Patrícia de Lima e Antônio Vinícius da Silva Sousa
  Orientador: Heraldo Antunes Silva Filho
  Data: 06/03/2023
*/

//Inclusão de Bibliotecas.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

//Definições e Criação de Objetos.
#define pinoDHT22 7       // Definição do Sensor de Temperatura e Umidade DHT22 para a porta digital 7
//#define tempoLeitura 5000 // Definição de um delay de cinco segundos para realização da leitura das variáveis do protótipo (Temperatura e Umidade)
#define tempoBotao 500    // Definição de um delay de meio segundo para controle do processo de pressionamento do botão
#define tempoEspera 300   // Definição de um delay de um segundo para apresentar os valores calculados e obtidos
#define tempoLED 100      // Definição de um delay de cem milisegundo para acionar o LED
#define botao 6           // Definição do botão do protótipo para a porta digital 6
#define LEDverde 9        // Definição do LED verde na entrada digital 10
#define LEDamarelo 10     // Definição do LED amarelo na entrada digital 11
#define LEDlaranja 11     // Definição do LED amarelo na entrada digital 11
#define LEDvermelho 12    // Definição do LED vermelho na entrada digital 12 

float TempF;              // Variável para conversão da leitura de Temperatura de Celsius para Fahrenheit
float TempC;              // Variável para conversão da leitura de Temperatura de Fahrenheit para Celsius
float IC;                 // Variável do Índice de Calor
float IDT;                // Variável do Índice de Desconforto Térmico
float ITU;                // Variável do Índice de Temperatura e Umidade
int contador = 0;         // Variável para controle do pressionamento do botão

dht sensorDHT22;          // Criação do objeto tipo DHT22

// Variáveis para o cálculo do Índice de Calor.
float c1 = -42.379;
float c2 = 2.04901523;
float c3 = 10.14333127;
float c4 = -0.22475541;
float c5 = -0.00683783;
float c6 = -0.05481717;
float c7 = 0.00122874;
float c8 = 0.00085282;
float c9 = -0.00000199;
  
// Instanciando objetos.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);                   // Inicialização do Monitor Serial
  lcd.init();                           // Inicia a comunicação com o Display LCD
  lcd.backlight();                      // Liga a iluminação do Display LCD
  lcd.clear(); 
  pinMode (LEDverde, OUTPUT);           // Definição da Porta Digital 10 como saída do sinal
  pinMode (LEDamarelo, OUTPUT);         // Definição da Porta Digital 11 como saída do sinal
  pinMode (LEDlaranja, OUTPUT);         // Definição da Porta Digital 11 como saída do sinal
  pinMode (LEDvermelho, OUTPUT);        // Definição da Porta Digital 12 como saída do sinal
  pinMode(botao, INPUT_PULLUP);         // Definição do botão como entrada do sinal
  sensorDHT22.read22(pinoDHT22);        // Read22 é o método de leitura, e o pino será pinoDHT22 definido no #define
}

void loop() {
  // Impressão no Display LCD.
  lcd.setCursor(0, 0);                          // Posiciona o cursor na primeira coluna da linha 1
  lcd.print(" TESTE INDICE AMB.  ");
  lcd.setCursor(0, 1);                          // Posiciona o cursor na primeira coluna da linha 2
  lcd.print("Pressione o botao");
  lcd.setCursor(0, 2);                          // Posiciona o cursor na primeira coluna da linha 3
  lcd.print("para proseguir!");
  lcd.setCursor(0, 3);                          // Posiciona o cursor na primeira coluna da linha 4
  lcd.print("       Aguardando...");

  Serial.print("Umidade(%)= ");
  Serial.println(sensorDHT22.humidity);
  Serial.print("Temperatura(ºC)= ");
  Serial.println(sensorDHT22.temperature);


// Acionamento do LED.
  digitalWrite(LEDverde, HIGH);
  //delay(10000);
  
  if(!digitalRead(botao)){
    contador++;                         // Armazenamento da quantidade de vezes que o botão foi pressionado
    delay(tempoBotao);                  // Criação de um intervalo de tempo de meio segundo
    lcd.clear();                        // Limpa o display LCD
    digitalWrite(LEDverde, LOW);        // Desligamento do LED
    
    while(contador == 1){

      sensorDHT22.read22(pinoDHT22);    // Read22 é o método de leitura, e o pino será pinoDHT22 definido no #define
      
      // Impressão no LCD.
      lcd.setCursor(0, 0);              // Posiciona o cursor na primeira coluna da linha 1
      lcd.print("-     Previsao    -");
      lcd.setCursor(0, 1);              // Posiciona o cursor na primeira coluna da linha 2
      lcd.print("Umidade: ");
      lcd.print(sensorDHT22.humidity);
      lcd.print("%");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      lcd.setCursor(0, 2);              // Posiciona o cursor na primeira coluna da linha 3
      lcd.print("Temp C: ");
      lcd.print(sensorDHT22.temperature);
      lcd.print("C");
      //delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      
      // Conversão para Fahrenheit.
      TempF = (sensorDHT22.temperature * 1.8) + 32; 
      
      // Impressão no LCD.
      lcd.setCursor(0, 3);              // Posiciona o cursor na primeira coluna da linha 4
      lcd.print("Temp F: ");
      lcd.print(TempF);
      lcd.print("F");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo

      if(!digitalRead(botao)){
        contador++;                     // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);              // Criação de um intervalo de tempo de meio segundo
        lcd.clear();                    // Limpa o display LCD

        // Desligamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
      }
    }
    
    while(contador == 2){

      sensorDHT22.read22(pinoDHT22);        // Read22 é o método de leitura, e o pino será pinoDHT22 definido no #define  

      // Se o índice de calor for MENOR OU IGUAL a 80 °F, utiliza-se apenas a equação do IC.
      IC = 0.5 * (TempF + 61 + ((TempF - 68) * 1.2 ) + (sensorDHT22.humidity * 0.094));
    
      // Se o índice de calor for MAIOR que 80 °F, utiliza-se a equação da análise de regressão múltipla (ICRM) baseada nos estudos de Steadman.
      if (IC >= 80){

      IC = c1 + 
           c2 * TempF + 
           c3 * sensorDHT22.humidity + 
           c4 * TempF * sensorDHT22.humidity + 
           c5 * (pow(TempF, 2)) + 
           c6 * (pow(sensorDHT22.humidity, 2)) + 
           c7 * (pow(TempF, 2)) * sensorDHT22.humidity + 
           c8 * TempF * (pow(sensorDHT22.humidity, 2)) + 
           c9 * (pow(TempF, 2)) * (pow(sensorDHT22.humidity, 2));
      
        if(sensorDHT22.humidity < 13 && (TempF >= 80.0 && TempF <= 112.0))
        {
          float ajuste = ((13.0 - sensorDHT22.humidity) / 4.0) * 
                          sqrt((17.0 - abs(TempF - 95.0)) / 17.0); 
          IC = IC - ajuste;
        }
        
        else if(sensorDHT22.humidity > 85.0 && (TempF >= 80.0 && TempF <= 87.0))
        {
          float ajuste = ((sensorDHT22.humidity - 85.0) / 10) * 
                          ((87.0 - TempF) / 5);
          IC = IC + ajuste;
        }
      }
      
      // Índice de Calor.  
      // Conversão para Celsius.
      TempC = ((IC - 32) * 5/9);
      
      // Impressão no LCD.
      lcd.setCursor(0, 0);              // Posiciona o cursor na primeira coluna da linha 1
      lcd.print("- Indice de Calor  -");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      lcd.setCursor(0, 1);              // Posiciona o cursor na primeira coluna da linha 2 
      lcd.print("Valor: ");
      lcd.print(TempC);
      lcd.print("C");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      
      // Influência do Índice de Calor (Intervalo de Faixas de 26 a 54°C).
    
      // Menor que 26°C.
      if (TempC < 26)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, HIGH);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
          
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Normal");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 26 a 32°C.
      if (TempC >= 26 && TempC < 32)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, HIGH);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);

        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Atencao");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 32 a 41°C.
      if(TempC >= 32 && TempC < 41)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, HIGH);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Cuidado extremo");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 41 a 54°C.
      if(TempC >= 41 && TempC <= 54)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, HIGH);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Perigo");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }
      
      // Maior que 54°C.
      if(TempC > 54)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, HIGH);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Perigo extremo");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }

      if(!digitalRead(botao)){
        contador++;                     // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);              // Criação de um intervalo de tempo de meio segundo
        lcd.clear();                    // Limpa o display LCD
        
        // Desligamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
      }
    }
    
    while(contador == 3){

      sensorDHT22.read22(pinoDHT22);        // Read22 é o método de leitura, e o pino será pinoDHT22 definido no #define

      // Índice de Desconforto Térmico.
      IDT = TempC - (0.55 - (0.0055 * sensorDHT22.humidity) * (TempC - 14.5));
    
      // Impressão no LCD.
      lcd.setCursor(0, 0);
      lcd.print("-    Indice IDT    -");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      lcd.setCursor(0, 1); 
      lcd.print("Valor: ");
      lcd.print(IDT);
      lcd.print("C");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
    
      // Influência do Índice de Desconforto Térmico (Intervalo de Faixas de 14,9 a 26,5°C).
      
      // Menor que 14.9°C.
      if (IDT <= 14.9)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, HIGH);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Frio desconfortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 15 a 19.9°C.
      if (IDT >= 15 && IDT <= 19.9)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, HIGH);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Confortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 20 a 26.4°C.
      if(IDT >= 20 && IDT <= 26.4)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, HIGH);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Conforto parcial");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Maior que 26.5°C.
      if(IDT >= 26.5)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, HIGH);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Calor desconfortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }
      
      if(!digitalRead(botao)){
        contador++;                     // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);              // Criação de um intervalo de tempo de meio segundo
        lcd.clear();                    // Limpa o display LCD
        
        // Desligamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
      }
              
    }
    
    while(contador == 4){

      sensorDHT22.read22(pinoDHT22);        // Read22 é o método de leitura, e o pino será pinoDHT22 definido no #define

      // Índice de Temperatura e Umidade.
      ITU = (0.8 * TempC) + ((sensorDHT22.humidity * TempC)/500);
    
      // Impressão no LCD.
      lcd.setCursor(0, 0);
      lcd.print("-    Indice ITU    -");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
      lcd.setCursor(0, 1); 
      lcd.print("Valor: ");
      lcd.print(ITU);
      lcd.print("C");
      delay(tempoEspera);               // Criação de um intervalo de tempo de um segundo
    
      // Influência do Índice de Temperatura e Umidade (Intervalo de Faixas de 14,9 a 26,5°C).
    
      // Menor que 14.9°C.
      if (ITU <= 14.9)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, HIGH);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Frio desconfortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 15 a 19.9°C.
      if (ITU >= 15 && ITU <= 19.9)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, HIGH);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Confortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Entre 20 a 26.4°C.
      if(ITU >= 20 && ITU <= 26.4)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, HIGH);
        digitalWrite(LEDvermelho, LOW);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Conforto parcial");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }  
      
      // Maior que 26.5°C.
      if(ITU >= 26.5)
      {
        // Acionamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, HIGH);
        
        // Impressão no LCD.
        lcd.setCursor(0, 2);            // Posiciona o cursor na primeira coluna da linha 3
        lcd.print("-     Situacao     -");
        lcd.setCursor(0, 3);            // Posiciona o cursor na primeira coluna da linha 4
        lcd.print("Calor desconfortavel");
        delay(tempoEspera);             // Criação de um intervalo de tempo de um segundo
      }
      
      if(!digitalRead(botao)){
        contador++;                     // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);              // Criação de um intervalo de tempo de meio segundo
        lcd.clear();                    // Limpa o display LCD
        // Desligamento do LEDs.
        digitalWrite(LEDverde, LOW);
        digitalWrite(LEDamarelo, LOW);
        digitalWrite(LEDlaranja, LOW);
        digitalWrite(LEDvermelho, LOW);
      }      
    }
    
    while(contador == 5){
      lcd.setCursor(0, 0);              // Posiciona o cursor na primeira coluna da linha 1
      lcd.print("Pressione para");
      lcd.setCursor(0, 1);              // Posiciona o cursor na primeira coluna da linha 1
      lcd.print("retorna!");
      lcd.setCursor(0, 3);              // Posiciona o cursor na primeira coluna da linha 4
      lcd.print("       Aguardando...");
      
      // Acionamento do LED.
      digitalWrite(LEDverde, HIGH);
      delay(tempoLED);
      digitalWrite(LEDamarelo, HIGH);
      delay(tempoLED);
      digitalWrite(LEDlaranja, HIGH);
      delay(tempoLED);
      digitalWrite(LEDvermelho, HIGH);
      delay(tempoLED);
      digitalWrite(LEDverde, LOW);
      delay(tempoLED);
      digitalWrite(LEDamarelo, LOW);
      delay(tempoLED);
      digitalWrite(LEDlaranja, LOW);
      delay(tempoLED);
      digitalWrite(LEDvermelho, LOW);
      delay(tempoLED);
      
      if(!digitalRead(botao)){
        contador++;                     // Armazenamento da quantidade de vezes que o botão foi pressionado
        delay(tempoBotao);              // Criação de um intervalo de tempo de meio segundo
        lcd.clear();                    // Limpa o display LCD
        contador = 0;                   // Zera a contagem das vezes que o botão foi pressionado        
      }
    }
    
    while(!digitalRead(botao)){         // Comando para controlar o pressionamento do botão
      lcd.clear();                    // Limpa o display LCD
      // Desligamento do LEDs.
      digitalWrite(LEDverde, LOW);
      digitalWrite(LEDamarelo, LOW);
      digitalWrite(LEDlaranja, LOW);
      digitalWrite(LEDvermelho, LOW);
    }
  }
}
