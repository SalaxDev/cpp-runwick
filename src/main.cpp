#include "RunWick.hpp"

int main(int argc, const char** argv) {
    RunWick rw(WindowSettings(1000, 600, "RunWick", 0)) ;
    rw.init() ;
    rw.run() ;
    return 0 ;
}