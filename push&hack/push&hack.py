import json
import urllib2
import telepot
import time
from random import randint

bot = telepot.Bot('----- TOKEN -------')
bot.getMe()

people = ['cole','paclema','reinaldo','julio']
comidas = ['sushi', 'pizza', 'hamburguesa', 'ensalada', 'kebab']

#data = json.load(urllib2.urlopen('https://api.bt.tn/2014-06/7782/counter'))
data = json.load(urllib2.urlopen('https://api.bt.tn/2014-06/7796/counter'))

old_data = data["counter"]

while True:
	#data = json.load(urllib2.urlopen('https://api.bt.tn/2014-06/7782/counter'))
	data = json.load(urllib2.urlopen('https://api.bt.tn/2014-06/7796/counter'))
	if data["counter"] > old_data :
		print "pulsado"
		print data["counter"]

		old_data = data["counter"]

		comida_enviada = 'Hoy toca para comer.... ' + comidas[randint(0,len(comidas)-1)] +  '!'
		persona_ganadora = 'Gana la comida preferida de ' + people[randint(0,len(people)-1)] + ' ;)'

		bot.sendMessage('6579123534524365234575(es el id del chat con alguien y el bot)', comida_enviada)
		bot.sendMessage('4885435234523450070', comida_enviada)
		bot.sendMessage('1199235624564231817', comida_enviada)

		bot.sendMessage('657934252345175', persona_ganadora)
		bot.sendMessage('488524523450070', persona_ganadora)
		bot.sendMessage('1199Q34534734631817', persona_ganadora)

	time.sleep(1)
