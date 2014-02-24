// proyecto de tesis abraham
String A;
int numero=0;
String S,S1;
int T[150];
int t;
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
  int rec=Serial.available();
       if(rec>2)
       {
          config_tiempo(); 
       } 
       if(numero>2)
       {
         String mm="";
         for(int j=0;j<t;j++)
         {
         if( indice % T[j]==0 && T[j]!=0)
           {
	      mm=mm+lectura(j+1);
           } 
         } 
         if(mm.length()>1)
         {
         Serial.print(mm);
         }
         indice++;        
       } 
}

String lectura(int n)
{
  String x;
  int a=0; 
  int dato=0;
  switch(n)
    {//esto va a depender de la cantidad de sensores utilizados
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
    }
  c=int(b/1000);
  return c;
}


void config_tiempo()
{
  //queremos que el buffer entrante sea de la forma "26362;15655;55453;43544;52562;46591#"
         
        numero=Serial.readBytes(A);
        int i;
        int puntoycoma=0;
        int primer_puntoycoma=A.indexOf(';');
        int primer_michi=A.indexof('#');
        for(i=0;;i++)
        {
          S=String(A).substring(puntoycoma,primer_puntoycoma-1);//separamos TIEMPO1
          T[i]=S.toInt();
          puntoycoma=primer_puntoycoma;
          primer_puntoycoma=A.indexOf(';',primer_puntoycoma+1);
          S1=S1+S;
        } 
       t=T.length();
}
void separador()
{
 int n_inicial=-1;
 int i=0;
 do
 {
 	n_inicial=A.indexof('#',n_inicial+1);
 	i++;
 }while(n_inicial>=0)

}
