#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

from __future__ import print_function
import bluetooth as bt
import time
import Queue
import recopilador

class Comunicador():
    """Esta es la clase que maneja la comunicacion con el Arduino"""

    def __init__(self, mac,puerto,separador=";" ):
        self.bt_direcc = mac
        self.port = puerto
        self.crear_socket()

    def crear_socket(self):
        """Crea, o vuelve a crear, el socket de conexión"""
        self.socket = bt.BluetoothSocket(bt.RFCOMM)

    def conectar(self):
        """Conecta el socket
        En caso de error, reintenta la conexión 5 veces. Luego, se rinde"""
        destino = ( self.bt_direcc , self.port )
        print("Intentando conexion")
        #Reintenta la conexion 5 veces
        for i in range(0,5):
            try:
                self.socket.connect( destino )
            except:
                print("Error al intentar conexion, reintentando")
                time.sleep(2)
                self.crear_socket()
            else:
                print("Conexion exitosa")
                break
        print("Imposible conectarse al destino")


    def enviar_mensaje(self,mensaje):
        """Manda una cadena
        """
        for i in range(0,5):
            try:
                self.socket.send(mensaje)
            except:
                print("Error al enviar mensaje, reintentando")
                time.sleep(2)
            else:
                break
        print("Imposible enviar mensaje al destino.")

    def iniciar( dicc,traducir=None,volcar_datos=None ):
        """Despliega el recopilador y comunica las configuración de las mediciones
        al arduino"""
        self.lista = Queue.Queue()
        
        recop = Recopilador(self.socket,self.lista,traducir,volcar_datos)
        
        #Iniciamos el recopilador
        recop.setDaemon(True)
        recop.start()
        self.recop= recop

        
