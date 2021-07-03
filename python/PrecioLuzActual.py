import array
import time 

arrayPrecios = array.array('f')
hora = int(time.strftime("%H"))


fprecios = open("luz.txt","r")

while(True):

	precio = fprecios.readline()
	print(str(precio))
	if not precio:
		break
	
	precio = float(precio)
	arrayPrecios.append(precio)

fprecios.close()

print(str(arrayPrecios))

fparametros = open("parametrosLuz.txt","w")

fparametros.write("Caro: ")
fparametros.write(str(max(arrayPrecios)))
print(str(max(arrayPrecios)))
fparametros.write("eu/MWh\n")
fparametros.write("Barato: ")
fparametros.write(str(min(arrayPrecios)))
fparametros.write("eu/MWh\n")
fparametros.write("Actual: ")
fparametros.write(str(arrayPrecios[hora]))
fparametros.write("eu/MWh  (")
fparametros.write(str(hora))
fparametros.write(":00-")
fparametros.write(str(hora+1))
fparametros.write(":00)")

fparametros.close()
