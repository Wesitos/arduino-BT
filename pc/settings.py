# -*- coding: iso-8859-15 -*-

#Archivo de opciones por defecto

#Configuracion de la base de datos
database = {
    "server" : "localhost",
    "database" : "python",
    "user" : "python",
    "pass" : "python",
    "dbm" : "MYSQL",
    "tables": [ "temp%d"%x for x in range(8) ],
}

#Periodos de muestreo
tiempos = [ (0,1) , (1,3) ]

#Servidor web
web = {
    "port" : 8888,
    "l_chart" : (
        { "div_id" : "chart_0",
          "sens_id": 0,
          "title"  : "Sensor 0"
      },
        { "div_id" :  "chart_1",
          "sens_id": 1,
          "title"  : "Sensor 1"
      },
        { "div_id" :  "chart_2",
          "sens_id": 2,
          "title"  : "Sensor 2"
      },
        { "div_id" :  "chart_3",
          "sens_id": 3,
          "title"  : "Sensor 3"
      },
        { "div_id" :  "chart_4",
          "sens_id": 4,
          "title"  : "Sensor 4"
      },
        { "div_id" :  "chart_5",
          "sens_id": 5,
          "title"  : "Sensor 5"
      },
        { "div_id" :  "chart_6",
          "sens_id": 6,
          "title"  : "Sensor 6"
      },
        { "div_id" :  "chart_7",
          "sens_id": 7,
          "title"  : "Sensor 7"
      }
    )
}
