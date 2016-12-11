# -*- coding: utf-8 -*-
import sys
import time
from bs4 import BeautifulSoup
import requests
import telepot

def handle(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)

    from_user =  msg.get('from')
    username = from_user.get('username')
    first_name = from_user.get('first_name')

    print(content_type, chat_type, chat_id, username, first_name)

    '''
    if content_type == 'text':
        bot.sendMessage(chat_id, msg['text'])
    '''
    if content_type == 'text':
        print msg['text'].encode('unicode-escape').decode('ascii')
        if msg['text'] == "nieve" or msg['text'] =="Nieve":
            scrap_page(msg)
    if content_type == 'new_chat_member':
        bot.sendMessage(chat_id, "Wellcome to this awesome group!")
    if content_type == 'left_chat_member':
        bot.sendMessage(chat_id, "Sayonara baby")


def scrap_page(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)
    url = "http://www.infonieve.es/estaciones-esqui/sistema-central/6/"

    # Realizamos la peticion a la web
    req = requests.get(url)

    # Comprobamos que la petición nos devuelve un Status Code = 200
    statusCode = req.status_code

    if statusCode == 200:

        bot.sendMessage(chat_id,"Estado de las estaciones:")
        # Pasamos el contenido HTML de la web a un objeto BeautifulSoup()
        html = BeautifulSoup(req.text,"html.parser")

        possible_links = html.find_all('a')
        for link in possible_links:
            if link.has_attr('class') :
                #print link['class'][0]
                if link['class'][0].encode('utf8') == 'tablaestado':
                    teststring = unicode("U0001f6ab", 'ascii').encode('unicode_escape')

                    bot.sendMessage(chat_id, link.attrs['title'] + ": " + link.string + teststring)
                    #\u2705\U0001f6ab
                    
    # <a href="http://www.infonieve.es/estacion-esqui/la-pinilla/29/" class="tablaestado redondeado" style="background-color:#FA8574;" title="Estación de Esquí de La Pinilla">Cerrada</a>
    #<a class="tablaestado redondeado" href="http://www.infonieve.es/estacion-esqui/la-pinilla/29/" style="background-color:#FA8574;" title="Estaci├│n de Esqu├¡ de La Pinilla">Cerrada</a>

    else:
        print "Status Code %d" %statusCode
        bot.sendMessage(chat_id,"Información no disponible")


#TOKEN = sys.argv[1]  # get token from command-line
TOKEN = "315349142:AAF_OpQ_EqOlWJFqRftUk0qW-Jzumb3P--c"

bot = telepot.Bot(TOKEN)
bot.message_loop(handle)
print ('Listening ...')

# Keep the program running.
while 1:
    time.sleep(10)
