
from gtts import gTTS
tts = gTTS(text = 'Hola, bienvenidos al taller', lang = 'es')
tts.save("hola.mp3")
