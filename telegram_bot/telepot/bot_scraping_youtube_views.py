# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
import requests

import sys
import time
import telepot

#url = "http://www.youtube.com/watch?v=QtXby3twMmI"

numero_view = 0

def handle(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)
    print(content_type, chat_type, chat_id)

    if content_type == 'text':
        #bot.sendMessage(chat_id, msg)
        bot.sendMessage(6579175, numero_view)
    if content_type == 'new_chat_member':
        bot.sendMessage(chat_id, "wellcome to this awesome group!")
    if content_type == 'left_chat_member':
        bot.sendMessage(chat_id, "Sayonara baby")

TOKEN = sys.argv[1]  # get token from command-line
url_video = sys.argv[2]  # get token from command-line

bot = telepot.Bot(TOKEN)
bot.message_loop(handle)
print ('Listening ...')

# Keep the program running.
while 1:
    req = requests.get(url_video)
    statusCode = req.status_code

    if statusCode == 200:
        print "Dentro del parser"
        html = BeautifulSoup(req.text,"html.parser")
        views = html.find_all('div',{'class':'watch-view-count'})

        for i,view in enumerate(views):
            # Con el método "getText()" no nos devuelve el HTML
            numero_view = view.getText()
    else:
        print "Status Code %d" %statusCode

    bot.sendMessage(6579175, numero_view)
    time.sleep(1)
    #break
