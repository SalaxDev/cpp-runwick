#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include <SDL3/SDL.h>

namespace RW {
    class Background {
        
    };
    
    class Player {
    private:
        // Player Textures
        SDL_Texture runTex ;
        SDL_Texture jumpTex ;
    public:
        // Constructor
        Player() ;
        // Destructor
        ~Player() ;
        
        // Functions
        void jump() ;
        
    };
}

#endif // GAME_TEXTURE_HPP