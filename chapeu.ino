#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// Pinos
const int ldrPin1 = 34; // LDR1
const int ldrPin2 = 35; // LDR2
const int buttonPin = 12; // Botão de segurança
const int servoPin = 13;

// Threshold para escuridão (ajustável conforme o ambiente)
const int ldrThreshold = 500;

// Objetos
SoftwareSerial mySoftwareSerial(16, 17); // RX, TX
DFRobotDFPlayerMini dfplayer;
Servo bocaServo;

// Flags
bool audioTocando = false;

// Setup
void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // Inicializa o DFPlayer
  if (!dfplayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer não encontrado!");
    while(true);
  }
  dfplayer.volume(25);  // Volume de 0 a 30

  // Pinos
  pinMode(buttonPin, INPUT_PULLDOWN);
  bocaServo.attach(servoPin);
  bocaServo.write(90); // Boca fechada
}

// Função para tocar áudio aleatório e mexer a boca
void tocarAudioComBoca() {
  if (audioTocando) return;

  audioTocando = true;

  int faixa = random(1, 5); // 1 a 4
  dfplayer.play(faixa);
  Serial.print("Tocando faixa: ");
  Serial.println(faixa);

  // Simula movimento da boca por ~5 segundos (ajuste se necessário)
  unsigned long inicio = millis();
  while (millis() - inicio < 5000) {
    bocaServo.write(60);
    delay(200);
    bocaServo.write(120);
    delay(200);
  }

  bocaServo.write(90); // Fecha boca
  audioTocando = false;
}

void loop() {
  int ldr1 = analogRead(ldrPin1);
  int ldr2 = analogRead(ldrPin2);
  bool botaoPressionado = digitalRead(buttonPin);

  Serial.printf("LDR1: %d | LDR2: %d | Botao: %d\n", ldr1, ldr2, botaoPressionado);

  if ((ldr1 < ldrThreshold && ldr2 < ldrThreshold) || botaoPressionado) {
    tocarAudioComBoca();
    delay(1000); // Delay anti-bounce
  }

  delay(100);
}
