#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include <SDL3/SDL.h>
#include <string>

namespace RW {
    class Background {
        
    };
    
    class Player {
    private:
        // Player Textures
        SDL_Texture runTex ;
        SDL_Texture jumpTex ;
        SDL_Texture jumpFallTex ;
        SDL_Texture idleTex ;
        
        // SDL Variables
        SDL_Renderer *rnd = nullptr ;
        std::string runTexLoc ;
        std::string jumpTexLoc ;
        std::string jumpFallTexLoc ;
        std::string idleTexLoc ;
        
        // Velocity & Speed
        float nSpeed = 10.0f ;
        float nGravity = 2.4f ;
        
    public:
        // Constructor
        Player(SDL_Renderer &rnd, std::string runTexLocation = "", std::string jumpTexLocation = "", std::string idleTexLocation = "") ;
        // Destructor
        ~Player() ;
        
        // Getters
        
        // Setters
        Player &setRunTexLocation(std::string location) noexcept ;
        Player &setJumpTexLocation(std::string location) noexcept ;
        Player &setIdleTexLocaiton(std::string location) noexcept ;
        
        // Functions
        void jump() ;
        void render() ;
        void update() ;
        void initTextures() ;
    };
}

#endif // GAME_TEXTURE_HPP