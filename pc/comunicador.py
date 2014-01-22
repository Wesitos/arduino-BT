#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

import bluetooth as bt
import time

#Esta es la clase que maneja la comunicacion con el Arduino
class Comunicador():
    
    def __init__(self, mac,puerto):
        self.bt_direcc = mac
        self.port = puerto
        self.crear_socket()

    def crear_socket(self):
        self.socket = bt.BluetoothSocket(RFCOMM)

    def connectar(self):
        destino = ( self.bt_direcc , self.port )
        print "Intentando conexion"
        #Reintenta la conexion 5 veces
        for i in range(0,5):
            try:
                self.socket.connect( destino )
            except:
                print "Error al intentar conexion, reintentando"
                time.sleep(2)
                self.crear_socket()
            else:
                print "Conexion exitosa"
                break

    def enviar_mensaje(self,mensaje):
        for i in range(0,5):
            try:
                self.socket.send(mensaje)
            except:
                print "Error al enviar mensaje, reintentando"
                time.sleep(2)
            else:
                break
