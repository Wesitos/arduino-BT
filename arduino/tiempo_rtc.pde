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
  t_inicial = aux_time.unixtime();
  t_previo = t_inicial;
  t_actual = t_inicial;
}


//Esta funcion actualiza t_actual.
//Devuelve la diferencia de este con t_previo en segundos
long actualiza_tiempo()
{
  t_actual = aux_time.unixtime();
  return t_actual - t_previo;
}

//Devuelve los segundos que han pasado desde el inicio de la toma de
//datos
unsigned get_tiempo()
{
  return t_actual - t_inicial;
}

//Indica si ya paso una multiplo entero de periodo (segundos)
int verifica_tiempo(unsigned periodo)
{
  long dif = t_actual - t_inicial;
  if ( dif % periodo )
    return 0;
  else
    return 1;
}
