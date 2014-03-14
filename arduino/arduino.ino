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
#define TALLA_AUX 11

//Caracteres separadores de mensajes (ASCII) (# ; , )
//Como cadena para utilizarlos en strsep
char SEP_MENS1[] = "\x35";
char SEP_MENS2[] = "\x59";
char SEP_MENS3[] = "\x46";

//Pines utilizados
#define PIN_VCC_SENS 13
#define PIN_RELAY 8
#define PIN_LED 12

//Cadena de caracteres, como C manda
char buf_mens[TALLA_BUF+1];

char aux[TALLA_AUX+1];

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
//Numero de bites de direccion
#define N_MUX 3
//Array de pines de seleccion de direccion LSBFIRST
byte Data[N_MUX] = {8,9,10};
//Pin analogico de lectura
#define DATA_PIN 0

int toma_datos;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  
  pinMode(PIN_VCC_SENS,OUTPUT);//pin de alimentacion al sensor de temperatura
  digitalWrite(PIN_VCC_SENS, LOW);

  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.setTimeout(100); //tiempo de espera maximo

  t_inicial = 0;
  t_previo = 0;
  t_actual = 0;

  inicializa_tiempo();
  //Inicializa el array de periodos
  for (int i=0; i<N_SENS;i++)
    Tiempo[i] = 0;

  //Al inicio no realizamos mediciones
  toma_datos = 0;
}

//Tratando de simplificar la estructura del programa
void loop()
{
  //Verifica si hay informacion por leer del puerto serial
  if ( Serial.available() )
    lee_serial();

  else if( realiza_medicion &&  actualiza_tiempo() )
    {
      realiza_medicion();
      delay(100);
    }
  
}

//Esta funcion lee del puerto serial hasta que este quede vacio
void lee_serial()
{
  char bytes_disp = 0;
  char bytes_leidos = 0;
  int len_buf;
  char *point_sep_mens = NULL;
  char *point_cad = NULL;

  do
    {
      bytes_disp= Serial.available();

      if ( bytes_disp )
	//Lee del buffer serial hacia aux
	//Lee a lo mucho TALLA_AUX bytes
	bytes_leidos=
	  Serial.readBytes(aux,
			   bytes_disp < TALLA_AUX ? bytes_disp : TALLA_AUX);

      //concatena lo leido en buf_mens
      len_buf=strlcat( buf_mens,aux, TALLA_BUF+1 );
	
      //Si el contenido de aux no entró en buf_mens
      if(len_buf >= TALLA_BUF+1)
	aux_size_error();
	
      if (! bytes_leidos)
	break;    //En caso de timeout deja de leer caracteres

      //Busca la posicion del caracter separador
      point_sep_mens = strchr(buf_mens, SEP_MENS1[0]);
      
    }
  //El bucle termina cuando se encuentra un caracter delimitador
  while( ! point_sep_mens);

  //En este punto debo tener un mensaje en buf_mens
  //o tengo un mensaje incompleto en el.
  //Verifico si hay un mensaje completo, en ese caso, lo leo


  if (point_sep_mens)
    {
      point_cad=buf_mens;
      //Sacado de la documentacion de string.h
      //http://www.nongnu.org/avr-libc/user-manual/
      //  group__avr__string.html#gac0dbc25e8b202114031a4aa2a7c5177b
      //
      //Reemplaza el caracter delimitador por '\0'
      strsep(&point_cad, SEP_MENS1);
      //Copia el mensaje a aux
      strcpy(aux,point_cad);

      //Llama a una u otra funcion dependiendo del primer caracter
      //del mensaje
      //
      //ESTAS FUNCIONES MODIFICAN AUX
      switch(aux[0])
	{
	case 'T':
	  config_tiempo(aux);
	  break;
	case 'I':
	  inicia_medicion();
	  break;
	case 'S':
	  toma_datos = 0;
	  break;
	  //Aqui van otros tipos de mensajes
	}
      
      //Borra el mensaje de buf_mens. El contenido de la cadena
      //despues del primer mensaje es movido al principio de esta
      if (point_cad)
	memmove( buf_mens, point_cad, strlen(point_cad)+1 );
      else
	buf_mens[0]='\0';
      //Ahora solo queda lo que le seguia al primer mensaje
      //posiblemente un mensaje incompleto o una cadena vacia

    }
}
 
//Falta implementar, lo que se va a ejecutar cuando lo que se lee del
//buffer serial no entra en aux
void aux_size_error()
{
}

//Devuelve el indice del ultimo sensor activado
int ultimo_sensor()
{
  int ultimo = 0;

  for(int i = 0; i < N_SENS ; i++)
    if ( verifica_tiempo( Tiempo[i] ))
      ultimo = i;
  
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
	Aux_Tiempo[i] &= 1 << j;
  
  return cont_sens;
}

//Devuelve si se debe utilizar un sensor
int verifica_sensor(int i)
{
  //Mas vale prevenir...
  if (i >= N_SENS)
    return 0;

  if (Aux_Tiempo[(i-1)/8] & 1 << (i%8) )
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

//Empieza la acción
void inicia_medicion()
{
  inicializa_tiempo();
  toma_datos = 1;
  
}

//Realiza las mediciones y las envia
void realiza_medicion()
{
  int ultimo = ultimo_sensor();

  for (int i = 0; i <= ultimo; i++)
    {
      if (  verifica_sensor(i) )  
	{
	  Serial.print(i);
	  Serial.print( SEP_MENS3 );
	  Serial.print( lectura(i) );

	  if( i != ultimo)
	    Serial.print(SEP_MENS2);
	  else
	    Serial.print(SEP_MENS1);
	}
    } 
  //Actualiza t_previo
  t_previo = t_actual;
}