/*
 * CVSReader.h
 *
 *  Created on: 8 de nov. de 2015
 *      Author: joha
 */

#ifndef CVSREADER_H_
#define CVSREADER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>

using namespace std;

class CSVReader {

private:

	void calcularProbabilidad(map<string,float> &nombreMap,map<string,float> frecuenciaCrimenes);

public:

	CSVReader();

	void open(char *nameFile,int &cantidadDeRowsTrain,map<string,float> &frecuenciasCrimenes,
			map<string,int> &crimenesPorDistrito,map<string,float> &probabilidadesCrimenes,
			map<string,float> probabilidadesDias[7],map<string,float> probabilidadesDistritos[10],
			map<string,float> probabilidadesHoras[24],map<string,float> probabilidadesMes[12],
			map<string,int> &crimenesPorMes);

	void calcularProbabilidadesDeLosCrimenes(map<string,float> frecuenciaCrimenes,map<string,float> &probabilidadesCrimenes,
			int cantidadDeRowsTrain);

	void calcularCrimenesPorDistrito(map<string,int> &crimenesPorDistrito,string &csvItem,map<string,float> &bayview,
			map<string,float> &central,map<string,float> &ingleside,map<string,float> &mission,
			map<string,float> &northern,map<string,float> &park,map<string,float> &richmond,
			map<string,float> &southern,map<string,float> &taraval,map<string,float> &tenderloin,string &crimen);

	void calcularCrimenesPorDia(string &csvItem,string &crimen,map<string,float> &lunes,map<string,float> &martes,
			map<string,float> &miercoles,map<string,float> &jueves,map<string,float> &viernes,map<string,float> &sabado,
			map<string,float> &domingo);

	void calcularCrimenesPorMes(string &mesActual,string &delitoActual,map<string,float> &enero, map<string,float> &febrero,
			map<string,float> &marzo,map<string,float> &abril,map<string,float> &mayo,map<string,float> &junio,
			map<string,float> &julio,map<string,float> &agosto,map<string,float> &septiembre,
			map<string,float> &octubre,map<string,float> &noviembre,map<string,float> &diciembre,map<string,int> &crimenesPorMes);

	void calcularProbabilidadesDeLosCrimenesPorMes(map<string,float> &enero, map<string,float> &febrero,
			map<string,float> &marzo,map<string,float> &abril,map<string,float> &mayo,map<string,float> &junio,
			map<string,float> &julio,map<string,float> &agosto,map<string,float> &septiembre,
			map<string,float> &octubre,map<string,float> &noviembre,map<string,float> &diciembre,
			map<string,float> frecuenciaCrimenes);

	void calcularCrimenesPorHora(string horaActual,string delitoActual,map<string,float> crimenesPorHora[24],map<string,int> &horas);

	void calcularProbabilidadesDeCrimenesPorHora(map<string,float> crimenesPorHora[24], map<string,float> frecuenciaCrimenes);

	void calcularProbabilidadesDeCrimenesPorDia(map<string,float> &lunes,map<string,float> &martes,
			map<string,float> &miercoles,map<string,float> &jueves,map<string,float> &viernes,map<string,float> &sabado,
			map<string,float> &domingo,map<string,float> frecuenciaCrimenes);

	void calcularProbabilidadesDeCrimenesPorDistrito(map<string,float> &bayview,map<string,float> &central,
			map<string,float> &ingleside,map<string,float> &mission,map<string,float> &northern,map<string,float> &park,
			map<string,float> &richmond,map<string,float> &southern,map<string,float> &taraval,
			map<string,float> &tenderloin,map<string,float> frecuenciaCrimenes);

	void imprimirCrimenesPorHora(map<string,float> crimenesPorHora[24]);

	virtual ~CSVReader();
};

#endif /* CVSREADER_H_ */
