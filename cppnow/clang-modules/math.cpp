module;

export module math;
import std;

export int add(int fir, int sec){
    return fir + sec;
}

export void greeting(std::string_view name) {
    std::cout << "Hello, " << name <<  "!\n";
}
