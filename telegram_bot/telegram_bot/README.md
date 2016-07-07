#FlagBot #

This is a bot to move a (physical) flag with a servo.

Work-in-progress bot from BQ Innovation Labs.

## Requisites ##
This requires the [twx-bot API](https://github.com/datamachine/twx.botapi). To install the stable version:

    $ sudo pip install twx.botapi

## Hardware ##
FlagBot requires:

 * A servo to move the flag
 * A board to control the servo (e.g. Arduino Uno)

The board and the servo are connected as follows:
<p align="center">
<img src="doc/Circuit.png" width="70%">
</p>  

The firmware for the Arduino board is provided, and can be found in the FlagBot/Arduino folder of this repository.

In order to hold the Arduino board and the servo together, a 3D printable holder can be found in the FlagBot/Hardware folder:
<p align="center">
<img src="doc/Holder.png" width="70%">
</p>  

This is the final result:
<p align="center">
<img src="doc/Assembly.jpg" width="70%">
</p>  

<p align="center">
<img src="doc/Assembly-2.jpg" width="70%">
</p>  
## Configuration ##
For the bot to work properly, the configuration file provided has to be modified with your bot's token and user
id.

To get a token check out the Official Telegram Bot API [documentation](https://core.telegram.org/bots/api).

To get the user id, open a chat with FlagBot and issue the command /start . After that, your id will appear on the
bot stdout.

## Usage ##
Just run the python script:

    $ python FlagBot.py

To lift the flag, send the following text to the Telegram bot in a Telegram chat:

    /flag up

To put the flag down again, send this command via Telegram:

    /flag down

Dependencies :

	sudo apt-get install python-pygame

## Windows install ##

  install python 2.7.11
  pip install twx.botapi   https://pypi.python.org/pypi/twx.botapi
  pip install pygame o http://www.pygame.org/download.shtml
