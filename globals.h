#ifndef globals
#define globals
#include <bits/stdc++.h>
using namespace std;
// print vectors
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {out<<'[';if(!v.empty())out<<v.front();for(int i=1;i<v.size();i++)out<<", "<<v[i];out<<']';return out;}

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

double crit_rates[5]; // 5 levels including 0, anything above stage 4 is stage 4
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
        if (lv<100) {
            return pow(lv,3) * (160-lv) / 100;
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

// Type struct
struct Type {
    string name;
    vector<string> meh_to, strong_to, no_effect_to; // type names
};
// A vector of all the type names - they'll mostly be accessed by types the map
vector<string> type_names;
// the map that gives access to the various types and their type advantage
map<string,Type> types;
#endif