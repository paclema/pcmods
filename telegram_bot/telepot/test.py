#More info on: http://telepot.readthedocs.io/en/latest/

#Get info of the bot
import telepot
bot = telepot.Bot('118628546:AAFp9vO2A271Gp4kx-6OQCrNUHucFaoeXg8')
print bot.getMe()

#Print the last message received from the bot
from pprint import pprint
response = bot.getUpdates()
pprint(response)

#Print with offset
bot.getUpdates(offset=100000001)

#Print only every new message
def handle(msg):
    pprint(msg)
bot.message_loop(handle)

 #Send a message to the id user 999999999
bot.sendMessage(999999999, 'Hey!')
