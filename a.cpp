#include <iostream>
#include <map>
#include <string>
#include <functional>

void foo() { std::cout << "foo()"; }
void boo() { std::cout << "boo()"; }
void too() { std::cout << "too()"; }
void goo() { std::cout << "goo()"; }

int main() {
  std::map<std::string, std::function<void()>> functions;
  functions["foo"] = foo;
  functions["boo"] = boo;
  functions["too"] = too;
  functions["goo"] = goo;

  std::string func;
  std::cin >> func;
  functions[func]();
  return 0;
}