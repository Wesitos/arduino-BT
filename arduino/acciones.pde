//---------------------------
//Funciones de acciones del arduino
//---------------------------



//Esto asigna los tiempos
//Recibe un mensaje de la forma
//   T234;6546;343;23    //Sin el caracter #
//Despues, hay que modificarlo para que el mensaje sea de la forma
//   T;1,2323;2,3454;5,34534
void config_tiempo( char *mensaje)
{
  int i=0;
  unsigned tiempo;
  char *point_cad;
  //Hace
  mensaje++;
  point_cad = mensaje;

  do
    {
      strsep( &point_cad, SEP_MENS2);
      tiempo=atol( mensaje );
      mensaje = point_cad;

      //Asigna el tiempo i
      Tiempo[i]=tiempo;

      i++;
    }
  while(point_cad);
  
}     