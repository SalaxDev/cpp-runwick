#include "GameTexture.hpp"
#include <SDL3_image/SDL_image.h>
#include <utility>

using namespace RW ;

// Functions
void Player::animate(float dt) {
    fTimer += dt ;
    if (fTimer >= FRAME_TIME) {
        fTimer -= FRAME_TIME ;
        
        sourceRect.x = W_FRAME * (nCurrentFrame % nFrameCount) ;
        nCurrentFrame = (nCurrentFrame + 1) % nFrameCount ;
    }
}

// Constructor
Player::Player(SDL_Renderer *&rnd, std::string runTexLocation, std::string jumpTexLocation, std::string jumpFallTexLocation, std::string idleTexLocation)
    : rnd(rnd), runTexLoc(std::move(runTexLocation)), jumpTexLoc(std::move(jumpTexLocation)),
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

Player &Player::setIdleTexLocation(std::string location) noexcept {
    if (!location.empty()) {
        idleTexLoc = location ;
    } else {
        SDL_Log("Provided location is empty %s\n", __FUNCTION__) ;
    }
    return *this ;
}


// Functions
void Player::jump(float dt) {
    fPlayerGravity = -470.f ;
}

void Player::render() {
    
    SDL_RenderTexture(rnd, currentTex, &sourceRect, &locationRect) ;
}

void Player::update(float dt) {
    if (bLoadFailed) {
        SDL_Log("Failed to properly load textures!, SDL Error: %s\n", SDL_GetError()) ;
        return ;
    }
    
    const bool *keys = SDL_GetKeyboardState(nullptr) ;
    bool bSpaceK = keys[SDL_SCANCODE_SPACE] ;
    if (bSpaceK && !pStatus.bJump) {
        pStatus.bJump = true ;
        jump(dt) ;
    }
    
    if (pStatus.bIdle) {
        currentTex = idleTex ;
        nFrameCount = IDLE_FRAMES ;
    } else if (pStatus.bRun) {
        currentTex = runTex ;
        nFrameCount = RUN_FRAMES ;
    } else if (pStatus.bJump) {
        currentTex = (fPlayerGravity > 0.f) ? jumpFallTex : jumpTex ;
        if (fPlayerGravity > 0.f) {
            currentTex = jumpFallTex ;
            nFrameCount = JUMPFALL_FRAMES ;
        } else {
            currentTex = jumpTex ;
            nFrameCount = JUMP_FRAMES ;
        }
    } else if (pStatus.bDeath) {
        // TODO: Implement Death animation and health in later
    }
    
    // Physics Gravity Player
    if (pStatus.bJump) {
        locationRect.y += fPlayerGravity * dt ;
        
        fPlayerGravity += fGravity * dt ;
    }
    
    
    // Resetting Settings if player touched ground
    if (locationRect.y >= GROUND_Y) {
        locationRect.y = GROUND_Y ;
        fPlayerGravity = 0.f ;
        pStatus.bJump = false ;
    }
    
    animate(dt) ;
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

void Player::start() {
    if (!pStatus.bIdle) return ;
    
    pStatus.bRun = true ;
    pStatus.bIdle = false ;
    
    // Gravity reset
    fPlayerGravity = 0.f ;
    
    // Timer Animation Reset
    fTimer = 0.f ;
    nCurrentFrame = 0 ;
    nFrameCount = 0 ;
    
    // Player Rect Reset
    sourceRect.x = 0 ;
    sourceRect.y = 0 ;
    locationRect.x = 200 ;
    locationRect.y = GROUND_Y ;
}