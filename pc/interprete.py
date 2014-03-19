#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

import threading
from datetime import datetime
import time

class Interprete(threading.Thread):
    """Lee los mensajes y los procesa"""

    def __init__( self , cola ):
        threading.Thread.__init__(self)
        self.cola = cola
        self.dict_acc = { "T" : self.almacena_datos }
        self.activo = False

    def terminar(self):
        """Termina la ejecucion del interprete"""
        self.activo = False

    def run(self):
        """Se encarga de interpretar los mensajes """
        self.activo = True

        while(self.activo):
            try:
                mensaje = self.cola.get( block = False )
            except:
                time.sleep(0.01)
                continue
            else:
                self.interpreta(mensaje)


    def interpreta( self, mens ):
        """Lee un mensaje y realiza las acciones adecuadas con este 
        
        Lo que hay que hacer depende del primer elemento del mensaje"""
        
        self.dict_acc[ mens[0][0] ]( mens[1:] )
        
    def set_tables( self, tables):
        self.tables = tables

    def set_db( self, database):
        self.database = database

    def traduce_dato( valor ):
        """Utiliza datos de calibracion para a partir de estos 
        intepolar la temperatura"""
        
        """
        #Datos de calibracion (<voltaje> , <temperatura>)
        l_calibracion = [];
        piso = 0;

        for punto in l_calibracion:
            if punto > 
        
        """

        #Hasta que hayan datos de calibracion
        return valor;

    def almacena_datos( self , lista ):
        """Procesa un mensaje de data
 
        El primer elemento de la lista es el tiempo en segundos de 
        toma de datos. Los demas elementos son pares 
        (<id_sensor>,valor)"""
        tiempo = datetime.fromtimestamp( int(lista[0][0]) )

        for dato in lista[1:]:
            fila = ( tiempo , dato[1] )
            self.database.insert_one( fila , self.tables[ int (dato[0]) ] )
