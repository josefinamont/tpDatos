/*
 * Bayes.cpp
 *
 *  Created on: 17/11/2015
 *      Author: josefina
 */

#include "Bayes.h"

using namespace std;

Bayes::Bayes() {

	this->probabilidadesPorDias = 0;
	this->probabilidadesPorDistrito = 0;
	this->probabilidadesPorHoras = 0;
	this->probabilidadesPorMes = 0;
	this->probabilidadesPorCoordenadas = 0;
}

void Bayes::train(map<string,float> frecuenciaDeCrimenes,map<string,float> probabilidadesDeCrimenes,
		map<string,int> frecuenciaDeCrimenesPorDistrito,map<string,float> *probabilidadesPorDias,
		map<string,float> *probabilidadesPorDistrito,map<string,float> *probabilidadesPorHoras,
		map<string,float> *probabilidadesPorMes,map<string,float> *probabilidadesPorCoordenadas){

	this->frecuenciaDeCrimenes = frecuenciaDeCrimenes;
	this->probabilidadesDeCrimenes = probabilidadesDeCrimenes;
	this->frecuenciaDeCrimenesPorDistrito = frecuenciaDeCrimenesPorDistrito;
	this->probabilidadesPorDias = probabilidadesPorDias;
	this->probabilidadesPorDistrito = probabilidadesPorDistrito;
	this->probabilidadesPorHoras = probabilidadesPorHoras;
	this->probabilidadesPorMes = probabilidadesPorMes;
	this->probabilidadesPorCoordenadas = probabilidadesPorCoordenadas;

	cout << "Se entrenó correctamente el clasificador" << endl;
}

void Bayes::predecir(char* nameFile){

	string line, csvItem, id;
	ifstream myfile(nameFile);
	int nroItem = 0;
	Row row;
	float probabilidades[39];

	ofstream predicciones("predicciones.csv");

	predicciones << "Id,";

	map<string,float>::iterator iter = frecuenciaDeCrimenes.begin();
	while (iter != frecuenciaDeCrimenes.end() ){
		if (iter->first != "WEAPON LAWS") {
			predicciones << iter->first << ",";
			iter++;
		} else {
			predicciones << iter->first;
			iter++;
		}

	}

	predicciones << "\n";

	if (myfile.is_open()) {

		while (getline(myfile,line)) {
			nroItem = 0;
			istringstream myline(line);

			while(getline(myline, csvItem, ',')) {

				if (nroItem == 0) id = csvItem;

				if (nroItem == 1) row.mes = csvItem; else
					if (nroItem == 2) row.hora = csvItem; else
						if (nroItem == 3) row.diaDeLaSemana = csvItem; else
							if (nroItem == 4) row.distrito = csvItem; else
								if(nroItem == 7) row.Parcela = csvItem;

				nroItem++;
			}

			this->predecirRow(row,probabilidades);

			predicciones << id;
			predicciones << ",";

			for (int i = 0; i < 39; i++) {

				if (i == 38) predicciones << probabilidades[i]; else {
					predicciones << probabilidades[i];
					predicciones << ",";
				}
			}
			predicciones << "\n";

		}

	}

	myfile.close();
	predicciones.close();

	cout << "Se predijeron todos los casos del set de testing";

}

int Bayes::calcularDistritoCorrespondiente(string distrito){

	if (distrito == "BAYVIEW") return 0; else
		if (distrito == "CENTRAL") return 1; else
			if (distrito == "INGLESIDE") return 2; else
				if (distrito == "MISSION") return 3; else
					if (distrito == "NORTHERN") return 4; else
						if (distrito == "PARK") return 5; else
							if (distrito == "RICHMOND") return 6; else
								if (distrito == "SOUTHERN") return 7; else
									if (distrito == "TARAVAL") return 8; else
										if (distrito == "TENDERLOIN") return 9; else
											cout << "Distrito no válido" << endl;
}

int Bayes::calcularDiaCorrespondiente(string dia){

	if (dia == "Monday") return 0; else
		if (dia == "Tuesday") return 1; else
			if (dia == "Wednesday") return 2; else
				if (dia == "Thursday") return 3; else
					if (dia == "Friday") return 4; else
						if (dia == "Saturday") return 5; else
							if (dia == "Sunday") return 6; else
								cout << "Día no válido" << endl;
}

int Bayes::calcularMesCorrespondiente(string mes){

	if (mes == "01") return 0; else
		if (mes == "02") return 1; else
			if (mes == "03") return 2; else
				if (mes == "04") return 3; else
					if (mes == "05") return 4; else
						if (mes == "06") return 5; else
							if (mes == "07") return 6; else
								if (mes == "08") return 7; else
									if (mes == "09") return 8; else
										if (mes == "10") return 9; else
											if (mes == "11") return 10; else
												if (mes == "12") return 11; else
													cout << "Mes no válido" << endl;
}

float Bayes::calcularProbabilidadPriori(Row row){

	int cantidadDeCrimenes = 0;
	float probabilidadPriori = 0,probabilidadPosteriori;

	map<string,float>::iterator iter = frecuenciaDeCrimenes.begin();
	while ((iter != frecuenciaDeCrimenes.end()) && (cantidadDeCrimenes < 39)){
		probabilidadPriori += this->calcularProbabilidadPosteriori(row,iter->first);
		iter++;
		cantidadDeCrimenes++;
	}

	return probabilidadPriori;
}

float Bayes::calcularProbabilidadPosteriori(Row row,string nombreDelCrimen){

	double p1,p2,p3,p4,p5;
	p5 = 1;

	int indiceDeHoras = atoi((row.hora).c_str());
	int indiceDeDias = calcularDiaCorrespondiente(row.diaDeLaSemana);
	int indiceDeDistritos = calcularDistritoCorrespondiente(row.distrito);
	int indiceDeMeses = calcularMesCorrespondiente(row.mes);
    int indiceCoordenadas = atoi((row.Parcela).c_str());

	p1 = (probabilidadesPorHoras[indiceDeHoras][nombreDelCrimen]);//de 0 a 23
	p2 = (probabilidadesPorDias[indiceDeDias][nombreDelCrimen]);//de 0 a 6
	p3 = (probabilidadesPorDistrito[indiceDeDistritos][nombreDelCrimen]);//de 0 a 9
	p4 = (probabilidadesPorMes[indiceDeMeses][nombreDelCrimen]);//de 0 a 11

	if(indiceCoordenadas != -1){
		p5 = (probabilidadesPorCoordenadas[indiceCoordenadas][nombreDelCrimen]);// de 0 a cantidadParcelas que es constante;
	}

	return p1*p2*p3*p4*p5;
}

float Bayes::calcularProbabilidadPorClase(float probabilidadDeClase,float probabilidadPriori,float probabilidadPosteriori){

	return (probabilidadDeClase*probabilidadPosteriori)/probabilidadPriori;
}

void Bayes::predecirRow(Row row,float* probabilidades){

	int cantidadDeCrimenes = 0;
	float probabilidadDeClase,probabilidadPriori,probabilidadPosteriori;

	probabilidadPriori = this->calcularProbabilidadPriori(row);//probabilidad del denominador

	map<string,float>::iterator iter = frecuenciaDeCrimenes.begin();
	while ((iter != frecuenciaDeCrimenes.end()) && (cantidadDeCrimenes < 39)){
		probabilidadPosteriori = this->calcularProbabilidadPosteriori(row,iter->first);//probabilidad del numerador
		probabilidadDeClase = probabilidadesDeCrimenes[iter->first];

		probabilidades[cantidadDeCrimenes] = calcularProbabilidadPorClase(probabilidadDeClase,
				probabilidadPriori,probabilidadPosteriori);
		iter++;
		cantidadDeCrimenes++;
	}
}

Bayes::~Bayes() {

}
