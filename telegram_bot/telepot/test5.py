import sys
import time
import telepot
import random

def handle(msg):
    chat_id = msg['chat']['id']
    command = msg['text']

    print msg
    print 'Got command: %s' % command

    if command == '/roll':
        bot.sendMessage(chat_id, random.randint(1,6))
    elif command == '/time':
        bot.sendMessage(chat_id, str(datetime.datetime.now()))

TOKEN = sys.argv[1]  # get token from command-line

bot = telepot.Bot(TOKEN)
bot.message_loop(handle)
print 'I am listening ...'

while 1:
    time.sleep(10)
