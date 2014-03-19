#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

from datetime import date
import tornado.escape
import tornado.ioloop
import tornado.web
from torndb import Connection
import settings
import atexit

db = Connection( settings.database["server"] ,\
                 settings.database["database"],\
                 user = settings.database["user"],\
                 password = settings.database["pass"]
            )

class DataHandler( tornado.web.RequestHandler ):
    def get(self, table, date):
        self.set_header('Content-Type', 'text/json')
        data = db.query("SELECT UNIX_TIMESTAMP(date),valor FROM "+\
                        settings.database["tables"][int(table)] +\
                       " WHERE UNIX_TIMESTAMP(date) > %s", int(date) )
        list_data = [ (x["UNIX_TIMESTAMP(date)"],x["valor"]) for x in data ]
        data_json = tornado.escape.json_encode(list_data)
        self.write( data_json ) 

class IndexHandler ( tornado.web.RequestHandler ):
    def get( self ):
        self.render("../web/new_index.html",\
                                l_chart = settings.web["l_chart"])
        
application = tornado.web.Application(
    [ ( r"/data/([0-9]+)/([0-9]+)", DataHandler ),
      ( r"/js/(.*)", tornado.web.StaticFileHandler, {"path":"../web/js"} ),
      ( r"/", IndexHandler ),
 ])

def deploy_server():
    application.listen(settings.web["port"])
    tornado.ioloop.IOLoop.instance().start()

@atexit.register
def clean_exit():
    tornado.ioloop.IOLoop.instance().stop()

if __name__ == "__main__":
    deploy_server()
