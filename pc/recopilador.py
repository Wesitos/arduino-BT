#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

from __future__ import print_function
import threading

class Recopilador(threading.Thread):
    """Se encarga de leer y almacenar los datos recibidos"""

    def __init__(self,bt_socket,cola,separador=';',traducir=None,volcar_datos=None):
        threading.Thread.__init__(self)
        #Tratamos la conexión como un archivo de texto
        self.stream = bt_socket.makefile("r")

        self.lista=cola
        self.sep = separador

        #Por defecto, no se traducen los datos
        if ( traducir == None ):
            self.traducir = lambda x:x
        else:
            self.traducir = traducir


        #Por defecto, se imprimen los datos en pantalla
        if ( volcar_datos == None ):
            self.volcar_datos = print
        else:
            self.volcar_datos = volcar_datos

    def terminar(self):
        self.activo = False

    def run(self):
        self.activo=True
        print("Recopilador funcionando")
        while self.activo:
            #print("Inicia bucle")
            #Cadena donde se almacena una lectura incompleta
            aux_cad = ""
            
            #Cadena donde se guarda lo recien leido del buffer de
            #entrada
            entrada = ""
            ultimo_char=self.stream.read(1)


            #Lee caracter a caracter hasta que encuentra el delimitador
            while ( ultimo_char != self.sep ):
                entrada+=ultimo_char
                ultimo_char=self.stream.read(1)
            
            if ( len(entrada)>0 ) :
                medida = self.traducir( entrada )
                #self.lista.put( medida )
                self.volcar_datos(medida)
        #Esto se ejecutara cuando finalize
        pass
