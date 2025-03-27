#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// Pinos
const int ldrPin1 = 34;
const int ldrPin2 = 35;
const int buttonPin = 12;
const int selectorPin = 32;  // Entrada analógica do seletor
const int servoPin = 13;

const int ldrThreshold = 500;

SoftwareSerial mySoftwareSerial(16, 17);
DFRobotDFPlayerMini dfplayer;
Servo bocaServo;

bool audioTocando = false;

// Função para determinar qual faixa com base no valor do seletor
int getFaixaSelecionada() {
  int val = analogRead(selectorPin);
  Serial.print("Valor do seletor: ");
  Serial.println(val);

  // Mapeamento do valor lido para a faixa (ajuste os ranges conforme seu hardware)
  if (val < 500) return 1;
  else if (val < 1500) return 2;
  else if (val < 2500) return 3;
  else return 4;
}

void tocarAudioComBoca(int faixa) {
  if (audioTocando) return;
  audioTocando = true;

  dfplayer.play(faixa);
  Serial.print("Tocando faixa: ");
  Serial.println(faixa);

  unsigned long inicio = millis();
  while (millis() - inicio < 5000) {
    bocaServo.write(60);
    delay(200);
    bocaServo.write(120);
    delay(200);
  }

  bocaServo.write(90);
  audioTocando = false;
}

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  if (!dfplayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer não encontrado!");
    while (true);
  }
  dfplayer.volume(25);

  pinMode(buttonPin, INPUT_PULLDOWN);
  bocaServo.attach(servoPin);
  bocaServo.write(90);
}

void loop() {
  int ldr1 = analogRead(ldrPin1);
  int ldr2 = analogRead(ldrPin2);
  bool botaoPressionado = digitalRead(buttonPin);

  Serial.printf("LDR1: %d | LDR2: %d | Botao: %d\n", ldr1, ldr2, botaoPressionado);

  if ((ldr1 < ldrThreshold && ldr2 < ldrThreshold) || botaoPressionado) {
    int faixa = getFaixaSelecionada();
    tocarAudioComBoca(faixa);
    delay(1000); // debounce
  }

  delay(100);
}
    
