//---------------------------
//Funciones de acciones del arduino
//---------------------------



//Esto asigna los tiempos
//Recibe un mensaje de la forma
//   T;1,2323;2,3454;5,34534 Sin el caracter #
void config_tiempo( char *mensaje)
{
  unsigned tiempo;
  int sensor;
  char *dato_actual;
  char *dato_sig = mensaje;

  char *par_actual;
  char *par_sig = mensaje;

  do
    {
      //Aislamos un elemento del mensaje
      dato_actual = strsep( &dato_sig, SEP_MENS2 );
      //Aislamos las partes del elemento
      par_actual = strsep ( &par_sig, SEP_MENS3 );
      
      //El primer elemento del par es el sensor
      sensor = atoi( par_actual );
      //El segundo elemento, el tiempo
      tiempo=atol( par_sig );

      //Asigna el tiempo al sensor
      Tiempo[sensor]=tiempo;

    }
  while(dato_sig);

  //si esta tomando datos, reinicia el tiempo
  if(toma_datos)
    inicializa_tiempo();

#ifdef DEBUG
  Serial.println();
  Serial.print( F("Sensores: ") );
  for (int i=0; i<N_SENS; i++)
    {
    Serial.print(Tiempo[i]);
    Serial.print( F(";") );
    }
  Serial.println();
#endif
}

//Empieza la acción
void inicia_medicion()
{
  inicializa_tiempo();
  toma_datos = 1;

#ifdef DEBUG
  Serial.println();
  Serial.println( F("--BEGIN--"));
  Serial.print( F(" Tiempo: ") );
  Serial.println(t_inicial);
#endif
}

void para_medicion()
{
  toma_datos = 0;

#ifdef DEBUG
  Serial.println();
  Serial.println( F("--STOP--") );
#endif
}

void reset_tiempo()
{
  for (int i=0; i<N_SENS;i++)
    Tiempo[i] = 0;
}