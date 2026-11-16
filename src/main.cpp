#include "RunWick.hpp"
#include "GameTexture.hpp"
#include "SDL3/SDL.h"

int main(int argc, const char** argv) {
    RunWick rw(WindowSettings(800, 600, "RunWick", 0)) ;
    rw.init() ;
    rw.run() ;
    
    
    return 0 ;
}