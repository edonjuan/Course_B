import mraa, time

relay = mraa.Gpio(5)
relay.dir(0)

button = mraa.Gpio(7)
button.dir(1)
switch = 0;

pir = mraa.Gpio(2)
pir.dir(1)
mov = 0

cont = 0

while True:

	switch = button.read()
	mov = pir.read()

	if(mov):
		relay.write(1)
		cont = cont + 1
		print "Se han detectado " + str(cont) +  " movimientos"
		time.sleep(3)
		relay.write(0)

	if(switch):
		cont = 0
		print "Se ha re-iniciado el contador a 0"
		time.sleep(1)
