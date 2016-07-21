# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
import requests
import os
import time

url = "http://medialab-prado.es/events"

# Keep the program running.

while 1:
    req = requests.get(url)
    statusCode = req.status_code

    if statusCode == 200:

        print "Dentro del parser"
        html = BeautifulSoup(req.text,"html.parser")
        #lugares = html.find_all('a',{'href':'lugar'})
        #views = html.find_all('div',{'class':'watch-view-count'})
        """
        lista = html.find_all('ul',{'class':'lista'})
        for a in lista.find( href=True):
            print "Found the URL:", a.contents
        """

        print "Dentro del parser"
        html = BeautifulSoup(req.text,"html.parser")
        listas = html.find_all('li')

        #print listas[0]

        for a in listas[5].find_all(href=True):
            print "Found the URL:", a.contents
        """
        for i,lista in enumerate(listas):
            # Con el método "getText()" no nos devuelve el HTML
            for a in lista.find(href=True):
                print "Found the URL:", a.contents

        for i,lugar in enumerate(lugares):
            # Con el método "getText()" no nos devuelve el HTML
            numero_view = lugar.getText()
            print numero_view
        """
    else:
        print "Status Code %d" %statusCode

    time.sleep(1)
