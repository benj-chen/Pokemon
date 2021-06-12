// Main.py is necessary for all of the display things, such as battle animations among other things.
#include<bits/stdc++.h>
using namespace std;
// For random number generation
    #include <cstdlib>
    #include <ctime>
    

// print vectors
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {out<<'[';if(!v.empty())out<<v.front();for(int i=1;i<v.size();i++)out<<", "<<v[i];out<<']';return out;}
// put effect statuses in a namespace
namespace effect {
    double super,not_very,normal,no,crit;
}

namespace exp_funcs {
    // https://bulbapedia.bulbagarden.net/wiki/Experience
    // Thank the c++ makers for implicit type conversion
    int erratic(int lv) {
        if (lv<50) {
            return pow(lv,3) * (100-lv) / 50;
        }
        if (lv<68) {
            return pow(lv,3)*(150-lv) / 100;
        }
        if (lv<98) {
            return pow(lv,3) * floor((1911-10*lv) / 3);
        }
        assert(0);
    }
    int fast(int lv) {
        return 4*pow(lv,3) / 5;
    }
    int medium_fast(int lv) {
        return pow(lv,3);
    }
    int medium_slow(int lv) {
        //  "according to the function, level 1 Pokémon in this group are calculated to have -54 experience points. This causes the experience underflow glitch in Generations I and II."
        // this only occurs for level 1 pokemon - level 2 is 4.
        // anyways this will make a special exception
        if (lv==1) return 1;
        return (pow(lv,3)*6/5) - 15*lv*lv + 100*lv -140;
    }
    int slow(int lv) {
        return 5*(pow(lv,3))/4;
    }
    int fluctuating(int lv) {
        if (lv<15) {
            return pow(lv,3) * ( ((lv+1)/3 + 24)/50 );
        }
        if (lv<36) {
            return pow(lv,3) * (lv+14)/50;
        }
        return pow(lv,3) * ((lv/2)+32)/50;
    }
    int exp_from_str(string cmd, int xp) {
        if (cmd=="erratic")     return erratic(xp);
        if (cmd=="fast")        return fast(xp);
        if (cmd=="medium fast") return medium_fast(xp);
        if (cmd=="medium slow") return medium_slow(xp);
        if (cmd=="slow")        return slow(xp);
        if (cmd=="fluctuating") return fluctuating(xp);
        cerr << "command was not erratic, fast, medium fast, medium slow, slow, or fluctuating" << endl;
        assert(0);
        return -1;
    }
}

double crit_rates[5]; // 5 levels including 0, anything above stage 4 is stage 4
// string splitting algorithm
vector<string> split_delimiter_char(const string s, const char delim) {
    if (s=="") return {};
    vector<string> res(1);
    for (char c: s) {
        if (c==delim) res.push_back("");
        else res.back()+=string(1,c);
    }
    return res;
}
// linear search algorithm for convenience
bool linear_search_str(const vector<string>& v, const string& s) {
    for (string st: v) {
        if (st==s) return true;
    }
    return false;
}
// Type struct
struct Type {
    string name;
    vector<string> meh_to, strong_to, no_effect_to; // type names
};
// A vector of all the type names - they'll mostly be accessed by types the map
vector<string> type_names;
// the map that gives access to the various types and their type advantage
map<string,Type> types;
// build the types, determining each type and what types they're effect against, not very effective against, and no-damage against
void build_types() {
    ifstream cin("input/type");
    cerr << "begin build types" << endl;
    cout << "Number of type names, i.e. 17 or 18 with the Fairy type: ";
    int num_types; cin >> num_types;
    cout << "Enter " << num_types << " type names, space separated or one on each line" << endl;
    for (int i=0;i<num_types;i++) {
        string s;
        cin >> s;
        type_names.push_back(s);
        types[s] = Type();
    }
    string input;
    getline(cin,input); // flush out remaining newline
    for (string s: type_names) {
        cout << "List all the types that " << s << " is super effective against, space separated in one line:" << endl;
        getline(cin,input);
        types[s].strong_to = split_delimiter_char(input,' ');
        for (string& st: types[s].strong_to) assert(linear_search_str(type_names,st));
        cout << "List all the types that " << s << " is not very effective against, space separated in one line:";
        getline(cin,input);
        types[s].meh_to = split_delimiter_char(input,' ');
        for (string& st: types[s].meh_to) assert(linear_search_str(type_names,st));
        cout << "List all the types that " << s << " has no effect against: ";
        getline(cin,input);
        types[s].no_effect_to = split_delimiter_char(input,' ');
        for (string& st: types[s].no_effect_to) assert(linear_search_str(type_names,st));
        // Standard effect is not asked for and is only triggered when the effect named is not in any other vector
        // in fact it doesn't even have a vector to hold.
        // putting the same type in two different vectors is a waste of effort for me to check
        // we'll call it unexpected behavior
    }
    cerr << "type building done" << endl;
    cin.close();
}

// Pokemon struct
struct Pokemon {
    // Pokemon traditionally have 4 moves to keep the strategy entertaining, otherwise everything would be good against everything, but vector.
    vector<string> moves;
    vector<string> types;
    // learnset in the Move class for easier modifying
    // in the games, each pokemon has a unique identification number called a Pokedex number.
    int pokedex_id;
    int hp, atk, spatk, def/* change when translating to Python */, spdef, spd; // Statistics: health, attack, special attack, defense, special defense, speed.
    // for more information on stats go to https://bulbapedia.bulbagarden.net/wiki/Stat#List_of_stats
    int mod_hp, mod_atk, mod_spatk, mod_def, mod_spdef, mod_spd; // for stat changing moves like Psych Up, Agility, etc. that refresh at the end of a battle
    // A Pokemon's crit level/stage can also be impacted by natures or items, thus the final crit stage is (this->crit_lv + moves[move].crit_lv)
    int crit_lv;
    // STAB is short for Same Type Attack Bonus, if the move that the pokemon uses is the same as one of its types.
    double stab_rate; // 1.5, unless the pokemon has the ability Adaptibility in which it is 2.
    // Pokemon have levels indicating how strong they are.
    int level;
    // function to determine how much xp is needed to get to the next level
    int (*exp_function) (int); 
    
    int needed_xp;
    void hit(string& move, Pokemon& other) {
        // the animations will be tied the move eventually. I'll actually be sending this to the Python program for pygame.
        // ?? send animate animation request to py, later
    }
};
// do the two vectors share a type?
bool type_match(const vector<string>& a, const vector<string>& b) {
    for (string c: a) {
        for (string d: b) {
            if (c==d) return true;
        }
    }
    return false;
}
// effect coefficient
double effectiveness(const vector<string>& a, const vector<string>& b) {
    double coefficient=effect::normal;
    // for each a[i] that is effective against b[j], multiply the coefficient
    for (string s: a) {
        for (string t: b) {
            if (linear_search_str(types[s].strong_to,t)) {
                coefficient*=effect::super;
            }
            else if (linear_search_str(types[s].meh_to,t)) {
                coefficient*=effect::not_very;
            }
            else if (linear_search_str(types[s].no_effect_to, t)) {
                coefficient*=effect::no;
            }
            else {
                coefficient*=effect::normal;
            }
        }
    }
    return coefficient;
    // for standard pokemon, effect::normal**2 is the standard effect, effect::no is no effect, effect::not_very or effect::not_very**2 is not very effective, etc.
}
// Move struct
struct Move {
    // usually have one type, but Flying Press has two (Flying, Fighting). They can have more. Let's put it in a vector.
    vector<string> types;
    // pokemon learn moves through leveling up and through machines called TMs (one-time use) or HMs (infinite use). Not all pokemon can learn a given move.
    // learnset determines if a Pokemon can learn this move
    vector<string> learnset;
    // accuracy - a number between -1 and 100 inclusive. If the move always happens, it should be -1, otherwise it represents the percent that the move hits
    int accuracy;
    // power - the amount of power the move has (different from the damage output because it's independent of the pokemon's attack stat). If the move is set damage, that is, always delivers the same amount of damage (in HP), then it should be instantiated as a SetDamageMove instead.
    int power;
    // in Pokemon, attacks can either be "attacks" aka "physical attacks", or special attacks. It's attack typing changes how the move is calculated - whether it uses the attacker's atk or spatk stat, and whether it uses the defender's def or spdef stat.
    bool is_spatk;
    // determine how frequent a critical hit should happen by stage - 0 is standard, but can be raised through items. Sometimes it's 1 if the move claims to have a high crit rate.
    int crit_lv;
    // If a crit landed, for example, it is added to this queue. This queue is checked and sent to Python for displaying. It is cleared after each turn.
    vector<string> msg_queue;
    Move(const vector<string>& types, const vector<string>& learnset, int accuracy, int power, bool is_spatk, int crit_lv):
    types(types),
    learnset(learnset),
    accuracy(accuracy),
    power(power),
    is_spatk(is_spatk),
    crit_lv(crit_lv)
    {}
    int calc_hp(Pokemon& attacker, Pokemon& defender) {
        int applied_atk = (is_spatk? attacker.spatk : attacker.atk),
        applied_def = (is_spatk? defender.spdef : defender.def);
        /*
        dmg = ( ( ( ((2*Level)/5)+2 ) * power * a/d )/50 + 2)
        
        * targets (if the pk is in a double battle with Surf which hits all opponents for example) is 0.75 if it hits more than one target and 1 otherwise. This will be calculated elsewhere.

        * weather: 1.5 if the move type is water in rain, or if the move type is fire in harsh sunlight. 0.5 if water in harsh sunlight or fire in rain. This will also be calculated elsewhere.

        * critical. 2 for a critical hit, 1 otherwise, usually. This is set by the user of course. It can be 1.5 if the gen it's based on is 6+.
        Critical hits (aka crits) have varying probabilities depending on the move being used, items and natures. There are 4 stages:
        0: crit rate = 1/16
        1: 1/8
        2: 1/4
        3: 1/3
        4+: 1/2
        Access the crit chance through the crit_rates array. i.e. crit_rates[0] refers to the crit rate at stage 0 which is most pokemon.

        * random. A float between 0.85 and 1.00.

        * STAB. If one of the move's types matches one of the pokemon's types then Pokemon.stab_rate is activated.
        
        * type, if the move's type is effective against the pokemon.

        * burn, 0.5 if the attacker is burned, 1 otherwise, with the exception of the Guts ability. As such this will be a local variable for the pokemon and calculated elsewhere.

        * other, which for our purposes is 1 but can change. I may update this later.
        */
        int dmg=  ( (( (attacker.level<<1) / 5)+2) * power * applied_atk / applied_def ) /50 + 2;
        dmg*=(rand()%101 /*rand int between 0 and 100*/ < crit_rates[ (crit_lv+attacker.crit_lv <=4 ? crit_lv+attacker.crit_lv : 4 /*round down to 4*/) ]? 1: effect::crit);
        dmg*=(rand()%16+85 /* random number in [0,16) + 85*/) / 100.0;
        if (type_match(types,attacker.types)) {
            dmg*=attacker.stab_rate;
        }
        dmg*=effectiveness(attacker.types,defender.types);
        return dmg;
    }
};
struct SetDamageMove: public Move {
    SetDamageMove(const vector<string>& types, const vector<string>& learnset, int accuracy, int amt_hp, bool is_spatk, int crit_lv): Move(types,learnset,accuracy,amt_hp,is_spatk, crit_lv)
    {}
    int calc_hp(Pokemon& attacker, Pokemon& defender) {
        return power;
    }
};
// build the moves, determining their types, damage, accuracy etc. Some moves always happen, like status changing moves, healing moves, and Aerial Ace. These are represented as "---" in the game, which is why accuracy is a string.
void build_moves() {
    cerr << "building moves now" << endl;
    cout << "List the number of moves" << endl;
    int num_moves;
    cin >> num_moves;
    cout << "For the next " << num_moves << " lines, list the name, power, and accuracy (-1 if it always hits), " << endl;
    cerr << "move building done" << endl;
}


struct Pokemon;
// struct Move {
//     string name;
//     int pp,power;
//     int accuracy;
//     Move(string _name, int _pp, int _power) {
//         name=_name;
//         pp=_pp;
//         power=_power;
//     }
//     void hit_pokemon(Pokemon& sender, Pokemon& receiver) {
//         // https://bulbapedia.bulbagarden.net/wiki/Damage
//         int damage=0;

//     }
// };


// Pokemon struct, unlike with Move and Type, they won't be accessed by their name because you can have multiple, the leveling and stats system is too complicated to be generalized into one instance of the class, etc.
// struct Pokemon {
//     int hp, atk, spatk, def, spdef, spd; // Health Points, Attack, Special Attack, Defense, Special Defense, and Speed
//     // bool dead; // 0 if the pokemon is alive, 1 for if the pokemon has fainted
//     // vector<Type> types; // could be 1 or 2
//     vector<string> current_types; // the amount is usually 1 or 2, "current" to avoid naming conflict
//     vector<string> current_moves; // usually max out at 4
//     int level;
//     int xp;
//     int 
//     bool dead=false;
//     Pokemon(int hp) {
//     }
//     void learn_move(string m) {
//         // m already in moves?
//         if (linear_search_str(moves[m].learnset,name) && !linear_search_str(current_moves,name)) {
//             current_moves.push_back(m);
//         }
//     }

// };


/* mechanics ignored:
Status (poison, paralysis) (does not include fainting)
IVs, which modify a Pokemon's stats
EVs
Weird moves like Double Team, Hidden Power, Refresh, etc. Even Aerial Ace for now as it always hits
Natures
STAB (for abilities like Protean)
Abilities
Berries
Weird evolutions like Eevee
Stat modifications like Growl, Leer, etc.
*/
/*
bool is_dead(Pokemon& to_check) {
    if (to_check.hp<=0) {
        to_check.hp=0;
        to_check.dead=true;
        return true;
    }
    return false;
}
*/

void build_effect_multipliers() {
    // cin has to route from "input/effect" when the process is run from the master directory
    ifstream cin("input/effect");
    cerr << "begin effect multiplier setup, where you establish how much more damage a move does if effective or not etc." << endl;
    cout << "Super effective multipler? i.e. 2 ";
    cin >> effect::super;
    cout << "Not very effective multipler? i.e. 0.5 ";
    cin >> effect::not_very;
    cout << "Normal effect multipler? i.e. 1 ";
    cin >> effect::normal;
    cout << "No effect multiplier? i.e. 0 ";
    cin >> effect::no;
    cout << endl;
    cout << "Critical hit multiplier? i.e. 1.5 or 2";
    cin >> effect::crit;
    cerr << "effect multiplier build success and done" << endl;
    cin.close();
}
void build_crit_rates() {
    ifstream cin("input/crit_rates");
    // crit input is based on percent, like 6.25% would be inputted as 6.25
    cin >> crit_rates[0] >> crit_rates[1] >> crit_rates[2] >> crit_rates[3] >> crit_rates[4];
}
int main() {
    srand(time(NULL));
    for (int i=1;i<99;i++) {
        cout << exp_funcs::erratic(i) << endl;
    }
    // build_effect_multipliers();
    // build_types();
}