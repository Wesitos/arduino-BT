//----------------------
//Funciones de tiempo
//----------------------

//Variables globales:
//  long t_inicial
//  longt_previo
//  long t_actual
//  RTC_DS1207 rtc

//Asigna el valor de tiempo inicial, reinicia los otros contadores de
//tiempo
void inicializa_tiempo()
{
  aux_time = rtc.now();
  //Por hacer, averiguar por que si no se le pone +1 no realiza
  //medicion para t_actual == t_inicial
  t_inicial = aux_time.unixtime() + 1 ;
  t_previo = t_inicial;
  t_actual = t_inicial;
}


//Esta funcion actualiza t_actual.
//Devuelve la diferencia de este con t_previo en segundos
long actualiza_tiempo()
{
  long dif;

  aux_time = rtc.now();
  t_actual = aux_time.unixtime();
  
  dif = t_actual-t_previo;
  
  if ( ! dif)
    return dif;
  else
    //Si es el inicio de la medicion
    return 1;
}

//Indica si ya paso una multiplo entero de periodo (segundos)
int verifica_tiempo(unsigned periodo)
{
  long dif;

  if (! periodo)
    return 0;

  dif = t_actual-t_inicial;
  //Tiempos antes de t_inicial no son validos
    if( dif < 0 )
       return 0;

  if ( dif % periodo )
    return 0;
  else
    return 1;
}
