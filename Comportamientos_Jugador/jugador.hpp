#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

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
  list<Action> secuencia;

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

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      //_init();
      hayPlan = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      //_init();
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

    /** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st*/
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);

  
  private:
    void _init();

  private:
    list<Action> plan;
    bool hayPlan;

    stateN0 c_state;
    ubicacion goal;

};

/** Devuelve si una ubicación en el mapa es transitable para el agente*/
bool CasillaTransitable (const ubicacion & x, const vector<vector<unsigned char>> & mapa);

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos);

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, devuelve como salida el mismo estado de entrada
*/
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa);

/**Sobrecarga de la función Find para buscar en lista de nodeN0*/
bool Find(const stateN0 &item, const list<nodeN0> &lista);

/** pone a cero todos los elementos de una matriz */
void AnulaMatriz(vector<vector<unsigned char> > &matriz);

void PintaPlan(const list<Action> &plan);

list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

#endif
