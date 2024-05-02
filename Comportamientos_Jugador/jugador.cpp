#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>



// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	
	if (sensores.nivel != 4) {
		if (!hayPlan) {
			cout << "Calculamos nuevo plan" << endl;

			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;

			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;

			c_state.ultimaOrdenColaborador = act_CLB_STOP;

			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;
			
			switch (sensores.nivel)
			{
			case 0:
				stateN0 csN0;
				csN0.jugador = c_state.jugador;
				csN0.colaborador = c_state.colaborador;
				csN0.ultimaOrdenColaborador = c_state.ultimaOrdenColaborador;

				plan = AnchuraSoloJugador(csN0, goal, mapaResultado);
				break;
			case 1:
				stateN1 csN1;
				csN1.jugador = c_state.jugador;
				csN1.colaborador = c_state.colaborador;
				csN1.ultimaOrdenColaborador = c_state.ultimaOrdenColaborador;
				plan = AnchuraColaborador(csN1, goal, mapaResultado);

				break;

			case 2:
				stateN2 csN2;
				csN2.jugador = c_state.jugador;
				csN2.colaborador = c_state.colaborador;
				csN2.ultimaOrdenColaborador = c_state.ultimaOrdenColaborador;
				
				if (mapaResultado.at(c_state.jugador.f).at(c_state.jugador.c) == 'K') {
					csN2.potenciador = bikini_on;
				} else if (mapaResultado.at(c_state.jugador.f).at(c_state.jugador.c) == 'D') {
					csN2.potenciador = zapatillas_on;
				} else {
					csN2.potenciador = items_off;
				}

				plan = CosteUniformeBateria(csN2, goal, mapaResultado);
				break;

			case 3:
				break;
			}

			if (plan.size() > 0){
				VisualizaPlan(c_state, plan);
				hayPlan = true;
			}
		}

		if (hayPlan and plan.size() > 0){
			accion = plan.front();
			plan.pop_front();
		}

		if (plan.size() == 0){
			hayPlan = false;
		}
	
	} else {
		// NIVEL 4
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

void ComportamientoJugador::_init(){
	hayPlan = false;
}

/** Devuelve si una ubicación en el mapa es transitable para el agente*/
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M' );
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, devuelve como salida el mismo estado de entrada
*/
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > & mapa){
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}
	return st_result;
}

/**Sobrecarga de la función Find para buscar en lista de nodeN0*/
bool Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}

void PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK){
			cout << "W ";
		}
		else if (*it == actRUN){
			cout << "R ";
		}
		else if (*it == actTURN_SR){
			cout << "r ";
		}
		else if (*it == actTURN_L){
			cout << "L ";
		}
		else if (*it == act_CLB_WALK){
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP){
			cout << "cS ";
		}
		else if (*it == actIDLE){
			cout << "I ";
		}
		else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

/** pone a cero todos los elementos de una matriz */
void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

/** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st*/
void ComportamientoJugador::VisualizaPlan(const state &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	state cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

bool EsSolucion(const stateN0 & st, const ubicacion & final) {
	return (st.jugador.f == final.f and st.jugador.c == final.c);
}

/** primera aproximación a la implementación de la búsqueda en anchura */
list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	vector< nodeN0 * > nodos_creados;
	list<nodeN0 *> frontier;
	set<nodeN0> explored;
	list<Action> plan;

	nodeN0 * current_node = new nodeN0;
	(*current_node).st = inicio;
	(*current_node).parent = nullptr;

	nodos_creados.push_back(current_node);

	bool SolutionFound = ((*current_node).st.jugador.f == final.f and
						  (*current_node).st.jugador.c == final.c);

	frontier.push_back(current_node);
	

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.insert((*current_node));

		// Generar hijo actWALK
		nodeN0 * child_walk = new nodeN0;
		nodos_creados.push_back(child_walk);
		(*child_walk).st = apply(actWALK, (*current_node).st, mapa);

		(*child_walk).last_action = actWALK;
		(*child_walk).parent = current_node;
		
		if (EsSolucion((*child_walk).st, final)){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if (explored.find((*child_walk)) == explored.end()){
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN0 * child_run = new nodeN0;
			nodos_creados.push_back(child_run);
			(*child_run).st = apply(actRUN, (*current_node).st, mapa);

			(*child_run).last_action = actRUN;
			(*child_run).parent = current_node;
			
			if (EsSolucion((*child_run).st, final)){
				current_node = child_run;
				SolutionFound = true;
			}
			else if (explored.find((*child_run)) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			nodeN0 * child_turnl = new nodeN0;
			nodos_creados.push_back(child_turnl);
			(*child_turnl).st = apply(actTURN_L, (*current_node).st, mapa);

			(*child_turnl).last_action = actTURN_L;
			(*child_turnl).parent = current_node;
			
			if (explored.find((*child_turnl)) == explored.end()){
				frontier.push_back(child_turnl);
			}		
			// Generar hijo actTURN_SR
			nodeN0 * child_turnsr = new nodeN0;
			nodos_creados.push_back(child_turnsr);
			(*child_turnsr).st = apply(actTURN_SR, (*current_node).st, mapa);

			(*child_turnsr).last_action = actTURN_SR;
			(*child_turnsr).parent = current_node;

			if (explored.find((*child_turnsr)) == explored.end()){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find((*current_node)) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()){
					current_node = frontier.front();
				}
			}

			

		}
	}

	if(SolutionFound){
		nodeN0 * cn = current_node;
		while ((*cn).parent != nullptr){
			plan.push_front((*cn).last_action);
			
			cn = (*cn).parent;
		}
		
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}

	// Liberamos memoria dinamica
	for (auto it = nodos_creados.begin(); it != nodos_creados.end(); it++){
		delete (*it);
	}
	
	return plan;
}

// ================================================================================================
//										NIVEL 1
// ================================================================================================

stateN1 apply(const Action &a, const stateN1 &st, const vector<vector<unsigned char> > & mapa){
	stateN1 st_result = st;

	// Actualizo posicion jugador

	ubicacion sig_ubicacion, sig_ubicacion2;

	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}

		
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;

	case (act_CLB_WALK):
		if (VeoColaborador(st)) st_result.ultimaOrdenColaborador = act_CLB_WALK;
		break;

	case (act_CLB_TURN_SR):
		if (VeoColaborador(st)) st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
		break;

	case(act_CLB_STOP):
		if (VeoColaborador(st)) st_result.ultimaOrdenColaborador = act_CLB_STOP;
		break;
	}

	// Actualizo posicion colaborador
	ubicacion CLB_sig_ubicacion;

	switch (st_result.ultimaOrdenColaborador){
	case (act_CLB_WALK):
		CLB_sig_ubicacion = NextCasilla(st.colaborador);

		if (CasillaTransitable(CLB_sig_ubicacion, mapa) and 
			!(CLB_sig_ubicacion.f == st.jugador.f and CLB_sig_ubicacion.c == st.jugador.c)){
				st_result.colaborador = CLB_sig_ubicacion;
			}

		break;

	case (act_CLB_TURN_SR):
		st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
		break;

	}


	return st_result;
}

bool EsSolucion(const stateN1 & st, const ubicacion & final){
	return(st.colaborador.f == final.f and st.colaborador.c == final.c);
}

bool PerteneceA (const ubicacion & ub, const pair<int, int> & x1, const pair<int,int> & x2) {	
	bool pertenece = false;
	
	// Calculo cual es la esquina inferior izquierda y cual es la superior derecha
	pair<int,int> esq_izda, esq_dcha;

	if (x1.first >= x2.first) {
		if (x1.second <= x2.second) {
			esq_izda = x1;
			esq_dcha = x2;
		} else {
			esq_izda.first = x1.first;
			esq_izda.second = x2.second;

			esq_dcha.first = x2.first;
			esq_dcha.second = x1.second;
		}
	} else {
		if (x2.second <= x1.second) {
			esq_izda = x2;
			esq_dcha = x1;
		} else {
			esq_izda.first = x2.first;
			esq_izda.second = x1.second;

			esq_dcha.first = x1.first;
			esq_dcha.second = x2.second;
		}
	}
	
	if (ub.f <= esq_izda.first and ub.c >= esq_izda.second and
		ub.f >= esq_dcha.first and ub.c <= esq_dcha.second) {

		pertenece = true;
	}

	return (pertenece);
}

bool VeoColaborador(const stateN1 & st) {
	bool lo_veo = false;

	if (st.jugador.brujula%2 == 0){ // Norte - Este - Sur - Oeste
		int mult = 1;

		if (st.jugador.brujula == sur || st.jugador.brujula == oeste) mult = -1;

		if (st.jugador.brujula == norte || st.jugador.brujula == sur) {	// Norte - Sur
			pair<int, int> x1 (st.jugador.f - (1*mult), st.jugador.c - 1);
			pair<int, int> x2 (st.jugador.f - (3*mult), st.jugador.c + 1);

			if (!PerteneceA(st.colaborador, x1, x2)) {
				x1 = pair<int, int>(st.jugador.f - (2*mult), st.jugador.c - 3);
				x2 = pair<int, int>(x1.first - (1*mult), x1.second + 1);

				if (st.colaborador.f != x1.first or st.colaborador.c != x1.second) {
					if (!PerteneceA(st.colaborador, x1, x2)) {
						x1.second += 6;
						x2.second += 4;

						if ((st.colaborador.f != x1.first or st.colaborador.c != x1.second) and PerteneceA(st.colaborador, x1, x2)) {
							lo_veo = true;
						}
					} else {
						lo_veo = true;
					}
				}

			} else {
				lo_veo = true;
			}

		} else {	// Este - Oeste
			pair<int, int> x1 (st.jugador.f - 1, st.jugador.c + (1*mult));
			pair<int, int> x2 (st.jugador.f + 1, st.jugador.c + (3*mult));

			if (!PerteneceA(st.colaborador, x1, x2)) {
				x1 = pair<int, int>(st.jugador.f - 3, st.jugador.c + (2*mult));
				x2 = pair<int, int>(x1.first + 1, x1.second + (1*mult));

				if (st.colaborador.f != x1.first or st.colaborador.c != x1.second) {
					if (!PerteneceA(st.colaborador, x1, x2)) {
						x1.first += 6;
						x2.first += 4;

						if ((st.colaborador.f != x1.first or st.colaborador.c != x1.second) and PerteneceA(st.colaborador, x1, x2)) {
							lo_veo = true;
						}
					} else {
						lo_veo = true;
					}
				}
				
			} else {
				lo_veo = true;
			}

		}
	} else {	// Diagonales
		int i = 1;
		int j = 1;

		if (st.jugador.brujula == sureste || st.jugador.brujula == suroeste) i = -1;
		if (st.jugador.brujula == suroeste || st.jugador.brujula == noroeste) j = -1;

		pair<int, int> x1 (st.jugador.f, st.jugador.c);
		pair<int, int> x2 (st.jugador.f - (3*i), st.jugador.c + (3*j));
		
		if (PerteneceA(st.colaborador, x1, x2)) {
			lo_veo = true;
			
		}
	}

	return (lo_veo);
}


list<Action> AnchuraColaborador(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	vector< nodeN1 * > nodos_creados;

	list<nodeN1 *> frontier;
	set<nodeN1> explored;
	list<Action> plan;

	nodeN1 * current_node = new nodeN1;
	nodos_creados.push_back(current_node);

	(*current_node).st = inicio;
	(*current_node).parent = nullptr;

	bool SolutionFound = EsSolucion((*current_node).st, final);

	frontier.push_back(current_node);
	
	//int niter = 0;

	while (!frontier.empty() and !SolutionFound){
		//++niter;
		frontier.pop_front();
		explored.insert((*current_node));

		// Generar hijo actIDLE
		nodeN1 * child_idle = new nodeN1;
		nodos_creados.push_back(child_idle);
		(*child_idle).st = apply(actIDLE, (*current_node).st, mapa);

		(*child_idle).last_action = actIDLE;
		(*child_idle).parent = current_node;
		
		if (EsSolucion((*child_idle).st, final)){
			current_node = child_idle;
			SolutionFound = true;
		}
		else if (explored.find((*child_idle)) == explored.end()){
			frontier.push_back(child_idle);
		}

		if (!SolutionFound) {
			// Genero hijo actCLB_WALK
			nodeN1 * child_CLBWalk = new nodeN1;
			
			(*child_CLBWalk).st = apply(act_CLB_WALK, (*current_node).st, mapa);

			if ((*child_CLBWalk) == (*child_idle)) {
				delete child_CLBWalk;
			} else {
				nodos_creados.push_back(child_CLBWalk);
				
				(*child_CLBWalk).last_action = act_CLB_WALK;
				(*child_CLBWalk).parent = current_node;

				if (EsSolucion((*child_CLBWalk).st, final)){
					current_node = child_CLBWalk;
					SolutionFound = true;
				} else if (explored.find((*child_CLBWalk)) == explored.end()) {
					frontier.push_back(child_CLBWalk);
				}
			}

		}
		if (!SolutionFound) {
			// Genero hijo actCLB_TURNSR
			nodeN1 * child_CLBTurnSr = new nodeN1;
			(*child_CLBTurnSr).st = apply(act_CLB_TURN_SR, (*current_node).st, mapa);

			if ((*child_CLBTurnSr) == (*child_idle)) {
				delete child_CLBTurnSr;
			} else {
				nodos_creados.push_back(child_CLBTurnSr);

				(*child_CLBTurnSr).last_action = act_CLB_TURN_SR;
				(*child_CLBTurnSr).parent = current_node;

				if (EsSolucion((*child_CLBTurnSr).st, final)){
					current_node = child_CLBTurnSr;
					SolutionFound = true;
				} else if (explored.find((*child_CLBTurnSr)) == explored.end()) {
					frontier.push_back(child_CLBTurnSr);
				}

			}
		}

		if (!SolutionFound) {
			// Genero hijo actCLB_STOP
			nodeN1 * child_CLBStop = new nodeN1;
			(*child_CLBStop).st = apply(act_CLB_STOP, (*current_node).st, mapa);

			if ((*child_CLBStop) == (*child_idle)) {
				delete child_CLBStop;
			} else {
				nodos_creados.push_back(child_CLBStop);

				(*child_CLBStop).last_action = act_CLB_STOP;
				(*child_CLBStop).parent = current_node;

				if (EsSolucion((*child_CLBStop).st, final)){
					current_node = child_CLBStop;
					SolutionFound = true;
				} else if (explored.find((*child_CLBStop)) == explored.end()) {
					frontier.push_back(child_CLBStop);
				}

			}

		}
		

		if (!SolutionFound) {
			// Generar hijo actWALK
			nodeN1 * child_walk = new nodeN1;
			nodos_creados.push_back(child_walk);
			(*child_walk).st = apply(actWALK, (*current_node).st, mapa);

			(*child_walk).last_action = actWALK;
			(*child_walk).parent = current_node;
			
			if (EsSolucion((*child_walk).st, final)){
					current_node = child_walk;
					SolutionFound = true;
			} else if (explored.find((*child_walk)) == explored.end()){
				frontier.push_back(child_walk);
			}
		}

		if (!SolutionFound) {
			// Generar hijo actRUN
			nodeN1 * child_run = new nodeN1;
			nodos_creados.push_back(child_run);
			(*child_run).st = apply(actRUN, (*current_node).st, mapa);

			(*child_run).last_action = actRUN;
			(*child_run).parent = current_node;
			
			if (EsSolucion((*child_run).st, final)){
					current_node = child_run;
					SolutionFound = true;
			} else if (explored.find((*child_run)) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound) {	// Si Idle no ha sido solucion, TurnL y TurnSr tampoco lo serán
			// Generar hijo actTURN_L
			nodeN1 * child_turnl = new nodeN1;
			nodos_creados.push_back(child_turnl);
			(*child_turnl).st = apply(actTURN_L, (*current_node).st, mapa);

			(*child_turnl).last_action = actTURN_L;
			(*child_turnl).parent = current_node;
			
			if (explored.find((*child_turnl)) == explored.end()){
				frontier.push_back(child_turnl);
			}		
			// Generar hijo actTURN_SR
			nodeN1 * child_turnsr = new nodeN1;
			nodos_creados.push_back(child_turnsr);
			(*child_turnsr).st = apply(actTURN_SR, (*current_node).st, mapa);

			(*child_turnsr).last_action = actTURN_SR;
			(*child_turnsr).parent = current_node;

			if (explored.find((*child_turnsr)) == explored.end()){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while(!frontier.empty() and explored.find((*current_node)) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()){
					current_node = frontier.front();
				}
			}

			

		}
	}

	//cout << "Nº iter:\t" << niter << endl;

	if(SolutionFound){
		nodeN1 * cn = current_node;
		while ((*cn).parent != nullptr){
			plan.push_front((*cn).last_action);
			
			cn = (*cn).parent;
		}
		
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}
	
	// Liberamos memoria dinamica
	for (auto it = nodos_creados.begin(); it != nodos_creados.end(); it++){
		delete (*it);
	}

	return plan;
}


// ================================================================================================
//											NIVEL 2
// ================================================================================================

stateN2 apply(const Action &a, const stateN2 &st, const vector<vector<unsigned char> > & mapa){
	stateN2 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;

				if (st_result.potenciador != bikini_on and mapa.at(sig_ubicacion.f).at(sig_ubicacion.c) == 'K') {
					st_result.potenciador = bikini_on;
				}

				if (st_result.potenciador != zapatillas_on and mapa.at(sig_ubicacion.f).at(sig_ubicacion.c) == 'D') {
					st_result.potenciador = zapatillas_on;
				}
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;

						if (st_result.potenciador != bikini_on and mapa.at(sig_ubicacion2.f).at(sig_ubicacion2.c) == 'K') {
							st_result.potenciador = bikini_on;
						}

						if (st_result.potenciador != zapatillas_on and mapa.at(sig_ubicacion2.f).at(sig_ubicacion2.c) == 'D') {
							st_result.potenciador = zapatillas_on;
						}
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}
	return st_result;
}

bool EsSolucion(const stateN2 & st, const ubicacion & final) {
	return (st.jugador.f == final.f and st.jugador.c == final.c);
}

int CalculaCosteBateria (const stateN2 & st, const Action & accion, const vector<vector<unsigned char>> & mapa) {
	unsigned char code_casilla = 'R';
	unsigned char casilla_actual = mapa.at(st.jugador.f).at(st.jugador.c);

	if (casilla_actual == 'A') {
		if (st.potenciador == bikini_on) {
			code_casilla = 'a';
		} else {
			code_casilla = 'A';
		}
	
	} else if (casilla_actual == 'B') {
		if (st.potenciador == zapatillas_on) {
			code_casilla = 'b';
		} else {
			code_casilla = 'B';
		}
	} else if (casilla_actual == 'T') {
		code_casilla = 'T';
	}

	int coste = 0;

	if (accion == actWALK) {
		coste = walkCost.at(code_casilla);

	} else if (accion == actRUN) {
		coste = runCost.at(code_casilla);

	} else if (accion == actTURN_L) {
		coste = turnLCost.at(code_casilla);

	} else if (accion == actTURN_SR) {
		coste = turnSrCost.at(code_casilla);

	}

	return (coste);
}

// TODO: Hacer sistema de busqueda por coste uniforme
list<Action> CosteUniformeBateria (const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	vector< nodeN2 * > nodos_creados;
	priority_queue<nodeN2*, vector<nodeN2*>, ComparePointerNodeN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;

	nodeN2 * current_node = new nodeN2;
	(*current_node).st = inicio;
	(*current_node).parent = nullptr;
	(*current_node).last_action = actIDLE;
	(*current_node).bateriaGastada = 0;

	nodos_creados.push_back(current_node);

	frontier.push(current_node);

	bool SolutionFound = false;

	int niter = 0;

	while (!frontier.empty() and !SolutionFound){
		++niter;

		frontier.pop();
		explored.insert((*current_node));

		if (EsSolucion((*current_node).st, final)) {
			SolutionFound = true;
		}

		if (!SolutionFound) {
			// Generar hijo actWALK
			nodeN2 * child_walk = new nodeN2;
			nodos_creados.push_back(child_walk);
			(*child_walk).st = apply(actWALK, (*current_node).st, mapa);

			(*child_walk).last_action = actWALK;
			(*child_walk).parent = current_node;
			(*child_walk).bateriaGastada = (*current_node).bateriaGastada + CalculaCosteBateria((*current_node).st, actWALK, mapa);
			
			if (explored.find((*child_walk)) == explored.end()){
				frontier.push(child_walk);
			}

			// Generar hijo actRUN
			nodeN2 * child_run = new nodeN2;
			nodos_creados.push_back(child_run);
			(*child_run).st = apply(actRUN, (*current_node).st, mapa);

			(*child_run).last_action = actRUN;
			(*child_run).parent = current_node;
			(*child_run).bateriaGastada = (*current_node).bateriaGastada + CalculaCosteBateria((*current_node).st, actRUN, mapa);
			
			if (explored.find((*child_run)) == explored.end()){
				frontier.push(child_run);
			}

			// Generar hijo actTURN_L
			nodeN2 * child_turnl = new nodeN2;
			nodos_creados.push_back(child_turnl);
			(*child_turnl).st = apply(actTURN_L, (*current_node).st, mapa);

			(*child_turnl).last_action = actTURN_L;
			(*child_turnl).parent = current_node;
			(*child_turnl).bateriaGastada = (*current_node).bateriaGastada + CalculaCosteBateria((*current_node).st, actTURN_L, mapa);
			
			if (explored.find((*child_turnl)) == explored.end()){
				frontier.push(child_turnl);
			}		
			// Generar hijo actTURN_SR
			nodeN2 * child_turnsr = new nodeN2;
			nodos_creados.push_back(child_turnsr);
			(*child_turnsr).st = apply(actTURN_SR, (*current_node).st, mapa);

			(*child_turnsr).last_action = actTURN_SR;
			(*child_turnsr).parent = current_node;
			(*child_turnsr).bateriaGastada = (*current_node).bateriaGastada + CalculaCosteBateria((*current_node).st, actTURN_SR, mapa);

			if (explored.find((*child_turnsr)) == explored.end()){
				frontier.push(child_turnsr);
			}
		}




		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.top();
			while(!frontier.empty() and explored.find((*current_node)) != explored.end()){
				frontier.pop();
				if(!frontier.empty()){
					current_node = frontier.top();
				}
			}

			

		}
	}
	//cout << "Nº iter: " << niter << endl;
	//cout << "Nº abiertos: " << frontier.size() << endl;
	//cout << "Nº cerrados: " << explored.size() << endl;
	if(SolutionFound){
		nodeN2 * cn = current_node;
		while ((*cn).parent != nullptr){
			plan.push_front((*cn).last_action);
			
			cn = (*cn).parent;
		}
		
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}

	// Liberamos memoria dinamica
	for (auto it = nodos_creados.begin(); it != nodos_creados.end(); it++){
		delete (*it);
	}
	
	return plan;
}