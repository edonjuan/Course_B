import mraa, time, upm
from upm import pyupm_jhd1313m1


ldr = mraa.Aio(0)
light = 0

display = pyupm_jhd1313m1.Jhd1313m1(0, 0x3e ,0x62)
display.setColor(255,0,255)
display.setCursor(0,0)

while True:
	light = ldr.read()
	
	if(light>400):
		display.setCursor(0,0)
		display.setColor(255,0,0)
		display.write("LUZ DE DIA      ")

	else:
		display.setCursor(0,0)
		display.setColor(0,0,255)
		display.write("LUZ DE NOCHE    ")
	
	display.setCursor(1,0)
	display.write("Luxes: " +str(light))
	time.sleep(1)
