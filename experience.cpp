#include <iostream>
#include <filesystem>
#include <functional>
#include <map>
#include <cmath>
using namespace std;
namespace fs = filesystem;
map<string/*pk name*/, function<int(int)> > lvup;
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
int main() {
    lvup["pikachu"]=erratic;
    cout << lvup["pikachu"](2) << endl;
}