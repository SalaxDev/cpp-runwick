#include "GameTexture.hpp"
#include <SDL3_image/SDL_image.h>
#include <utility>

using namespace RW ;


// Background Class

// Constructor
Background::Background(SDL_Renderer *rnd, std::string skyTexLocation, std::string groundTexLocation)
    : rnd(rnd), skyTexLoc((std::move(skyTexLocation))), groundTexLoc(std::move(groundTexLocation)) {
    
}

// Destructor
Background::~Background() {
    if (skyTex) {
        SDL_DestroyTexture(skyTex) ;
        skyTex = nullptr ;
    }
    
    if (groundTex) {
        SDL_DestroyTexture(groundTex) ;
        groundTex = nullptr ;
    }
}


// Setters
Background &Background::setSkyTexLocation(std::string location) {
    if (location.empty()) {
        SDL_Log("Provided Location is empty %s\n", __FUNCTION__) ;
    }
    skyTexLoc = location ;
    return *this ;
}

Background &Background::setGroundTexLocation(std::string location) {
    if (location.empty()) {
        SDL_Log("Provided Location is empty %s\n", __FUNCTION__) ;
    }
    groundTexLoc = location ;
    return *this ;
}

Background &Background::setRenderer(SDL_Renderer *&renderer) {
    rnd = renderer ;
    return *this ;
}

// Size Setter
Background &Background::setSkySize(int width, int height) {
    skyRect = {0, 0, (float) width, (float) height - 100} ;
    skyRect2 = {skyRect.w, skyRect.y, skyRect.w, skyRect.h} ;
    return *this ;
}

Background &Background::setGroundSize(int width, int height) {
    groundRect = {0, height - 100.f, (float) width, (float) 100} ;
    groundRect2 = {(float) groundRect.w, groundRect.y, groundRect.w, 100} ;
    return *this ;
}


// Getters

// Functions
bool Background::initTextures() {
    if (!rnd) { // Checks if renderer is not set
        SDL_Log("Provided renderer is null: [%s]\n", __FUNCTION__) ;
        bLoadFailed = true ;
        return false ;
    }
    
    // Loading Textures
    skyTex = IMG_LoadTexture(rnd, skyTexLoc.c_str()) ;
    if (!skyTex) {
        SDL_Log("Failed to Load sky Texture!: %s\n", SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    groundTex = IMG_LoadTexture(rnd, groundTexLoc.c_str()) ;
    if (!groundTex) {
        SDL_Log("Failed to Load Ground Texture!: %s\n", SDL_GetError()) ;
        bLoadFailed = true ;
        return false ;
    }
    
    return true ;    
}

void Background::update(float dt) {
    if (bLoadFailed) [[unlikely]] {
        SDL_Log("Failed to properly load textures!, SDL Error: %s\n", SDL_GetError()) ;
        return ;
    }
    
    
}

void Background::render() {
    // Rendering sky texture
    SDL_RenderTexture(rnd, skyTex, nullptr, &skyRect) ;
    SDL_RenderTexture(rnd, skyTex, nullptr, &skyRect2) ;
    
    // Rendering ground texture
    SDL_RenderTexture(rnd, groundTex, nullptr, &groundRect) ;
    SDL_RenderTexture(rnd, groundTex, nullptr, &groundRect2) ; 
}


// Player Class

// Functions
void Player::animate(float dt) {
    fTimer += dt ;
    if (fTimer >= FRAME_TIME) {
        fTimer -= FRAME_TIME ;
        
        sourceRect.x = W_FRAME * (nCurrentFrame % nFrameCount) ;
        if (nFrameCount != JUMPFALL_FRAMES) {
            nCurrentFrame = (nCurrentFrame + 1) % nFrameCount ;
        } else {
            if (nCurrentFrame < JUMPFALL_FRAMES - 1) {
                nCurrentFrame++ ;
            }
        }
    }
}

// Helper
void Player::setDefaultTex(SDL_Texture *&tex, const int &frame_count) {
    if (currentTex == tex) [[likely]] {
        return ;
    }
    
    currentTex = tex ;
    nCurrentFrame = 0 ;
    nFrameCount = frame_count ;
    fTimer = 0.f ;
    sourceRect.x = 0.f ;
}

// Constructor
Player::Player(SDL_Renderer *rnd, std::string runTexLocation, std::string jumpTexLocation, std::string jumpFallTexLocation, std::string idleTexLocation)
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

Player &Player::setRenderer(SDL_Renderer *&renderer) {
    rnd = renderer ;
    return *this ;
}


// Functions
void Player::jump(float dt) {
    fPlayerGravity = -710.f ;
}

void Player::render() {
    SDL_RenderTexture(rnd, currentTex, &sourceRect, &locationRect) ;
}

void Player::update(float dt) {
    if (bLoadFailed) [[unlikely]] {
        SDL_Log("Failed to properly load textures!, SDL Error: %s\n", SDL_GetError()) ;
        return ;
    }
    
    const bool *keys = SDL_GetKeyboardState(nullptr) ;
    bool bSpaceK = keys[SDL_SCANCODE_SPACE] ;
    if (bSpaceK && !pStatus.bJump) {
        pStatus.bJump = true ;
        pStatus.bIdle = false ;
        pStatus.bRun = false ;
        jump(dt) ;
    }
    
    if (pStatus.bIdle) {
        setDefaultTex(idleTex, IDLE_FRAMES) ;
    } else if (pStatus.bRun) {
        setDefaultTex(runTex, RUN_FRAMES) ;
    } else if (pStatus.bJump) {
        if (fPlayerGravity > 0.f) {
            setDefaultTex(jumpFallTex, JUMPFALL_FRAMES) ;
        } else {
            setDefaultTex(jumpTex, JUMP_FRAMES) ;
        }
    } else if (pStatus.bDeath) {
        // TODO: Implement Death animation and health in later
    }
    
    // Physics Gravity Player
    if (pStatus.bJump) {
        locationRect.y += fPlayerGravity * dt ;
        
        fPlayerGravity += fGravity * dt * 1.15f;
    }
    
    
    // Resetting Settings if player touched ground
    if (locationRect.y >= GROUND_Y) {
        locationRect.y = GROUND_Y ;
        fPlayerGravity = 0.f ;
        pStatus.bJump = false ;
        pStatus.bRun = true ;
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