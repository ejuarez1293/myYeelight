import socket
import re
import json
import struct	
import threading
import signal
import sys
import fileinput
from collections import OrderedDict
import subprocess
from subprocess import *


devices = {}

def addDevices(properties):
	bulb_id = properties['id']
	if(bulb_id in devices):
		return
	properties.pop(bulb_id, None)
	devices[bulb_id]  = properties
	for key,value in devices[bulb_id].items():
		if("support" in key):
			devices[bulb_id][key] = list(filter(None, value.split(" ")))
			
def discover_bulbs():
	properties = OrderedDict()
	p = Popen(["./Discover_Bulbs"],stdout=PIPE)
	p.wait()
	for line in p.stdout:
		line = line.decode()
		line = line.strip("\r\n")
		if(":" in line):
			line = line.split(": ")
			try:
				properties.update({line[0]:int(line[1])})
			except:
				properties.update({line[0]:line[1]})
		
	addDevices(properties)
	return devices

bulbs = discover_bulbs()
print(bulbs)