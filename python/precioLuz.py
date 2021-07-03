from bs4 import BeautifulSoup
import lxml,urllib2

link = 'https://tarifaluzhora.es/'

url = urllib2.urlopen(link)
soup = BeautifulSoup(url,"lxml")
soup = str(soup)

comienzo = soup.find("['Horas', 'Precio']")+25
fin = soup.find("]);")

informacion = soup[comienzo:fin]

comienzoPrecio = 0

fichero = open("luz.txt","w")

for i in range(24):

	finPrecio = informacion.find("]",comienzoPrecio)
	
	precio = informacion[comienzoPrecio+15:finPrecio]
	
	precio = float(precio)*1000
	precio = str(precio)	

	fichero.write(precio) 
	fichero.write('\n')
	
	comienzoPrecio = finPrecio + 1
	

fichero.close()
