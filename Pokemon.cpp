#include "headers.h"
map<string, int> stat_index = {
    {"HP",0},
    {"ATK",1},
    {"DEF",2},
    {"SPATK",3},
    {"SPDEF",4},
    {"SPD",5}
};
struct Pokemon {
    // an instance of Pokemon is a Pokemon, not a species, because EVs, IVs, natures, sprite differ among pokemon.
    string name;
    vector<string> types; // types.size() is 1 or 2
    vector<int> stats, base_stat, IV, EV;
    string sprite_path; // a path that starts with "data/"... and ends with a .png file
    void setHP() {
        /*
        According to Bulbapedia:
        HP = (2 * Base + IV + (round down EV / 4) * Level) / 100 + Level + 10
        */
    }
    void force(int stat, int value) {
        // for stat, use the stats map to figure it out

    }
};
int main() {
    return 0;
}