#!/usr/bin/python
# -*- coding: iso-8859-15 -*-
from comunicador import Comunicador
from database import Database
from mensajes import mens_dicc

from cmd2 import Cmd


class Cli(Cmd):

    comunicador = None
    database = None
    address = "20:13:08:08:02:37"
    port = 1;
    prompt = "arduino-BT>>"
    intro = mens_dicc["WELCOME"]
    locals_in_py = False

    @options([make_option('-a','--address',type='str'),
              make_option('-p','--port',type='int'),
              make_option(),
              ])
    def do_conectar(self,arg, opts=None):
        """Crea una instancia de Comunicador y la conecta"""
        
        self.comunicador = Comunicador(addr,port)
        self.comunicador.conectar()
    
        



if __name__ == '__main__':
    Cli().cmdloop()
