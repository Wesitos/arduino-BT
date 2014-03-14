//Activa el mux en la dirección dada por valor
//Los pines de dirección son especificador en data
// LSBFIRST
void set_mux(int valor, byte* data, int len_data)
{
  for( int i=0; i < len_data; i++)
    {
      digitalWrite( data[i], ( valor%2 ) ? HIGH:LOW );
      valor /= 2;
    }

}