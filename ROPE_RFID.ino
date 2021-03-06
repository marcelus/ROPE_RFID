//-------------Som WAV---------------------
#include <SD.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328 

TMRpcm tmrpcm;   // create an object for use in this sketch

// ---------------Som Beeps----------------
int const som = 11;

// notas para o som
#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

void somParada() {
  beep(som, a, 200);
  beep(som, c, 200);
  beep(som, d, 200);
  beep(som, c, 200);
}

void somAndar() {
    beep(som, c, 500); 
    beep(som, g, 500);     
    beep(som, a, 500); 
    beep(som, b, 350); 
    beep(som, cH, 150);
}

void somFinal() {
    beep(som, g, 500);     
    beep(som, f, 500); 
    beep(som, e, 350); 
}

void somBotao() {
    beep(som, d, 100); 
    beep(som, c, 100); 
}

void somInicio() {
//    ligarLed();  
    beep(som, b, 50);
    beep(som, f, 50); 
    beep(som, f, 50);
    beep(som, e, 50);
    beep(som, d, 50); 
    beep(som, c, 50);
}

void beep (unsigned char som, int frequencyInHertz, long timeInMilliseconds) {
  while(tmrpcm.isPlaying()){
    
  }
//  ligarLed();  
    int x;   
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    for (x=0;x<loopTime;x++) {   
        digitalWrite(som,HIGH);
        delayMicroseconds(delayAmount);
        digitalWrite(som,LOW);
        delayMicroseconds(delayAmount);
    }    
//    desligarLed();
    delay(20);
    //Um dalay para separar as notas
}

//-------------RFID---------------------

//Bibliotecas para utilização do sensor
#include <SPI.h>
#include <MFRC522.h>

//Pinos de Reset e SlaveSelect
#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

byte posicaoAtual[4];                   // Armazenamento do cartão lido pelo RC522
byte posicaoAnterior[4];                    // Armazenamento do cartão anterior
int qntLeiturasRFID = 20;           // Quantidade de tentativas de leitura que o sensor tenta realizar

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Declaração do valor dos cartões RFID do tabuleiro
byte celula[6][4][4] = 
{
  {
    {byte(39), byte(72), byte(217), byte(148)},
    {byte(119), byte(44), byte(160), byte(36)},
    {byte(119), byte(127), byte(43), byte(37)},
    {byte(87), byte(124), byte(39), byte(37)},
  },{
    {byte(199), byte(245), byte(36), byte(37)},
    {byte(23), byte(112), byte(163), byte(36)},
    {byte(135), byte(57), byte(163), byte(36)},
    {byte(167), byte(169), byte(159), byte(36)},
  },{
    {byte(39), byte(99), byte(159), byte(36)},
    {byte(231), byte(59), byte(37), byte(37)},
    {byte(103), byte(191), byte(162), byte(36)},
    {byte(7), byte(94), byte(31), byte(37)},
  },{
    {byte(247), byte(255), byte(162), byte(36)},
    {byte(87), byte(155), byte(217), byte(148)},
    {byte(7), byte(93), byte(149), byte(37)},
    {byte(87), byte(48), byte(241), byte(217)},
  },{
    {byte(199), byte(211), byte(178), byte(37)},
    {byte(247), byte(232), byte(178), byte(37)},
    {byte(135), byte(48), byte(179), byte(37)},
    {byte(215), byte(57), byte(248), byte(217)},
  },{
    {byte(119), byte(162), byte(247), byte(217)},
    {byte(23), byte(176), byte(149), byte(37)},
    {byte(119), byte(106), byte(255), byte(217)},
    {byte(215), byte(251), byte(247), byte(217)},
  }
};

// Vetor para verificação de posição
int contCelula[6][4];

// Vetor para verificação da posição anterior
int contCelulaAnterior[6][4];

// Função para zerar as variáveis utilizadas para identificação de posição
void zerarRFID() {
  for (int i = 0; i < 4; i++) {
    posicaoAtual[i] = 0;
  }
  
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      contCelula[i][j] = 0;
      contCelulaAnterior[i][j] = 0;
    }
  }
}

//Função de leitura de tag RFID
void lerRFID(){

  //Desativa o som para evitar conflitos com o SPI
  tmrpcm.disable();
  
  //Armazena cartão antigo, se houver um
  if (posicaoAtual[0]) {
    Serial.println("");
    Serial.print("Cartao antigo: ");
    for (int i = 0; i < 4; i++) {
      posicaoAnterior[i] = posicaoAtual[i];
      Serial.print(posicaoAnterior[i], HEX);
    }
    Serial.println("");
  }

  //Zera as variáveis para efetuar nova leitura e verificar posição
  zerarRFID();

  //Faz a leitura e armazenamento de novo RFID
  for (int i = 0 ; i < qntLeiturasRFID ; i ++) {
    if (mfrc522.PICC_IsNewCardPresent()) {
      if (mfrc522.PICC_ReadCardSerial()) {
        for (int i = 0; i < 4; i++) {
          posicaoAtual[i] = mfrc522.uid.uidByte[i];
        }
      }      
    }
  }

  //Imprime RFID lido
  Serial.println("");
  Serial.print("UID: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(posicaoAtual[i], HEX);
  }
  Serial.println("");
}

// Verifica a posição referente a leitura do RFID
void descobrirPosicao(){
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        if(posicaoAtual[k] == celula[i][j][k])
          contCelula[i][j]++;
      }
    }
  }

  for (int i = 0; i < 6; i++){
    for (int j = 0; j < 4; j++){
      if (contCelula[i][j] == 4){
        Serial.println();
        Serial.print("Estou na coluna ");
        Serial.print(i + 1);
        Serial.print(", linha ");
        Serial.print(j + 1);
        Serial.println(".");
      }
    }
  }
  Serial.println("-----------------------------------");
}

boolean compararCelula(byte celula1[4], byte celula2[4]){
  int contIgualdade = 0;
  for (int i = 0; i < 4; i++){
    if (celula1[i] == celula2 [i])
      contIgualdade++;
  }
  if (contIgualdade == 4){
    return true;
  } else {
    return false;
  }
}

//------------INTERAÇÕES----------------

boolean tomate = false;

void baterArvore() {
  tmrpcm.play("arvore.wav");
  tremer();
  limparInstrucoes();
}

void cairBuraco(){
  tmrpcm.play("buraco.wav");
  tremer();
  limparInstrucoes();
}

void cairAgua(){
  tmrpcm.play("agua.wav");
  tremer();
  limparInstrucoes();
}

void atravessarPonteFraca(){
  if(random(2)==1){
    tmrpcm.play("ponte.wav");
    Serial.println("NÃO CAI!");
    tremer();
  } else {
    tmrpcm.play("pontea.wav");
    Serial.println("CAI!");
    tremer();
    limparInstrucoes();
  }
}

void pegarTomate(byte celulaAtual[4]){
  if (!tomate){
    if (!furarPneu(celulaAtual)) {
      tmrpcm.play("tomate.wav");
      tomate = true;
    }
  } else {
    furarPneu(celulaAtual);
  }
}

void interagirFazendeiro(){
  if (!compararCelula(posicaoAtual,posicaoAnterior)){
    if(tomate){
      tmrpcm.play("farmer2.wav");
      tomate = false;
    } else {
      tmrpcm.play("farmer1.wav");
    }
  }
}

boolean furarPneu(byte celulaAtual[4]) {
  if (compararCelula(celulaAtual,celula[4][1])) {
    if (compararCelula(posicaoAnterior,celula[5][1])) {
      limparInstrucoes();
      tmrpcm.play("pneu.wav");
      return true;
    }
  } else if (compararCelula(celulaAtual,celula[5][1])) {
    if (compararCelula(posicaoAnterior,celula[4][1])) {
      limparInstrucoes();
      tmrpcm.play("pneu.wav");
      return true;
    }
  }
  return false;
}

// Identifica a posição atual e executa ação correspondente
void executarInteracao() {
  
  if (contCelula[0][0] == 4) {
    interagirFazendeiro();
    return;
  }
  
  if (contCelula[1][0] == 4) {
    cairAgua();
    return;
  }
  
  if (contCelula[1][2] == 4) {
    cairAgua();
    return;
  }
  
  if (contCelula[1][3] == 4) {
    atravessarPonteFraca();
    return;
  }
  
  if (contCelula[2][0] == 4) {
    baterArvore();
    return;
  }
  
  if (contCelula[2][2] == 4) {
    baterArvore();
    return;
  }
  
  if (contCelula[2][3] == 4) {
    pegarTomate(celula[2][3]);
    return;
  }
  
  if (contCelula[3][0] == 4) {
    cairBuraco();
    return;
  }
  
  if (contCelula[3][2] == 4){
    baterArvore();
    return;
  }
  
  if (contCelula[4][0] == 4) {
    baterArvore();
    return;
  }

  if (contCelula[4][1] == 4) {
    furarPneu(celula[4][1]);
    return;
  }
  
  if (contCelula[5][1] == 4) {
    pegarTomate(celula[5][1]);
    return;
  }
  
  if (contCelula[5][3] == 4) {
    baterArvore();
    return;
  }
  
}

//-------------Botoes-------------------
int btesquerda = A1;
int btfrente = A2;
int btgo = A4;
int bttras = A5;
int btdireita = A3;


boolean botaoFrente(){
  if(analogRead(btfrente) >= 500)
    return true;
  else
    return false;
}

boolean botaoTras(){
  if(analogRead(bttras) >= 500)
    return true;
  else
    return false;
}

boolean botaoDireita(){
  if(analogRead(btdireita) >= 500)
    return true;
  else
    return false;
}

boolean botaoEsquerda(){
  if(analogRead(btesquerda) >= 500)
    return true;
  else
    return false;
}

boolean botaoGo(){
  if(analogRead(btgo) >= 500)
    return true;
  else
    return false;
}

//-------------Leds--------------
//int led1 = 2;
//int led2 = 3;
//
//void ligarLed(){
//  digitalWrite(led1,HIGH);
//  digitalWrite(led2,LOW);
//}
//
//void desligarLed(){
//  digitalWrite(led1,LOW);
//  digitalWrite(led2,HIGH);
//}

//-------------Motor----------
#include <AccelStepper.h>
#define HALFSTEP 5

#define motorPin1  9  // IN1 on the ULN2003 driver 1
#define motorPin2  8  // IN2 on the ULN2003 driver 1
#define motorPin3  7  // IN3 on the ULN2003 driver 1
#define motorPin4  6  // IN4 on the ULN2003 driver 1

#define motorPin5  14 // IN1 on the ULN2003 driver 2
#define motorPin6  15 // IN2 on the ULN2003 driver 2
#define motorPin7  16 // IN3 on the ULN2003 driver 2
#define motorPin8  17 // IN4 on the ULN2003 driver 2

AccelStepper stepper1(HALFSTEP, motorPin1, motorPin2, motorPin3, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin6, motorPin7, motorPin8);

int const velocidadeMaxima = 800; 
int const aceleracao = 6000; 
int const passosGiro = 1530; // numero de giros para o lado
int const passosParaFrente = 2700; // numero de passo para frente

boolean moverFrente() {
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(-passosParaFrente);
  stepper2.moveTo(passosParaFrente);

  while (stepper1.currentPosition() != -passosParaFrente) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  return false; 
}

boolean moverTras() {
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(passosParaFrente);
  stepper2.moveTo(-passosParaFrente);

  while (stepper1.currentPosition() != passosParaFrente) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  return false; 
}

boolean moverEsquerda() {
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(passosGiro);
  stepper2.moveTo(passosGiro);

  while (stepper1.currentPosition() != passosGiro) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  return false; 
}

boolean moverDireita() {
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(-passosGiro);
  stepper2.moveTo(-passosGiro);

  while (stepper1.currentPosition() != -passosGiro) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  return false; 
}

boolean tremer() {
  
//Move para direita
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(passosGiro/8);
  stepper2.moveTo(passosGiro/8);

  while (stepper1.currentPosition() != passosGiro/8) {
    stepper1.run();
    stepper2.run();
  }

//delay(100);

//Move para esquerda
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(-passosGiro/4);
  stepper2.moveTo(-passosGiro/4);

  while (stepper1.currentPosition() != -passosGiro/4) {
    stepper1.run();
    stepper2.run();
  }

//delay(100);

//Volta para o eixo
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  stepper1.moveTo(passosGiro/8);
  stepper2.moveTo(passosGiro/8);

  while (stepper1.currentPosition() != passosGiro/8) {
    stepper1.run();
    stepper2.run();
  }
  
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  return false; 
}

//-------------//Geral//-----------------
int frente = 10;
int tras = 20;
int direita = 30;
int esquerda = 40;
int memoria[50];
int posiMemoria = 0;
int contadorMemoria = 0;

long randNumber;

void setup() {
  Serial.begin(9600);
  pinMode(btfrente,INPUT);
  pinMode(bttras,INPUT);
  pinMode(btdireita,INPUT);
  pinMode(btesquerda,INPUT);
  pinMode(btgo,INPUT);
//  pinMode(led1,OUTPUT);
//  pinMode(led2,OUTPUT);
  pinMode(som,OUTPUT);

  //digitalWrite(led2,HIGH);

  stepper1.setMaxSpeed(velocidadeMaxima);
  stepper1.setAcceleration(aceleracao);
  stepper2.setMaxSpeed(velocidadeMaxima);
  stepper2.setAcceleration(aceleracao);

  SPI.begin(); //Init SPI bus
  mfrc522.PCD_Init(); //Init MFRC522 card
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

  Serial.println("IT'S ALIVE!");

  tmrpcm.speakerPin = 11; //11 on Mega, 9 on Uno, Nano, etc
  
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("Problema no SD!");
    return;   // don't do anything more if not
  }
  
  tmrpcm.setVolume(5);

  //tmrpcm.play("arvore.wav");

  randomSeed(analogRead(0));
}

void loop() {
  programar();
  executar();
  limparInstrucoes();
}


void programar() {
  while(!botaoGo()) {
    if(botaoFrente()) {
      Serial.println("Botao Frente!");
      somBotao();
      memoria[posiMemoria] = frente;
      posiMemoria++;
//      ligarLed();
      while(botaoFrente()){
        delay(10);
      }
//      desligarLed();
    }
     
    else if(botaoTras()) {
      Serial.println("Botao Tras!");
      somBotao();
      memoria[posiMemoria]=tras;
      posiMemoria++;
//      ligarLed();
      while(botaoTras()) {
        delay(10);
      }
//      desligarLed();
    }
    
    else  if(botaoDireita()) {
      Serial.println("Botao Direita!");      
      somBotao();
      memoria[posiMemoria]=direita;
      posiMemoria++;
//      ligarLed();
      while(botaoDireita()) {
        delay(10);
      }
//      desligarLed();
    }
    
    else if(botaoEsquerda()) {
      Serial.println("Botao Esquerda!");      
      somBotao();
      memoria[posiMemoria]=esquerda;
      posiMemoria++;
//      ligarLed();
      while(botaoEsquerda()){
        delay(10);
      }
//      desligarLed();
    }
  }
  Serial.println("Botão Go!");
}

void printMemoria(){
  Serial.println();
  Serial.print("Vetor de Memoria: (");
  for(int i = 0; i < 50; i++){
    Serial.print(memoria[i]);
    Serial.print(", ");
  }
  Serial.println(")");
}

void executar(){
  lerRFID();
  descobrirPosicao();
  executarInteracao();

  while(contadorMemoria<posiMemoria) 
  {
    Serial.println("");
    Serial.print("Execucao: ");
    Serial.println(contadorMemoria);

    if(memoria[contadorMemoria]==frente) {
//      ligarLed();
      somAndar();
      moverFrente();
//      desligarLed();
      contadorMemoria++;
    }
    
    else if(memoria[contadorMemoria]==tras) {
//      ligarLed();
      somAndar();
      moverTras();
//      desligarLed();
      contadorMemoria++;
    }
    
    else if(memoria[contadorMemoria]==direita) {
//      ligarLed();
      somAndar();
      moverDireita();
//      desligarLed();
      contadorMemoria++;
    }
    
    else if(memoria[contadorMemoria]==esquerda) {
//      ligarLed();
      somAndar();
      moverEsquerda();
//      desligarLed();
      contadorMemoria++;
    }

    lerRFID(); 
    descobrirPosicao();
    executarInteracao();
  }
}

void limparInstrucoes(){
  int i = 0;
  for(i = 0; i < 50; i++) {
    memoria[i] = 0;
  }
  posiMemoria = 0;
  contadorMemoria = 0;
}
