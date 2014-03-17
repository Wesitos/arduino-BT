//Programa del arduino

//Problemas a abordar:
//    -El buffer de lectura del puerto serial almacena a lo mucho 64
//     bytes, si el mensaje es muy largo es necesario fragmentarlo.
//     O talvez modificar la libreria HardwareSerial.cpp en el
//     compilador de arduino (al parecer, es posible).
//
//    -Es posible que requiramos uso de mucha memoria, hay que diseñar
//     el programa de tal forma que el uso de esta se minimice
//    
//    -Hay que diseñar el programa de tal forma que se pueda luego
//     agregar mayor cantidad de sensores por otros metodos
//
//    -Se deben tomar datos por tiempos muy prolongados de tiempo
//     el reloj del arduino no es lo suficientemente preciso. Hay que
//     utilizar un reloj externo o sincronizar el tiempo con la
//     computadora periodicamente.

#include<string.h>

//Librerias para utilizar el RTC
#include "Wire.h"          
// https://github.com/adafruit/RTClib
#include "RTClib.h"

//Numero de sensores
#define N_SENS 5

#define TALLA_BUF 200

//Pines utilizados
#define PIN_RELAY 12
#define PIN_LED 13
//Pin analogico de lectura
#define DATA_PIN 0

//Caracteres separadores de mensajes (ASCII) (# ; , )
//Como cadena para utilizarlos en strsep
char SEP_MENS1[] = "\x23";
char SEP_MENS2[] = "\x3B";
char SEP_MENS3[] = "\x2C";

//Cadena de caracteres, como C manda
char buf_mens[TALLA_BUF+1];

//Array de periodos de tiempo
unsigned Tiempo[N_SENS];
byte Aux_Tiempo[(N_SENS - 1)/8 + 1];


//--- Tiempo
RTC_DS1307   rtc;
//Variable auxiliar
DateTime aux_time;

//Variables temporales (segundos)
//Tiempo del inicio de la toma de medidas
long t_inicial;
//Tiempo de la ultima medida
long t_previo;
//Tiempo "actual"
long t_actual;

//MUX
//Numero de bytes de direccion ( log_2( N_SENS) )
#define N_MUX 3
//Array de pines de seleccion de direccion LSBFIRST
byte Data[N_MUX] = {8,9,10};


int toma_datos;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.setTimeout(100); //tiempo de espera maximo

  buf_mens[0] = '\0';

  t_inicial = 0;
  t_previo = 0;
  t_actual = 0;

  inicializa_tiempo();
  //Inicializa el array de periodos
  reset_tiempo();

  //Al inicio no realizamos mediciones
  toma_datos = 0;
}

//Tratando de simplificar la estructura del programa
void loop()
{
  //Verifica si hay informacion por leer del puerto serial
  if ( Serial.available() )
    lee_serial();

  else if( toma_datos &&  actualiza_tiempo() )
    {
      realiza_medicion();
      delay(100);
    }
  
}

//Falta implementar, lo que se va a ejecutar cuando lo que se lee del
//buffer serial no entra en aux
void aux_size_error()
{
}

//Devuelve el indice del ultimo sensor activado
//La informacion de que sensores se deben utilizar es actualizada
//por la funcion recolecta_sensores
int ultimo_sensor()
{
  int ultimo = N_SENS-1;

  while( ultimo )
    {
      if ( verifica_sensor(ultimo) )
	break;
      
      ultimo++;
    }
  return ultimo;
}

//Actualiza Aux_Tiempo
int recolecta_sensores()
{
  int cont_sens = 0;
  for (int i=0; i < N_SENS/8 + 1; i++)
    for ( int j=0 ; (8*i + j  < N_SENS) && (j < 8) ; j++)
      if ( verifica_tiempo( i*8 + j ) )
	{//Set bit
	  Aux_Tiempo[i] |= 1 << j;
	  cont_sens++;
	}
      else
	//Unset bit
	Aux_Tiempo[i] &= ~(1 << j);
  
  return cont_sens;
}

int imprime_sensores()
{
  for (int i=0; i < N_SENS/8 + 1; i++)
    for ( int j=0 ; (8*i + j  < N_SENS) && (j < 8) ; j++)
      if ( verifica_tiempo( i*8 + j ) )
	Serial.print(1);
      else
	Serial.print(0);
  Serial.println();
}

//Devuelve si se debe utilizar un sensor
//La informacion de que sensores se deben utilizar es actualizada
//por la funcion recolecta_sensores
int verifica_sensor(int i)
{
  //Mas vale prevenir...
  if (i >= N_SENS)
    return 0;

  if (Aux_Tiempo[(i-1)/8] & ( 1 << (i%8) ) )
    return 1;
  else
    return 0;
      
}

int lectura( int sensor )
{
  //Selecciona el sensor
  set_mux( sensor , Data , N_MUX );
  return analogRead(DATA_PIN);
}


//Realiza las mediciones y las envia
void realiza_medicion()
{
  int cont_sens = recolecta_sensores();
  Serial.println();
  Serial.print( F("Recolecta_sensores: ") );

  Serial.print(cont_sens);
  Serial.print( F( "Sensores;") );
  imprime_sensores();  

  //Si hay sensores que utilizar
  if ( cont_sens )
    {
      digitalWrite(PIN_LED, HIGH);
      Serial.print( F("T") );
      Serial.print( SEP_MENS2 );
      Serial.print( t_actual );
      Serial.print( SEP_MENS2);
      //Ejem:
      // T;1394830169;
    }

  int ultimo = ultimo_sensor();

  for (int i = 0; i <= ultimo; i++)
    {
      if (  verifica_sensor(i) )  
	{
	  Serial.print(i);
	  Serial.print( SEP_MENS3 );
	  Serial.print( lectura(i) );
	  //Ejem:
	  // 1,253

	  if( i != ultimo)
	    Serial.print(SEP_MENS2);
	  else
	    Serial.print(SEP_MENS1);
	}
    } 
  digitalWrite(PIN_LED , LOW);
  //Actualiza t_previo
  t_previo = t_actual;
}