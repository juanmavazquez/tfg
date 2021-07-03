from bs4 import BeautifulSoup
import lxml,urllib2

fichero=open("ciudad.txt","r")
ciudad=fichero.read()
fichero.close()

x=str()

for i in ciudad:
	if i.isspace():
		x+='-'
	else:
		x+=i

link='https://www.eltiempo.es/'+x+'.html'

url=urllib2.urlopen(link)
soup=BeautifulSoup(url,"lxml")

headlist=soup.head.contents
headlist=str(headlist)

numero=headlist.find("precip_amount")+16

msg=headlist[numero:numero+4]

if msg=="none":
	print("NO")
else:
	print("SI")
