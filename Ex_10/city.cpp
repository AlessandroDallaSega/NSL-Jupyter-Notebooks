#include "city.h"

city :: city(int idx, double xcoord, double ycoord){
    index = idx;
    x = xcoord;
    y = ycoord;
}

city :: city(){}

city& city :: operator=(const city& other) {
    if (this == &other) {
        return *this; // Gestione dell'auto-assegnazione
    }
        
    index = other.index;
    x = other.x;
    y = other.y;

    return *this;
}