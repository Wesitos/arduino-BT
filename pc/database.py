#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

class Database():
    """Clase para facilitar el manejo de distintos tipos de bases de datos """
    
    def __init__(self,host, database=None, username=None,password=None,dbm="SQLITE"):
        self.server = host
        self.db     = database
        self.user   = username
        self.psw    = password

        #Diccionario de tipos de base de datos
        db_dict = { "MYSQL"   : self.mysql_connect,
                    "SQLITE"  : self.sqlite_connect,
                    }
        cod_dict = { "MYSQL" : "%s",
                     "SQLITE" : "?",
                     }

        #Ejecuta la funcion connect correspondiente a la base de datos
        #indicada
        db_dict[dbm]()
        #Selecciona el caracter de reemplazo a utilizarce
        self.cod = cod_dict[dbm]
        self.cursor = self.conexion.cursor()

    #Metodo de SQLite
    #--------------------
    def sqlite_connect(self):
        import sqlite3 as dbapi
        
        self.conexion = dbapi.connect(self.server)

    #Metodo de MySQL
    #-------------------
    def mysql_connect(self):
        import MySQLdb as dbapi

        self.conexion = dbapi.connect(self.server, self.user, self.psw, self.db)



    def insert_one(self,fila,tabla):
        """Inserta "fila" en "tabla" """

        #si es una fila de un solo valor, la convierte en una tupla de
        #tamaño 1
        if isinstance(fila,basestring):
            fila = fila, 

        cadena_base = "INSERT INTO "+tabla+" VALUES"
        #Crea la subcadena donde se reemplazaran los valores
        #Por ejemplo, si son 2 valores por fila: (?,?)
        cod = self.cod
        cadena_format = "("+ (cod+",")*( len(fila) - 1 )+cod+ ")"

        cadena_sentencia = cadena_base + cadena_format

        with self.conexion:
            self.cursor.execute(cadena_sentencia,fila)

    def version(self):
        pass

    def desconectar(self):
        """Desconecta de la base de datos de manera segura """
        self.cursor.close()
        self.conexion.close()

            
            
