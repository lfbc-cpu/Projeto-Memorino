#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Configurando o display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Definindo pinos
const int leds[] = {2, 3, 4};
const int botoes[] = {5, 6, 7};
const int buzzer = 8;

//Definindo constantes do jogo
const int MAX_FASES = 10;
int sequencia[MAX_FASES];

//Variaveis do jogo
int tamanhoSequencia = 0;
int indiceJogador = 0;
bool jogando = false;

//Protótipos das funções
void iniciarJogo();
void mostrarInicioFase();
void mostrarSequencia();
void verificarJogada(int botaoPressionado);


void setup() {
  Serial.begin(9600);
  // Inicializa o gerador de números aleatórios
  randomSeed(analogRead(A0));

  // Inicializa o display LCD
  lcd.init();
  lcd.backlight();

  // Configura os pinos
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(botoes[i], INPUT_PULLUP);
    digitalWrite(leds[i], LOW);
  }
  // Inicia o jogo com a função
  iniciarJogo();
}

void loop() {
  // Se não estiver jogando, mostra a sequência de inicio e começa o jogo
  if (!jogando) {
    mostrarInicioFase();
    mostrarSequencia();
    jogando = true;
    indiceJogador = 0;
  }
  // Verifica se algum botão foi pressionado
  for (int i = 0; i < 3; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      digitalWrite(leds[i], HIGH);
      verificarJogada(i);
      delay(500);
      digitalWrite(leds[i], LOW);
      delay(100);
    }
  }
}

void iniciarJogo() {
  // Inicializa o jogo na 1 fase
  tamanhoSequencia = 1;
  sequencia[0] = random(0, 3);
  jogando = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Novo jogo!");
  lcd.setCursor(0, 1);
  lcd.print("Fase: 1");
  delay(1500);
}

void mostrarInicioFase() {
  // Mostra a fase atual no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Observe...");
  lcd.setCursor(0, 1);
  lcd.print("Fase: ");
  lcd.print(tamanhoSequencia);
  delay(1200);
}

void mostrarSequencia() {
  // Mostra a sequência de LEDs e sons
  for (int i = 0; i < tamanhoSequencia; i++) {
    digitalWrite(leds[sequencia[i]], HIGH);
    tone(buzzer, 800, 200);
    delay(500);
    digitalWrite(leds[sequencia[i]], LOW);
    delay(300);
  }
}

void verificarJogada(int botaoPressionado) {
  // Verifica se o botão pressionado está correto na sequência
  if (botaoPressionado == sequencia[indiceJogador]) {
    tone(buzzer, 1000, 200);
    indiceJogador++;
    // Se o jogador completou a sequência corretamente
    if (indiceJogador == tamanhoSequencia) {
      // Verifica se o jogador venceu o jogo
      if (tamanhoSequencia == MAX_FASES) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("VOCE VENCEU!");
        tone(buzzer, 1500, 200); delay(200);
        tone(buzzer, 2000, 400); delay(400);
        delay(3000);
        iniciarJogo();
        return;
      }
      // Prepara a próxima fase
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Boa!!");
      lcd.setCursor(0, 1);
      lcd.print("Prox fase: ");
      lcd.print(tamanhoSequencia + 1);
      delay(1500);

      tamanhoSequencia++;
      sequencia[tamanhoSequencia - 1] = random(0, 3);
      jogando = false;
    }
  } else {
    // Jogador errou a sequência
    tone(buzzer, 200, 1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voce perdeu!");
    lcd.setCursor(0, 1);
    lcd.print("Fase: ");
    lcd.print(tamanhoSequencia);
    delay(3000);

    iniciarJogo();
  }
}
