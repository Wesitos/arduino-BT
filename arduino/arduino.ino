//Programa del arduino
int numero=0;
char A[26];int T[5]={0,0,0,0,0}; 
int T1=0, T2=0, T3=0, T4=0, T5=0;
int pulso=1;
String S1,S2,S3,S4,S5;
String C="";
int indice=0; 
void setup()
{
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);//pin de alimentacion al sensor de temperatura
  pinMode(12,OUTPUT); //pines del bluetooth
  pinMode(10,INPUT);//entrada del pulsador
  
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(8,0);
}
void loop()
{
  //Lee cuantos bytes puede leer
  int rec=Serial.available();
    
  if(rec>2)
    conf_tiempo();

  if(numero>2)
    { 
      String mm="";
      for(int i=0;i<5;i++)
	if( indice % T[i]==0 && T[i]!=0)
	  mm=mm+lectura(i+1);

      mm=mm;
      if(mm.length()>1)
	Serial.print(mm);

      indice=indice+1;
    }; 
  delay(1000);
}

//-----funciones------
     
String lectura(int n)
{
  String x;int a=0; int dato=0;
  switch(n)
    {
    case 1:
      //TIEMPO DE MUESTREO DEL SENSOR 
      digitalWrite(13, HIGH);delay(9); dato=filtrador(0);digitalWrite(13, LOW);
      x=String(dato)+";";break;
    case 2: 
      //TIEMPO DE MUESTREO DEL SENSOR 
      digitalWrite(13, HIGH);delay(9); dato=filtrador(1);digitalWrite(13, LOW);
      x=String(dato)+";";break;
    case 3:
      //TIEMPO DE MUESTREO DEL SENSOR 
      digitalWrite(13, HIGH);delay(9); dato=filtrador(2);digitalWrite(13, LOW);
      x=String(dato)+";";break;
    case 4:
      //TIEMPO DE MUESTREO DEL SENSOR 
      digitalWrite(13, HIGH);delay(9); dato=filtrador(3);digitalWrite(13, LOW);
      x=String(dato)+";";break;
    case 5:
      //TIEMPO DE MUESTREO DEL SENSOR 
      digitalWrite(13, HIGH);delay(9); dato=filtrador(4);digitalWrite(13, LOW);
      x=String(dato)+";";break;
    }
  return x;
}


int filtrador(int n)
{
  long b=0, c=0, a=0;
  for(int i=0; i<1000;i++)
    {
      a=analogRead(n);
      b=b+a;
    };
  c=int(b/1000);return c;
}

void conf_tiempo()
{
  // el buffer entrante es de la forma "120005465200343211223"
  numero=Serial.readBytes(A,25);
    
  //Los tiempos maximos son un dia     
  S1=String(A).substring(0,5);// separamos TIEMPO1
  T1=S1.toInt();//eliminamos los ceros
    
  S2=String(A).substring(5,10);// separamos TIEMPO2
  T2=S2.toInt();//eliminamos los ceros
    
  S3=String(A).substring(10,15);// separamos TIEMPO3
  T3=S3.toInt();//eliminamos los ceros
    
  S4=String(A).substring(15,20);// separamos TIEMPO4
  T4=S4.toInt();//eliminamos los ceros
    
  S5=String(A).substring(20,25);// separamos TIEMPO5
  T5=S5.toInt();
  T[0]=T1;T[1]=T2;T[2]=T3;T[3]=T4;T[4]=T5;
}