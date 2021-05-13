int out = 5;                                        //Pin digital on generarem el senyal PWM
int duty, percentatge, lectura;                     //Variables de tipus int per enmagatzemar el duty cycle, el percentatge i la lectura
int pinAnalogic = A0;                               //Pin d'entrada on volem mesurar el voltatge
double Vteoric, Vreal;                              //Variables de tipus double on enmagatzemarem el valor teòric i experimental de la tensiño.
double Vunitat = 0.00488758553275;                  //Variable per indicar els Volts per unitat de la lectura (vector de 10 bits -> 1024, Vref=5V, 5V/1023bits=0.0.00488758553V/unitat)
int iteracions = 20;                                //Nombre d'iteracions per promitjar

void setup() {
  pinMode(out, OUTPUT);                             //Declarem el pin de sortida com a output
  TCCR0B = TCCR0B & B11111000 | B00000001;          //Fem l'overclock de l'arduino per passar de 976.56Hz a 62.5kHz
  Serial.begin(9600);                               //Inicialitzem la comunicació per Serial a 9600 bauds
}

void loop() {
  Vreal = 0;                                        //Reiniciem la variable Vreal a 0
  delay(100000);                                    //Fem una espera  

  for (int i = 0; i <= 100; i++) {                  //Fem un bucle for per passar del 0 al 100%

    duty = map(i, 0, 100, 0, 255);                  //Calculem proporcionalment el valor entre 0 i 255 que correspon al percentatge desitjat
    analogWrite(5, duty);                           //Generem el senyal PWM
    delay(5000);                                    //Fem una petita espera perquè s'estabilitzin les tensions

    for (int j = 1; j < iteracions; j++) {          //Fem un bucle for per fer el promig
      lectura = analogRead(pinAnalogic);            //Llegim la variable de 10 bits del pin analògic pinAnalogic i la guardem a la variable lectura
      Vreal = Vreal + (lectura * Vunitat);          //Sumem a la variable Vreal el resultat de multiplicar els bits rebuts per Vunitat
    }
    Vreal = Vreal / iteracions;                     //Calculem el promig Vreal dividint la suma de les iteracions entre el nombre d'iteracions
    Vteoric = duty * 0.01961;                       //Calculem el valor teòric multiplicant el la variable duty (de 0 a 255) per 0.01960784313 (5v/255)
    Serial.print(Vteoric, 8);                       //Enviem per Serial el Vteoric amb 8 decimals
    Serial.print(";");  ¡
    Serial.println(Vreal, 8);                       //Enviem per Serial el Vreal amb 8 decimals
  }


}
