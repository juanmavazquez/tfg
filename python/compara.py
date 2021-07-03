import sys

fichero = open("consigna.txt","r")

valoractual = int(sys.argv[1])
consigna = int(fichero.readline())

if valoractual < consigna: 
	print("on")
else:
	print("off")
