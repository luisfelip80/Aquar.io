segue_linha_17-11.c
DETALHES
ATIVIDADE
Carregando...


     #include "Ultrasonic.h"
    #define echoPin 10 //Pino 10 recebe o pulso do echo
    #define trigPin 9 //Pino 9 envia o pulso para gerar o echo
   int limiteD =8,t=300;
   
    //iniciando a função e passando os pinos
    Ultrasonic ultrasonic(9,10);

    int parou = 0;
    int Motor_D1 = 8; // pino do motor direito, que pelo código original era esquerdo.
    int Motor_D2 = 7; // pino do motor direito, que pelo código original era esquerdo.
    int Motor_DV = 6; // pino do motor direito, que pelo código original era esquerdo.
    int Motor_EV = 5; // pino do motor esquerdo, que pelo código original era direito.
    int Motor_E2 = 4; // pino do motor esquerdo, que pelo código original era direito.
    int Motor_E1 = 3; // pino do motor esquerdo, que pelo código original era direito.
    int inPin = 2; //pino de entrada no sensor
    int valor = 0; //Inteiro que armazena o valor lido
    int distancia;

    void paraTudo(){
      
        digitalWrite(Motor_E1, HIGH);
        digitalWrite(Motor_E2, HIGH);
        digitalWrite(Motor_D1, HIGH);
        digitalWrite(Motor_D2, HIGH);
    }

    void medida(){
      
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    distancia = (ultrasonic.Ranging(CM));
    Serial.println(distancia);
    }

    void setup(){
       Serial.begin(9600);
       pinMode(echoPin, INPUT); // define o pino 10 como entrada (recebe)
       pinMode(trigPin, OUTPUT); // define o pino 9 como saida (envia)
       pinMode(inPin, INPUT); //  colocando a porta 2 como entrada
       pinMode(Motor_D1, OUTPUT);
       pinMode(Motor_D2, OUTPUT);
       pinMode(Motor_DV, OUTPUT);
       pinMode(Motor_EV, OUTPUT);
       pinMode(Motor_E2, OUTPUT);
       pinMode(Motor_E1, OUTPUT);
    }
    void desvia(){
      paraTudo();
      delay(100);
      if(distancia<limiteD){
      	limiteD=30;
      }
      while(distancia<limiteD){
           medida();

          //liga motor E voltando.
          analogWrite(Motor_DV, 80);
          digitalWrite(Motor_D1, LOW);
          digitalWrite(Motor_D2, HIGH);
          delay(40);
          paraD(10);
          analogWrite(Motor_DV, 50);
          digitalWrite(Motor_D1, LOW);
          digitalWrite(Motor_D2, HIGH);
        
        }
        
        //delay(1000);
        //segue retor;
        paraD(100);
        boostE(90,50);
        boostD(80,40);
        ligaE(80);
        ligaD(60);
        delay(700);

        paraTudo();
		delay(100);

		//curva
        boostD(80,40);
        ligaD(50);
        delay(900);
        paraD(100);
        
        // segue reto 
        boostE(90,50);
        boostD(80,40);
        ligaE(80);
        ligaD(60);
        delay(500);
        /*
        paraTudo();
		delay(100);

		boostD(80,40);
        ligaD(50);
        delay(900);

        paraD(100);

        boostE(90,50);
        boostD(80,40);
        ligaE(80);
        ligaD(60);
		*/

        valor = digitalRead(inPin);
        while(valor==HIGH){//branco
          valor = digitalRead(inPin);
        }
        paraTudo();
        delay(100);

        boostE(80,40);
        ligaE(50);

        while (valor == HIGH){//branco
           medida();
           valor = digitalRead(inPin);//realizando a leitura no sensor optico
        }

        paraE(100);
        limiteD = 8;
    }
    void ligaE(int velo){
        analogWrite(Motor_EV, velo);
        digitalWrite(Motor_E1, LOW);
        digitalWrite(Motor_E2, HIGH);
    }
    void ligaD(int velo){
        analogWrite(Motor_DV, velo);
        digitalWrite(Motor_D1, HIGH);
        digitalWrite(Motor_D2, LOW);
    }
    void paraE(int temp){
        digitalWrite(Motor_E1, HIGH);
        digitalWrite(Motor_E2, HIGH);
        delay(temp);
    }
    void paraD(int temp){
        digitalWrite(Motor_D1, HIGH);
        digitalWrite(Motor_D2, HIGH);
        delay(temp);
    }
    void boostD(int velo, int temp){
        analogWrite(Motor_DV, velo);
        digitalWrite(Motor_D1, HIGH);
        digitalWrite(Motor_D2, LOW);
        delay(temp);
        paraD(10);
    }
    void boostE(int velo, int temp){

            analogWrite(Motor_EV, velo);
            digitalWrite(Motor_E1, LOW);
            digitalWrite(Motor_E2, HIGH);
            delay(temp);
            paraE(10);

    }
    void loop(){   
        
        valor = digitalRead(inPin);//realizando a leitura no sensor optico
        medida();
        
         boostE(90,50);
         ligaE(55);

        while (valor == HIGH){//branco
           medida();
           if(distancia<limiteD){
                desvia();
                boostE(80,40);
                ligaE(50);
            }
           valor = digitalRead(inPin);//realizando a leitura no sensor optico
        }
        
        while (valor != HIGH ){//preto
            medida();
            if(distancia < limiteD){
                desvia();
                boostE(80,40);
                ligaE(50);
        }
        valor = digitalRead(inPin);//realizando a leitura no sensor optico

     	}
          //para E
         paraE(100);
         boostD(80,40);
        ligaD(50);
        while (valor == HIGH){
            medida();
            if(distancia<limiteD){
                desvia();
                boostD(80,40);
                ligaD(50);
            }
            valor = digitalRead(inPin);//realizando a leitura no sensor optico
        }
           
         while (valor != HIGH){
            medida();
            if(distancia<limiteD){
                desvia();
                boostD(80,40);
                ligaD(50);
            }

            valor = digitalRead(inPin);//realizando a leitura no sensor optico

         }
          //para D
         paraD(100);   
    }