import sys
import time
import random
import telepot
from telepot.namedtuple import InlineKeyboardMarkup, InlineKeyboardButton

def on_chat_message(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)

    keyboard = InlineKeyboardMarkup(inline_keyboard=[
                   [InlineKeyboardButton(text='Press me', callback_data='press'),
                   InlineKeyboardButton(text='Dont press me', callback_data='no press')]
               ])
    answer_list = ["yeah!", "cool!", "nice ;)", "Awesooooome!"]
    bot.sendMessage(chat_id, random.choice(answer_list), reply_markup=keyboard)

def on_callback_query(msg):
    query_id, from_id, query_data = telepot.glance(msg, flavor='callback_query')
    print('Callback Query:', query_id, from_id, query_data)

    if query_data == 'press': bot.answerCallbackQuery(query_id, text='Got it')
    elif query_data == 'no press': bot.answerCallbackQuery(query_id, text='Not got it')

TOKEN = sys.argv[1]  # get token from command-line

bot = telepot.Bot(TOKEN)
bot.message_loop({'chat': on_chat_message,
                  'callback_query': on_callback_query})
print('Listening ...')

while 1:
    time.sleep(10)
