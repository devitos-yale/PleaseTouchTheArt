import socket
from pythonosc import udp_client
from pydub import AudioSegment
import pygame

pygame.init()
pygame.mixer.init()

#audio segment
#filename = 'input.wav'
#sound = AudioSegment.from_file(filename, format="wav")

# UDP_IP = "172.29.129.22"
UDP_IP = "172.29.35.120"
UDP_PORT = 8092

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

sc_client = udp_client.SimpleUDPClient("127.0.0.1", 57120) # Default ip and port for SC
#sc_client.send_message('/print', value)

n = 0 #packet data
sensor = 0 #which sensor is being read
pressed1 = 0
pressed2 = 0
pressed3 = 0
pressed4 = 0

while True:

	value = 3
	#sc_client.send_message("/print", value)

	data, addr = sock.recvfrom(1024)
	n = data.decode("ASCII")
	#print("Message: ", n)
	#print("sensor: ", sensor)

	if (int(n) == 5000): #start key
    	#print('KEY');
		sensor = 1

	elif (sensor == 1): #BUTTON 1
		if (int(n) == 0):
			pressed1 = 1
		else:
			pressed1 = 0
		sensor = 2

	elif (sensor == 2): #BUTTON 2
		if (int(n) == 0):
			pressed2 = 1
		else:
			pressed2 = 0
		sensor = 3

	elif (sensor == 3): #BUTTON 3
		if (int(n) == 0):
			pressed3 = 1
		else:
			pressed3 = 0
		sensor = 4

	elif (sensor == 4): #BUTTON 4
		if (int(n) == 0):
			pressed4 = 1
		else:
			pressed4 = 0
		sensor = 5

	elif (sensor == 5): #PHOTORESISTOR 1
		print('one')
		print(n)
		if pressed1 == 1 or pressed4 == 1:
			if (int(n) == 0):
				sc_client.send_message("/p1", 100)
			else:
				sc_client.send_message("/p1", n)
		else:
			sc_client.send_message("/p1", 0)
		sensor = 6
		
	elif (sensor == 6): #PHOTORESISTOR 2
		print('two')
		print(n)
		if pressed3 == 1:
			if (int(n) == 0):
				sc_client.send_message("/p2", 100)
			else:
				sc_client.send_message("/p2", n)
		else:
			sc_client.send_message("/p2", 0)
		sensor = 7
	elif (sensor == 7): #PHOTORESISTOR 3
		print('three')
		print(n)
		if pressed2 == 1:
			if (int(n) == 0):
				sc_client.send_message("/p3", 100)
			else:
				sc_client.send_message("/p3", n)
		else:
			sc_client.send_message("/p3", 0)
		sensor = 0
