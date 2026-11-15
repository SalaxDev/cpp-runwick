#include "GameTexture.hpp"
#include <SDL3_image/SDL_image.h>

using namespace RW ;

// Constructor
Player::Player(SDL_Renderer *&rnd, std::string runTexLocation, std::string jumpTexLocation, std::string jumpFallTexLocation, std::string idleTexLocation)
    : rnd(rnd), runTexLoc(std::move(runTexLocation)), jumpTexLoc(std::move(runTexLocation)),
    jumpFallTexLoc(std::move(jumpFallTexLocation)), idleTexLoc(std::move(idleTexLocation)) {
    
    
}

// Destructor
Player::~Player() {
    if (runTex) {
        SDL_DestroyTexture(runTex) ;
        runTex = nullptr ;
    }
    
    if (jumpTex) {
        SDL_DestroyTexture(jumpTex) ;
        jumpTex = nullptr ;
    }
    
    if (jumpFallTex) {
        SDL_DestroyTexture(jumpFallTex) ;
        jumpFallTex = nullptr ;
    }
    
    if (idleTex) {
        SDL_DestroyTexture(idleTex) ;
        idleTex = nullptr ;
    }
}


// Getters

// Setters
Player &Player::setRunTexLocation(std::string location) noexcept {
    if (!location.empty()) {
        runTexLoc = location ;
    } else {
        SDL_Log("Provided location is empty %s\n", __FUNCTION__) ;
    }
    return *this ;
}

Player &Player::setJumpTexLocation(std::string location) noexcept {
    if (!location.empty()) {
        jumpTexLoc = location ;
    } else {
        SDL_Log("Provided location is empty %s\n", __FUNCTION__) ;
    }
    return *this ;
}

Player &Player::setJumpFallTexLocation(std::string location) noexcept {
    if (!location.empty()) {
        jumpFallTexLoc = location ;
    } else {
        SDL_Log("Provided location is empty %s\n", __FUNCTION__) ;
    }
    return *this ;
}

Player &Player::setIdleTexLocaiton(std::string location) noexcept {
    if (!location.empty()) {
        idleTexLoc = location ;
    } else {
        SDL_Log("Provided location is empty %s\n", __FUNCTION__) ;
    }
    return *this ;
}


// Functions
void Player::jump() {
    
}

void Player::render() {
    
}

void Player::update() {
    if (bLoadFailed) {
        SDL_Log("Failed to properly load textures!, SDL Error: %s\n", SDL_GetError()) ;
    }
}

bool Player::initTextures() {
    runTex = IMG_LoadTexture(rnd, runTexLoc.c_str()) ;
    if (!runTex) {
        SDL_Log("Failed to Load Run Texture [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    jumpTex = IMG_LoadTexture(rnd, jumpTexLoc.c_str()) ;
    if (!jumpTex) {
        SDL_Log("Failed to Load Jump Texture [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    jumpFallTex = IMG_LoadTexture(rnd, jumpFallTexLoc.c_str()) ;
    if (!jumpFallTex) {
        SDL_Log("Failed to Load Jump Fall Texture [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    idleTex = IMG_LoadTexture(rnd, idleTexLoc.c_str()) ;
    if (!idleTex) {
        SDL_Log("Failed to Load Idle Texture [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    return true ;
}