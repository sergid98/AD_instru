#include <Arduino.h>
#include <math.h>                                   //Llibreria per poder per els càlculs d'exponencials

double Vteoric, Vreal;                              //Variables tipus double on guardarem el voltatge teòric calculat, el temps en segons i i el voltatge mesurat per l'ADC, respectivament
long tempsmillis, tempsmillisbase;                  //Variable tipus long on guardarem el temps en milisegons
int estat;                                          //Variable int que ens servirà per controlar la màquina d'estats

long cicle = 30000;                                 //Variable per indicar els milisegons que durarà el cicle de càrrega/descàrrega
float RC = 2.97;                                    //Variable per indicar el resultat de multiplicar el valor de la resistència (en Ohm) per la capacitat (en Farads)
double Vunitat = 0.00488758553275;                  //Variable per indicar els Volts per unitat de la lectura (vector de 10 bits -> 1024, Vref=5V, 5V/1024bits=0.0048828125V/unitat)
int pinAnalogic = A0;                               //Pin d'entrada on volem mesurar el voltatge
int sortida = 2;                                    //Pin on conectarem el rele
int t=120;                                          //Temps entre enviaments
int esperainicial=30000;

void carrega(double tempscarrega);                  //Funció que cridarem durant la càrrega
void descarrega(double tempsdescarrega);            //Funció que cridarem durant la descàrrega

void setup() {
  Serial.begin(9600);                               //Inicialitzem la connexió Serial a 9600 bauds
  tempsmillisbase = esperainicial;                  //Inicialitzem la variable tempsmillisbase a 30000
  estat = 0;                                        //Inicialitzem la variable d'estat a 0
  pinMode(sortida, OUTPUT);                         //Declarem el pin del relé com a sortida
  digitalWrite(sortida, LOW);
  delay(esperainicial);
}


void loop() {
  switch (estat) {                                  //Discriminem per estat

    case 0:                                         //Estat 0: càrrega del condensador
      digitalWrite(sortida, HIGH);                  //Activem el relé
      tempsmillis = millis();                       //Guardem a la variable tempsmillis els milisegons actuals

      if (tempsmillis - tempsmillisbase >= cicle) { //Si la diferència entre tempsmillis i tempsmillisbase és major a la variable cicle
        tempsmillisbase = tempsmillis;              //Prenem tempsmillis com a tempsmillisbase per tal de utilitzar-ho com a 0
        estat = 1;                                  //Passem a l'estat 1, descàrrega
        break;                                      //Sortim del case 0
      }

      else {                                        //Mentres la diferència entre tempsmillis i tempsmillisbase sigui menor a la variable cicle
        carrega(tempsmillis - tempsmillisbase);     //Executem la funció càrrega al temps Temps
        break;                                      //Sortim del case 0
      }


    case 1:                                         //Estat 1: descàrrega del condensador
      digitalWrite(sortida, LOW);                   //Desactivem el relé
      tempsmillis = millis();                       //Guardem a la variable tempsmillis els milisegons actuals

      if (tempsmillis - tempsmillisbase >= cicle) { //Si la diferència entre tempsmillis i tempsmillisbase és major a la variable cicle
        tempsmillisbase = tempsmillis;              //Prenem tempsmillis com a tempsmillisbase per tal de utilitzar-ho com a 0
        estat = 2;                                  //Passem a l'estat 0, càrrega
        break;                                      //Sortim del case 1
      }

      else {                                        //Mentres la diferència entre tempsmillis i tempsmillisbase sigui menor a la variable cicle
        descarrega(tempsmillis - tempsmillisbase);  //Executem la funció càrrega al temps Temps
        break;                                      //Sortim del case 1
      }
    
    case 2:                                         //Estat d'enclavament
    break;
  }
}


void carrega(double tempscarrega) {                 //Funció que cridarem durant la càrrega
  Vteoric = 5 * (1 - (exp((-tempscarrega) / (RC*1000.0))));  //Calculem Vteoric segons el temps
  int lectura = analogRead(pinAnalogic);            //Llegim la variable de 10 bits del pin analògic pinAnalogic i la guardem a la variable lectura
  Vreal = lectura * (Vunitat);                      //Calculem el voltage (multiplicant la lectura entre 0 i 1023 per el voltatge corresponent a cada unitat) i el guardem a Vreal
  Serial.print(tempsmillis/1000.0,3);               //Imprimim per serie el temps en segons amb 3 decimals
  Serial.print(",");                                //Separem per una coma
  Serial.print(Vteoric,10);                         //Imprimim per serie el valor teòric calculat amb 10 decimals
  Serial.print(",");                                //Separem per una coma
  Serial.println(Vreal,10);                         //Imprimim per serie el valor real captat per l'ADC amb 10 decimals
  delay(t);
}

void descarrega(double tempsdescarrega) {           //Funció que cridarem durant la descàrrega
  Vteoric = 5 * (exp((-tempsdescarrega) / (RC*1000.0))); //Calculem Vteoric segons el temps
  int lectura = analogRead(pinAnalogic);            //Llegim la variable de 10 bits del pin analògic pinAnalogic i la guardem a la variable lectura
  Vreal = lectura * (Vunitat);                      //Calculem el voltage (multiplicant la lectura entre 0 i 1023 per el voltatge corresponent a cada unitat) i el guardem a Vreal
  Serial.print(tempsmillis/1000.0,3);               //Imprimim per serie el temps en segons amb 3 decimals
  Serial.print(",");                                //Separem per una coma
  Serial.print(Vteoric,10);                         //Imprimim per serie el valor teòric calculat amb 10 decimals
  Serial.print(",");                                //Separem per una coma
  Serial.println(Vreal,10);                         //Imprimim per serie el valor real captat per l'ADC amb 10 decimals
  delay(t);
}