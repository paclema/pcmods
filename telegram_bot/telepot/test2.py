#Example using telepot.glace to parse the data

import sys
import time
import telepot

def handle(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)
    print(content_type, chat_type, chat_id)

    if content_type == 'text':
        bot.sendMessage(chat_id, msg)
    if content_type == 'new_chat_member':
        bot.sendMessage(chat_id, "wellcome to this awesome group!")
    if content_type == 'left_chat_member':
        bot.sendMessage(chat_id, "Sayonara baby")

TOKEN = sys.argv[1]  # get token from command-line

bot = telepot.Bot(TOKEN)
bot.message_loop(handle)
print ('Listening ...')

# Keep the program running.
while 1:
    time.sleep(10)
