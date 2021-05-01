// hello.cpp
module;
#include <iostream>
#include <string_view>

export module smd.hello;

export namespace hello {
void hello(std::string_view name)
{
  std::cout << "Hello, " << name << "! \n";
}
}  // namespace hello
