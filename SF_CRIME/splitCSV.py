#!/usr/bin/python
import csv

#constantes globales
cantParcelasAncho = 5
cantParcelasAlto = 5
minLong = 37.6  #37.71
maxLong = 37.82
maxLat = -122.34
minLat = -122.53


def getParcelas(nameFile):
	
	archivo_input = open(nameFile)
	archivo_input_csv = csv.reader (archivo_input, delimiter = ",")
	archivo_input_csv.next()
	firstRow = archivo_input_csv.next()
	
	coordinates= {"Xmin": float(firstRow[7]),"Xmax":float(firstRow[7]),"Ymin":float(firstRow[8]),"Ymax":float(firstRow[8])}
	
	for date, category, descript, dayOfWeek, district, resolution, \
	address, x, y  in archivo_input_csv:
		getMaxAndMinCoord(coordinates,float(x),float(y))
	
	print coordinates
	archivo_input.close()
	parcelas= generararParcelas(coordinates)
	
	return coordinates, parcelas

def generararParcelas(coordinates):
	
	b =  abs(coordinates["Xmax"]) - abs(coordinates["Xmin"])
	h =  abs(coordinates["Ymax"]) - abs(coordinates["Ymin"])
	sup = b*h
	
	anchoParcela = abs(b / cantParcelasAncho)
	altoParcela = abs(h / cantParcelasAlto)
	
	print anchoParcela, altoParcela
	
	Xi= coordinates["Xmin"]
	Yi= coordinates["Ymin"]
	parcelas = {}
	
	for fila in xrange (cantParcelasAlto):
		Xi= coordinates["Xmin"]
		Ydesde = Yi
		Yhasta = Ydesde + altoParcela
		
		for i in xrange (cantParcelasAncho):
			Xdesde = Xi
			Xhasta = Xdesde + anchoParcela
			numParcela =  fila*cantParcelasAlto + i 
			
			if (i == (cantParcelasAncho-1)):
				Xhasta = coordinates["Xmax"]
				
			if (fila == (cantParcelasAlto-1)):
				Yhasta = coordinates["Ymax"]
				
			parcelas[numParcela] = [ Xdesde, Xhasta, Ydesde, Yhasta ] 
			Xi = Xhasta
			
		Yi= Yhasta
	
	
	return parcelas
	
def clasificarPorParcela(coordinates,parcelas, x, y):
	""" parcelas = {numeroDeParcel: [Xdesde, Xhasta, Ydesde, Yhasta]} """
	
	for key in parcelas:
		if (parcelas[key][0] <= x) & (parcelas[key][1] >= x) & (parcelas[key][2] <= y) & (parcelas[key][3] >= y) :
			return key
	
	return "Sin parcela"
	 

def perteneceAParcela( numParcela, x,y ):
	return ( (numParcela[0] <= x) & (numParcela[1] >= x) & (numParcela[2] <= y) & (numParcela[3] >= y))

def getMaxAndMinCoord(coordinates, x,y):
	if ((x < coordinates["Xmin"]) & (x > minLat)):
		coordinates["Xmin"] = x

	if ((x > coordinates["Xmax"]) & (x < maxLat) ):
		coordinates["Xmax"] = x
		
	if ((y < coordinates["Ymin"]) & (y > minLong)):
		coordinates["Ymin"] = y

	if ((y > coordinates["Ymax"]) & (y < maxLong)):
		coordinates["Ymax"] = y
			

def splitDate(stringDate):
	
	dateTime = stringDate.split(" ")
	date = dateTime[0].split("-")
	month= date[1]
	
	timeString= dateTime[1].split(":")
	hour= timeString[0]
	return (month,hour)

def splitTest(nameFile, outputFile,coordinates, parcelas):

	archivo_input = open(nameFile)
	archivo_input_csv = csv.reader (archivo_input, delimiter = ",")
	archivo_input_csv.next()
	
	registros = []
	
	for ids, date, dayOfWeek, district, address, x, y  \
	in archivo_input_csv:
		
		(month, hour) = splitDate(date)
		parcela = clasificarPorParcela(coordinates,parcelas,float(x), float(y))
		registro = (ids,month, hour, dayOfWeek, district, x, y, parcela)
		registros.append(registro)
	
	archivo_input.close()
	archivo_output = open (outputFile,"w")
	archivo_output_csv = csv.writer(archivo_output)
	archivo_output_csv.writerows(registros)
	archivo_output.close()
	
def splitTrain(nameFile, outPutFile, coordinates, parcelas):
			
	for key, value in parcelas.iteritems():
		print key,value
		
		
	archivo_input = open(nameFile)
	archivo_input_csv = csv.reader (archivo_input, delimiter = ",")
	archivo_input_csv.next()
	
	registros= []
	
	for date, category, descript, dayOfWeek, district, resolution, \
	address, x, y  in archivo_input_csv:
		(month, hour) = splitDate(date)
		parcela = clasificarPorParcela(coordinates, parcelas,float(x), float(y))
		registro = (month, hour,category,dayOfWeek, district,x,y, parcela)
		registros.append(registro)
		
	
	archivo_input.close()
	archivo_output = open (outPutFile,"w")
	archivo_output_csv = csv.writer(archivo_output)
	archivo_output_csv.writerows(registros)
	archivo_output.close()

def main():
	trainFile = "train.csv"
	outputTrain  = "trainSpliteado.csv"
	
	testFile  = "test.csv"
	outputTest = "testSpliteado.csv"
	
	# Notar que se dividen parcelas de acuerdo a los datos del train, si los max y min del test son distintos no los clasifica!
	#cerca de 70 puntos no se clasifican
	coordinates, parcelas = getParcelas(trainFile)
	splitTrain(trainFile, outputTrain, coordinates, parcelas)
	splitTest(testFile, outputTest, coordinates, parcelas)


main()
