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
	{
	  //Pequeño delay para esperar a que llegue todo el mensaje
	  delay(5);
	  //El mensaje se pondrá despues del contenido de buf_mens
	  point_cad = buf_mens + strlen(buf_mens);
	  if (point_cad == buf_mens + TALLA_BUF )
	    //Overflow de buf_mens;
	    buf_overflow_error();

	  bytes_leidos =
	    Serial.readBytes( point_cad , TALLA_BUF - strlen(buf_mens) );

	  //Nos aseguramos que se coloque el fin de cadena
          point_cad[bytes_leidos] = '\0';
#ifdef DEBUG
	  Serial.println();
	  Serial.print( F("buf_mens: ") );
	  Serial.println(buf_mens);
#endif DEBUG

	}
      else
	break;

      //Busca la posicion del caracter separador
      point_sep_mens = strchr(buf_mens, SEP_MENS1[0]);

      while (point_sep_mens)
	{
      //Si la encuentra, lee el mensaje
	    recolecta_mensaje();
	    point_sep_mens = strchr(buf_mens, SEP_MENS1[0]);
	}
    }
  //El bucle termina cuando no hay más que leer
  while( bytes_disp );

 
}

void recolecta_mensaje()
{

  char *point_cad = buf_mens;
  char *inicio_mens = strchr( buf_mens , SEP_MENS2[0] );
  //Sacado de la documentacion de string.h
  //http://www.nongnu.org/avr-libc/user-manual/
  //  group__avr__string.html#gac0dbc25e8b202114031a4aa2a7c5177b
  //
  //Reemplaza el caracter delimitador por '\0'
  strsep(&point_cad, SEP_MENS1);
  //Adicionalmente, en point_cad ahora se almacena la dirección
  //del resto de mensajes

  //depuracion
#ifdef DEBUG
  Serial.print( F("Mensaje ") );
  Serial.println( buf_mens);
#endif
  //Ahora en buf_mens hay 2 cadenas seguidas una por la otra (ambas
  // con el caracter fin de cadena). Donde la primera cadena es
  //un mensaje.

  //Llama a una u otra funcion dependiendo del primer caracter
  //del mensaje
  //
  //Si se le pasa el mensaje a una funcion esta no debera contener
  //su primer elemento (tipo de mensaje)
  inicio_mens = strchr( buf_mens, SEP_MENS2[0] ) + 1;

  //depuracion
#ifdef DEBUG
  Serial.print( F(" inicio_mens ") );
  Serial.println( inicio_mens);
#endif
  //ESTO PUEDE MODIFICAR EL MENSAJE MAS NO MODIFICAR LOS MENSAJES
  //QUE LE SIGUEN
  
  switch(buf_mens[0])
    {
    case 'T':
      config_tiempo(inicio_mens);
      break;
    case 'I':
      inicia_medicion();
      break;
    case 'S':
	para_medicion();
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

void buf_overflow_error()
{}