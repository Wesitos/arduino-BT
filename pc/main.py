#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

from __future__ import print_function
import sys
from comunicador import Comunicador

#Connecta al arduino
comunic = Comunicador("20:13:08:08:02:37",1)
comunic.conectar()

#Inicia la toma de datos
comunic.iniciar(5*"00001")

#Esto permitirá al programa ser interactivo
while True:
    try:
        entrada=sys.stdin.readline()
    except KeyboardInterrupt:
        print("Ctrl+c capturado!\nLimpiando todo.")
        comunic.terminar()
        sys.exit()
        
