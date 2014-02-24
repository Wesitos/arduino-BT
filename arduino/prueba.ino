//Programa del arduino

//Problemas a abordar:
//    -El buffer de lectura de lpuerto serial almacena a lo mucho 64
//     bytes, si el mensaje es muy largo es necesario fragmentarlo.
//     O talvez modificar la libreria HardwareSerial.cpp en el
//     compilador de arduino (al parecer, es posible).
//
//    -Es posible que requiramos uso de mucha memoria, hay que diseñar
//     El programa de tal forma que el uso de esta se minimice
//    
//    -Hay que diseñar el programa de tal forma que se pueda luego
//     agregar mayor cantidad de sensores por otros metodos
//
//    -Se deben tomar datos por tiempos muy prolongados de tiempo
//     El reloj del arduino no es lo suficientemente preciso. Hay que
//     Utilizar un reloj externo o sincronizar el tiempo con la
//     computadora periodicamente.

//grrrrr
#include<string.h>
//Numero de sensores
#define N_SENS 5

#define TALLA_BUF 200
#define TALLA_AUX 11

//Caracteres separadores de mensajes (ASCII) (# ; , )
#define SEP_MENS1 35
#define SEP_MENS2 59
#define SEP_MENS3 46

//Cadena de caracteres, como C manda
char buf_mens[TALLA+1];
char aux[TALLA_AUX+1];

//Array de periodos
unsigned Tiempo[N_SENS]

void setup()
{
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);//pin de alimentacion al sensor de temperatura
  pinMode(12,OUTPUT); //pines del bluetooth
  pinMode(10,INPUT);//entrada del pulsador
  
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(8,0);

  Serial.setTimeout(200); //tiempo de espera maximo

  //Inicializa el array de periodos
  for (i=0; i<N_SENS;i++)
    Tiempo[i] = 0;
}


//Tratando de simplificar la estructura del programa
void loop()
{
  //Verifica si hay informacion por leer del puerto serial
  if (Serial.available() )
    lee_serial();

  else if( verifica_tiempo() )
    realiza_medicion();
  
  //Retardo para evitar iteraciones innecesarias
  delay(200);
}



//Esta funcion lee del puerto serial a lo mucho un mensaje
//
void lee_serial();
{
  char bytes_disp;
  char bytes_leidos;
  int len_buf;
  char *point_sep_mens;
  char **point_cad;

  do
    {
      bytes_disp= Serial.available();

      if ( bytes_disp )
	//Lee del buffer serial
	//Lee a lo mucho TALLA_AUX bytes
	bytes_leidos=
	  Serial.readBytes(aux,
			   bytes_disp<TALLA_AUX?bytes_disp:TALLA_AUX);

      //concatena lo leido en buf_mens
      len_buf=strlcat( buf_mens,aux, TALLA+1 );
	
      //if(len_buf > TALLA+1)
      //no se pudo almacenar todo el contenido de aux 
      //, falta implementar esto
	
      if (! bytes_leidos)
	//En caso de timeout deja de leer caracteres
	break;
      //Busca la posicion del caracter separador
      point_sep_mens = strchr(buf_mens, SEP_MENS1);
	
    }
  while( ! point_sep_mens);

  //En este punto debo tener un mensaje en buf_mens
  //o tengo un mensaje incompleto en el.
  //Verifico si hay un mensaje completo, en ese caso, lo leo


  if (point_sep_mens)
    {
      point_cad=&buf_mens;
      //Sacado de la documentacion de string.h
      //http://www.nongnu.org/avr-libc/user-manual/
      //  group__avr__string.html#gac0dbc25e8b202114031a4aa2a7c5177b
      strsep(point_cad, "\xSEP_MENS1");
      //Copia el mensaje a aux
      strcpy(aux,*point_cad);

      //Llama a una u otra funcion dependiendo del primer caracter
      //del mensaje
      switch(aux[0])
	{
	case 'T':
	  config_tiempo(aux);
	  break;

	  //Aqui van otros tipos de mensajes
	}

      //Borra el mensaje de buf_mens
      strcpy(buf_mens,*point_cad);
      //Ahora solo queda lo que le seguia al primer mensaje
      //posiblemente un mensaje incompleto o una cadena vacia

    }

}

//Esto asigna los tiempos
//Recibe un mensaje de la forma
//   T234;6546;343;23    //Sin el caracter #
//Despues, hay que modificarlo para que el mensaje sea de la forma
//   T1,2323;2,3454;5,34534
void config_tiempo( char *mensaje)
{
  //Tengo sueño
  char **point_cad;
  point_cad = &mensaje;
 
  //zzz
  //zzzzzz
  //zzzzzzzzzz
  
}
     