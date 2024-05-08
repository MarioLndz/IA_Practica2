#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <map>
#include <set>

struct state {
  ubicacion jugador;
  ubicacion colaborador;

  Action ultimaOrdenColaborador;
};

struct stateN0 {
  ubicacion jugador;
  ubicacion colaborador;

  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x) const {
    return (jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c);
  }
};

struct nodeN0 {
  stateN0 st;
  Action last_action;

  nodeN0 * parent;

  bool operator==(const nodeN0 & n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN0 &b) const {
    if (st.jugador.f < b.st.jugador.f) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula){
      return true;
    } else {
      return false;
    }
  }
};

struct stateN1 {
  ubicacion jugador;
  ubicacion colaborador;

  Action ultimaOrdenColaborador;

  bool operator==(const stateN1 &x) const {
    return (jugador == x.jugador and colaborador == x.colaborador and ultimaOrdenColaborador == x.ultimaOrdenColaborador);
  }
};

struct nodeN1 {
  stateN1 st;
  Action last_action;

  nodeN1 * parent;

  bool operator==(const nodeN1 & n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN1 &b) const {
    if (st.jugador.f < b.st.jugador.f) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f < b.st.colaborador.f){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c < b.st.colaborador.c){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula < b.st.colaborador.brujula){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and
               st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador){
      return true;
    } else {
      return false;
    }
  }
};

enum item_state {items_off, bikini_on, zapatillas_on};

struct stateN2 {
  ubicacion jugador;
  ubicacion colaborador;

  Action ultimaOrdenColaborador;

  item_state potenciador;

  bool operator==(const stateN2 & x) const {
    return (jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c and
            potenciador == x.potenciador);
  }
};

struct nodeN2 {
  stateN2 st;
  int bateriaGastada;

  Action last_action;

  nodeN2 * parent;

  bool operator==(const nodeN2 & n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN2 &b) const {
    if (st.jugador.f < b.st.jugador.f) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.potenciador < b.st.potenciador){
      return true;
    } else {
      return false;
    }
  }
};

class ComparePointerNodeN2 {
public:
  bool operator()(const nodeN2* x, const nodeN2* y) {
    if (x->bateriaGastada >= y->bateriaGastada) {
      return (true);
    }

    return (false);
  }
};


struct stateN3 {
  ubicacion jugador;
  ubicacion colaborador;

  Action ultimaOrdenColaborador;

  item_state jg_item;
  item_state clb_item;

  bool operator==(const stateN3 & x) const {
    return (jugador == x.jugador and colaborador == x.colaborador and ultimaOrdenColaborador == x.ultimaOrdenColaborador and 
            jg_item == x.jg_item and clb_item == x.clb_item);
  }
};

struct nodeN3 {
  stateN3 st;
  mutable int bateriaGastada;
  int heuristica;

  Action last_action;

  mutable nodeN3 * parent;
  vector<nodeN3*> childs;

  bool operator==(const nodeN3 & n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN3 &b) const {
    if (st.jugador.f < b.st.jugador.f) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f < b.st.colaborador.f){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c < b.st.colaborador.c){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula < b.st.colaborador.brujula){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and
               st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador){
      return true;

    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and 
               st.ultimaOrdenColaborador == b.st.ultimaOrdenColaborador and st.jg_item < b.st.jg_item){
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and
               st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and 
               st.ultimaOrdenColaborador == b.st.ultimaOrdenColaborador and st.jg_item == b.st.jg_item and st.clb_item < b.st.clb_item){
      return true;
    } else {
      return false;
    }
  }
};

class ComparePointerNodeN3 {
public:
  bool operator()(const nodeN3* x, const nodeN3* y) {
    if ((x->bateriaGastada + x->heuristica) >= (y->bateriaGastada + y->heuristica)) {
      return (true);
    }

    return (false);
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      _init();
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      _init();
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

    /** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st*/
    void VisualizaPlan(const state &st, const list<Action> &plan);

    // NIVEL 4
    void ActualizaState(const Action & last_action);

    void ActualizaUbicacion(ubicacion & ubi, const Action & last_action);

    bool PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const ubicacion & st, vector<vector<unsigned char>> & matriz);
  
  private:
    void _init();

  private:
    list<Action> plan;
    bool hayPlan;

    state c_state;
    ubicacion goal;

    Action last_action;
    bool ubicado;

    vector<ubicacion> casillasRecarga;

    const int MAX_BATERIA = 3000;
    const int MAX_CICLOS = 3000;

    bool buscoBateria;
    bool recargaBateria;


};

// ========================================================================
//                                Comunes
// ========================================================================
const map<unsigned char, int> walkCost = {{'A', 100}, {'B', 50}, {'T', 2}, {'R', 1}, {'a', 10}, {'b', 15}};
const map<unsigned char, int> runCost = {{'A', 150}, {'B', 75}, {'T', 3}, {'R', 1}, {'a', 15}, {'b', 25}};
const map<unsigned char, int> turnLCost = {{'A', 30}, {'B', 7}, {'T', 2}, {'R', 1}, {'a', 5}, {'b', 1}};
const map<unsigned char, int> turnSrCost = {{'A', 10}, {'B', 5}, {'T', 1}, {'R', 1}, {'a', 2}, {'b', 1}};

/** Devuelve si una ubicación en el mapa es transitable para el agente*/
bool CasillaTransitable (const ubicacion & x, const vector<vector<unsigned char>> & mapa);

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos);

/**Sobrecarga de la función Find para buscar en lista de nodeN0*/
bool Find(const stateN0 &item, const list<nodeN0> &lista);

/** pone a cero todos los elementos de una matriz */
void AnulaMatriz(vector<vector<unsigned char> > &matriz);

void PintaPlan(const list<Action> &plan);

// ========================================================================
//                                NIVEL 0
// ========================================================================

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, devuelve como salida el mismo estado de entrada
*/
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > & mapa);

bool EsSolucion(const stateN0 & st, const ubicacion & final);

list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

// ========================================================================
//                                NIVEL 1
// ========================================================================

stateN1 apply(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> & mapa, bool check_CLB);

bool VeoColaborador(const stateN1 & st);

list<Action> AnchuraColaborador(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

bool EsSolucion(const stateN1 & st, const ubicacion & final);

// ========================================================================
//                                NIVEL 2
// ========================================================================

list<Action> CosteUniformeBateria (const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

stateN2 apply(const Action &a, const stateN2 &st, const vector<vector<unsigned char> > & mapa);

bool EsSolucion(const stateN2 & st, const ubicacion & final);

int CalculaCosteBateria (const stateN2 & st, const Action & accion, const vector<vector<unsigned char>> & mapa);

// ========================================================================
//                                NIVEL 3
// ========================================================================

int CalculaCosteAccion (const stateN3 & st, const Action & accion, const Action & accion_clb, const vector<vector<unsigned char>> & mapa);

bool EsSolucion(const stateN3 & st, const ubicacion & final);

stateN3 apply(const Action &a, const stateN3 &st, const vector<vector<unsigned char> > & mapa, bool check_CLB);

bool VeoColaborador(const stateN3 & st);

list<Action> Algoritmo_A_Estrella (const stateN3 & inicio, const ubicacion & final, const vector<vector<unsigned char>> & mapa);

int HeuristicaNormaMaximo (const stateN3 & st, const ubicacion & final);

int CalculaHeuristica (const stateN3 & st, const ubicacion & final);

void ActualizaBateriaHijos(const vector<nodeN3> & childs, int nuevo_coste_bateria);



// ================================================================================


list<Action> AnchuraMenosPasos(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

stateN0 applyMP(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > & mapa);

ubicacion CasillaExploradaMasCercana (const ubicacion & jugador, const ubicacion & objetivo, const vector<vector<unsigned char>> & mapa);

bool PosicionValidaEnMapa (const int & posF, const int & posC, const vector<vector<unsigned char>> & mapa);

int CalculaDistancia (const ubicacion & jugador, const ubicacion & casilla_mas_cercana);

list<Action> AnchuraJugadorSinLimites(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

stateN0 applySL(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > & mapa);


#endif
