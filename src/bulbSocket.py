import socket
import re
import json
import struct
from urlparse import urlparse
import threading
import signal
import sys

class Multicast_Discovery:
	def __init__(self):
		self.MCAST_GRP = '239.255.255.250'
		self.MCAST_PORT = 1982
		self.bulbSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
		self.bulbSocket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 1)
		self.lightBulbProperties = {}
		self.msg = 'M-SEARCH * HTTP/1.1\r\nST:wifi_bulb\r\nMAN:"ssdp:discover"\r\n'
		self.bulbs = []
		self.bulbs_ips = set()
		self.parsed_url = ''
		self.max_age = 0

	def Multicast_Bulb_Discover(self,timeout=2):
		self.bulbSocket.settimeout(timeout)
		self.bulbSocket.sendto(self.msg.encode(), (self.MCAST_GRP,self.MCAST_PORT) )
		while(True):
			try:
				data, addr = self.bulbSocket.recvfrom(65507)
			except socket.timeout:
				break

			print(addr)
			#lightBulbProperties = dict([value.strip("\r").split(": ") for value in data.decode().split("\n") if":" in value])
		print(lightBulbProperties)
		try:
			self.bulbSocket.shutdown()
			self.bulbSocket.close()
		except:
			pass
sock = Multicast_Discovery()
sock.Multicast_Bulb_Discover()