/*
 * CVSReader.cpp
 *
 *  Created on: 8 de nov. de 2015
 *      Author: joha
 */

#include "CSVReader.h"

using namespace std;

CSVReader::CSVReader() {

}

void CSVReader::open(char *nameFile,int &cantidadDeRowsTrain,map<string,float> &frecuenciaCrimenes,
		map<string,int> &crimenesPorDistrito,map<string,float> &probabilidadesCrimenes,
		map<string,float> probabilidadesDias[7],map<string,float> probabilidadesDistritos[10],
		map<string,float> probabilidadesHoras[24],map<string,float> probabilidadesMes[12],
		map<string,int> &crimenesPorMes){

	string line, csvItem, crimen, horaActual, mesActual;
	ifstream myfile(nameFile);
	int nroItem = 0;

	map<string,int> horas;

	map<string,float> lunes;
	map<string,float> martes;
	map<string,float> miercoles;
	map<string,float> jueves;
	map<string,float> viernes;
	map<string,float> sabado;
	map<string,float> domingo;

	map<string,float> enero;
	map<string,float> febrero;
	map<string,float> marzo;
	map<string,float> abril;
	map<string,float> mayo;
	map<string,float> junio;
	map<string,float> julio;
	map<string,float> agosto;
	map<string,float> septiembre;
	map<string,float> octubre;
	map<string,float> noviembre;
	map<string,float> diciembre;

	map<string,float> bayview;
	map<string,float> central;
	map<string,float> ingleside;
	map<string,float> mission;
	map<string,float> northern;
	map<string,float> park;
	map<string,float> richmond;
	map<string,float> southern;
	map<string,float> taraval;
	map<string,float> tenderloin;

	if (myfile.is_open()) {

		while (getline(myfile,line)) {
			nroItem = 0;
			cantidadDeRowsTrain++;
			istringstream myline(line);

			while(getline(myline, csvItem, ',')) {

				if (nroItem == 0) mesActual = csvItem;

				if (nroItem == 1) horaActual = csvItem;

				if (nroItem == 2){
					crimen = csvItem;
					if (frecuenciaCrimenes.count(csvItem) < 1) frecuenciaCrimenes[csvItem] = 1; else
						frecuenciaCrimenes[csvItem] += 1;
					this->calcularCrimenesPorHora(horaActual,csvItem,probabilidadesHoras,horas);
					this->calcularCrimenesPorMes(mesActual,csvItem,enero,febrero,marzo,abril,mayo,junio,julio,agosto,septiembre,
						octubre,noviembre,diciembre,crimenesPorMes);
				}

				if (nroItem == 3) this->calcularCrimenesPorDia(csvItem,crimen,lunes,martes,miercoles,jueves,viernes,sabado,domingo);

				if (nroItem == 4) this->calcularCrimenesPorDistrito(crimenesPorDistrito,csvItem,bayview,central,ingleside,mission,
								northern,park,richmond,southern,taraval,tenderloin,crimen);
				nroItem++;
			}

		}

	}

	myfile.close();

	//se calculan las probabilidades de cada clase
	this->calcularProbabilidadesDeLosCrimenes(frecuenciaCrimenes,probabilidadesCrimenes,cantidadDeRowsTrain);

	//esto lo que hace es calcular las probabilidades de los crimenes por dia y distrito, y almacenarlos en un vector
	//para luego poder acceder a estos datos calculados
	this->calcularProbabilidadesDeCrimenesPorDia(lunes,martes,miercoles,jueves,viernes,sabado,domingo,frecuenciaCrimenes);
	this->calcularProbabilidadesDeCrimenesPorDistrito(bayview,central,ingleside,mission,northern,park,richmond,southern,taraval,
			tenderloin,frecuenciaCrimenes);
    this->calcularProbabilidadesDeCrimenesPorHora(probabilidadesHoras,frecuenciaCrimenes);
    this->calcularProbabilidadesDeLosCrimenesPorMes(enero,febrero,marzo,abril,mayo,junio,julio,agosto,
    		septiembre,octubre,noviembre,diciembre,frecuenciaCrimenes);

	probabilidadesDias[0] = lunes;
	probabilidadesDias[1] = martes;
	probabilidadesDias[2] = miercoles;
	probabilidadesDias[3] = jueves;
	probabilidadesDias[4] = viernes;
	probabilidadesDias[5] = sabado;
	probabilidadesDias[6] = domingo;

	probabilidadesDistritos[0] = bayview;
	probabilidadesDistritos[1] = central;
	probabilidadesDistritos[2] = ingleside;
	probabilidadesDistritos[3] = mission;
	probabilidadesDistritos[4] = northern;
	probabilidadesDistritos[5] = park;
	probabilidadesDistritos[6] = richmond;
	probabilidadesDistritos[7] = southern;
	probabilidadesDistritos[8] = taraval;
	probabilidadesDistritos[9] = tenderloin;

	probabilidadesMes[0] = enero;
	probabilidadesMes[1] = febrero;
	probabilidadesMes[2] = marzo;
	probabilidadesMes[3] = abril;
	probabilidadesMes[4] = mayo;
	probabilidadesMes[5] = junio;
	probabilidadesMes[6] = julio;
	probabilidadesMes[7] = agosto;
	probabilidadesMes[8] = septiembre;
	probabilidadesMes[9] = octubre;
	probabilidadesMes[10] = noviembre;
	probabilidadesMes[11] = diciembre;

	cout << "Se calcularon correctamente las probabilidades necesarias" << endl;

}

void CSVReader::calcularCrimenesPorMes(string &mesActual,string &crimen,map<string,float> &enero, map<string,float> &febrero,
		map<string,float> &marzo,map<string,float> &abril,map<string,float> &mayo,map<string,float> &junio,
		map<string,float> &julio,map<string,float> &agosto,map<string,float> &septiembre,
		map<string,float> &octubre,map<string,float> &noviembre,map<string,float> &diciembre,map<string,int> &crimenesPorMes){

	if (crimenesPorMes.count(mesActual) < 1) crimenesPorMes[mesActual] = 1; else
		crimenesPorMes[mesActual] += 1;
	if (mesActual == "01"){
		if (enero.count(crimen) < 1) enero[crimen] = 1; else enero[crimen] += 1;
	} else if (mesActual == "02"){
		if (febrero.count(crimen) < 1) febrero[crimen] = 1; else febrero[crimen] += 1;
	} else if (mesActual == "03"){
		if (marzo.count(crimen) < 1) marzo[crimen] = 1; else marzo[crimen] += 1;
	} else if (mesActual == "04"){
		if (abril.count(crimen) < 1) abril[crimen] = 1; else abril[crimen] += 1;
	} else if (mesActual == "05"){
		if (mayo.count(crimen) < 1) mayo[crimen] = 1; else mayo[crimen] += 1;
	} else if (mesActual == "06"){
		if (junio.count(crimen) < 1) junio[crimen] = 1; else junio[crimen] += 1;
	} else if (mesActual == "07"){
		if (julio.count(crimen) < 1) julio[crimen] = 1; else julio[crimen] += 1;
	} else if (mesActual == "08"){
		if (agosto.count(crimen) < 1) agosto[crimen] = 1; else agosto[crimen] += 1;
	} else if (mesActual == "09"){
		if (septiembre.count(crimen) < 1) septiembre[crimen] = 1; else septiembre[crimen] += 1;
	} else if (mesActual == "10"){
		if (octubre.count(crimen) < 1) octubre[crimen] = 1; else octubre[crimen] += 1;
	} else if (mesActual == "11"){
		if (noviembre.count(crimen) < 1) noviembre[crimen] = 1; else noviembre[crimen] += 1;
	} else if (mesActual == "12"){
		if (diciembre.count(crimen) < 1) diciembre[crimen] = 1; else diciembre[crimen] += 1;
	}

}

void CSVReader::calcularProbabilidadesDeLosCrimenesPorMes(map<string,float> &enero,
		map<string,float> &febrero,map<string,float> &marzo,map<string,float> &abril,map<string,float> &mayo,
		map<string,float> &junio,map<string,float> &julio,map<string,float> &agosto,map<string,float> &septiembre,
		map<string,float> &octubre,map<string,float> &noviembre,map<string,float> &diciembre,
		map<string,float> frecuenciaCrimenes){

	this->calcularProbabilidad(enero,frecuenciaCrimenes);
	this->calcularProbabilidad(febrero,frecuenciaCrimenes);
	this->calcularProbabilidad(marzo,frecuenciaCrimenes);
	this->calcularProbabilidad(abril,frecuenciaCrimenes);
	this->calcularProbabilidad(mayo,frecuenciaCrimenes);
	this->calcularProbabilidad(junio,frecuenciaCrimenes);
	this->calcularProbabilidad(julio,frecuenciaCrimenes);
	this->calcularProbabilidad(agosto,frecuenciaCrimenes);
	this->calcularProbabilidad(septiembre,frecuenciaCrimenes);
	this->calcularProbabilidad(octubre,frecuenciaCrimenes);
	this->calcularProbabilidad(noviembre,frecuenciaCrimenes);
	this->calcularProbabilidad(diciembre,frecuenciaCrimenes);

}

void CSVReader::calcularProbabilidadesDeLosCrimenes(map<string,float> frecuenciaCrimenes,map<string,float> &probabilidadesCrimenes,
		int cantidadDeRowsTrain){

	probabilidadesCrimenes = frecuenciaCrimenes;
	map<string,float>::iterator iter = probabilidadesCrimenes.begin();
	while (iter != probabilidadesCrimenes.end() ){
		iter->second = (iter->second/cantidadDeRowsTrain);
		iter++;
	}
}

void CSVReader::calcularProbabilidad(map<string,float> &nombreMap,map<string,float> frecuenciaCrimenes){

	map<string,float>::iterator iter = nombreMap.begin();
	while (iter != nombreMap.end() ){
		//cout << iter->first + " " <<  iter->second << frecuenciaCrimenes[iter->first] <<endl;
		iter->second = (iter->second/frecuenciaCrimenes[iter->first]);
		//cout << iter->first + " " <<  iter->second << endl;
		iter++;
	}
}

void CSVReader::calcularProbabilidadesDeCrimenesPorHora(map<string,float> crimenesPorHora[24],
	map<string,float>frecuenciaCrimenes){

	int numero = 0;
	while(numero < 24){
		calcularProbabilidad(crimenesPorHora[numero], frecuenciaCrimenes);
		numero++;
	}
}

void CSVReader::imprimirCrimenesPorHora(map<string,float>crimenesPorHora[24]){

	int numero = 0;
	long int cantidadDeDelitosTotales = 0;
	long int cantidadDeDelitosPorHora;
	map<string, float>::iterator iter; //= probabilidades[numero].begin();
	while(numero <24){
		iter = crimenesPorHora[numero].begin();
		cout<< "hora " << numero<<endl;
		cantidadDeDelitosPorHora = 0;
		while (iter != crimenesPorHora[numero].end() ){
			      cout << iter->first + " " <<  iter->second << endl;
			      //cantidadDeDelitosPorHora= cantidadDeDelitosPorHora + iter->second;
			      iter++;
		}
		//cout << cantidadDeDelitosPorHora << endl;
		//cantidadDeDelitosTotales= cantidadDeDelitosTotales + cantidadDeDelitosPorHora;
		numero++;
	}
	cout << cantidadDeDelitosTotales << endl;
}

void CSVReader::calcularProbabilidadesDeCrimenesPorDia(map<string,float> &lunes,map<string,float> &martes,
		map<string,float> &miercoles,map<string,float> &jueves,map<string,float> &viernes,map<string,float> &sabado,
		map<string,float> &domingo,map<string,float> frecuenciaCrimenes){

	this->calcularProbabilidad(lunes,frecuenciaCrimenes);
	this->calcularProbabilidad(martes,frecuenciaCrimenes);
	this->calcularProbabilidad(miercoles,frecuenciaCrimenes);
	this->calcularProbabilidad(jueves,frecuenciaCrimenes);
	this->calcularProbabilidad(viernes,frecuenciaCrimenes);
	this->calcularProbabilidad(sabado,frecuenciaCrimenes);
	this->calcularProbabilidad(domingo,frecuenciaCrimenes);

}

void CSVReader::calcularProbabilidadesDeCrimenesPorDistrito(map<string,float> &bayview,map<string,float> &central,
		map<string,float> &ingleside,map<string,float> &mission,map<string,float> &northern,map<string,float> &park,
		map<string,float> &richmond,map<string,float> &southern,map<string,float> &taraval,
		map<string,float> &tenderloin,map<string,float> frecuenciaCrimenes){

	this->calcularProbabilidad(bayview,frecuenciaCrimenes);
	this->calcularProbabilidad(central,frecuenciaCrimenes);
	this->calcularProbabilidad(ingleside,frecuenciaCrimenes);
	this->calcularProbabilidad(mission,frecuenciaCrimenes);
	this->calcularProbabilidad(northern,frecuenciaCrimenes);
	this->calcularProbabilidad(park,frecuenciaCrimenes);
	this->calcularProbabilidad(richmond,frecuenciaCrimenes);
	this->calcularProbabilidad(southern,frecuenciaCrimenes);
	this->calcularProbabilidad(taraval,frecuenciaCrimenes);
	this->calcularProbabilidad(tenderloin,frecuenciaCrimenes);

}

void CSVReader::calcularCrimenesPorHora(string horaActual,string delitoActual,map<string,float> crimenesPorHora[24],map<string,int> &horas){

	if (horas.count(horaActual) < 1){
		if (crimenesPorHora[atoi(horaActual.c_str())].count(delitoActual)<1){
			crimenesPorHora[atoi(horaActual.c_str())][delitoActual] = 1;
		}
		else crimenesPorHora[atoi(horaActual.c_str())][delitoActual] += 1;
	}
	else {
		horas[horaActual] = atoi(horaActual.c_str());
		crimenesPorHora[atoi(horaActual.c_str())][delitoActual] = 1;
	}
}

void CSVReader::calcularCrimenesPorDia(string &csvItem,string &crimen,map<string,float> &lunes,map<string,float> &martes,map<string,float> &miercoles,
						map<string,float> &jueves,map<string,float> &viernes,map<string,float> &sabado,map<string,float> &domingo){

	if (csvItem == "Monday"){
		if (lunes.count(crimen) < 1) lunes[crimen] = 1; else lunes[crimen] += 1;
	} else if (csvItem == "Tuesday"){
		if (martes.count(crimen) < 1) martes[crimen] = 1; else martes[crimen] += 1;
	} else if (csvItem == "Wednesday"){
		if (miercoles.count(crimen) < 1) miercoles[crimen] = 1; else miercoles[crimen] += 1;
	} else if (csvItem == "Thursday"){
		if (jueves.count(crimen) < 1) jueves[crimen] = 1; else jueves[crimen] += 1;
	} else if (csvItem == "Friday"){
		if (viernes.count(crimen) < 1) viernes[crimen] = 1; else viernes[crimen] += 1;
	} else if (csvItem == "Saturday"){
		if (sabado.count(crimen) < 1) sabado[crimen] = 1; else sabado[crimen] += 1;
	} else if (csvItem == "Sunday"){
		if (domingo.count(crimen) < 1) domingo[crimen] = 1; else domingo[crimen] += 1;
	}

}

void CSVReader::calcularCrimenesPorDistrito(map<string,int> &crimenesPorDistrito,string &csvItem,map<string,float> &bayview,
										map<string,float> &central,map<string,float> &ingleside,map<string,float> &mission,
										map<string,float> &northern,map<string,float> &park,map<string,float> &richmond,
										map<string,float> &southern,map<string,float> &taraval,map<string,float> &tenderloin,
										string &crimen){

	if (crimenesPorDistrito.count(csvItem) < 1) crimenesPorDistrito[csvItem] = 1; else
		crimenesPorDistrito[csvItem] += 1;
	if (csvItem == "BAYVIEW"){
		if (bayview.count(crimen) < 1) bayview[crimen] = 1; else bayview[crimen] += 1;
	} else if (csvItem == "CENTRAL"){
		if (central.count(crimen) < 1) central[crimen] = 1; else central[crimen] += 1;
	} else if (csvItem == "INGLESIDE"){
		if (ingleside.count(crimen) < 1) ingleside[crimen] = 1; else ingleside[crimen] += 1;
	} else if (csvItem == "MISSION"){
		if (mission.count(crimen) < 1) mission[crimen] = 1; else mission[crimen] += 1;
	} else if (csvItem == "NORTHERN"){
		if (northern.count(crimen) < 1) northern[crimen] = 1; else northern[crimen] += 1;
	} else if (csvItem == "PARK"){
		if (park.count(crimen) < 1) park[crimen] = 1; else park[crimen] += 1;
	} else if (csvItem == "RICHMOND"){
		if (richmond.count(crimen) < 1) richmond[crimen] = 1; else richmond[crimen] += 1;
	} else if (csvItem == "SOUTHERN"){
		if (southern.count(crimen) < 1) southern[crimen] = 1; else southern[crimen] += 1;
	} else if (csvItem == "TARAVAL"){
		if (taraval.count(crimen) < 1) taraval[crimen] = 1; else taraval[crimen] += 1;
	} else if (csvItem == "TENDERLOIN"){
		if (tenderloin.count(crimen) < 1) tenderloin[crimen] = 1; else tenderloin[crimen] += 1;
	}
}

CSVReader::~CSVReader() {

}

