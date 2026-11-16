#include "RunWick.hpp"
#include <format>

// Private Functions
// Initialization
bool RunWick::initWindow() {
    if (bInitFailed) return false ;
    
    if (wnd) SDL_DestroyWindow(wnd) ;
    wnd = SDL_CreateWindow(sWindow.name.c_str(), sWindow.nWidth, sWindow.nHeight, 0) ;
    if (!wnd) {
        logError("create window") ;
        return false ;
    }
    
    if (rnd) SDL_DestroyRenderer(rnd) ;
    rnd = SDL_CreateRenderer(wnd, nullptr) ;
    if (!rnd) {
        logError("create renderer") ;
        return false ;
    }
    
    SDL_SetDefaultTextureScaleMode(rnd, SDL_SCALEMODE_PIXELART) ;
    
    // TO-DO: Add Window icon using surface
    
    return true ;
}

// Player Variables Initialization
bool RunWick::initPlayer() {
    if (bInitFailed) {
        SDL_Log("Initialization Failed in previous functions, can't call %s\n", __FUNCTION__) ;
        return false ;
    }
    
    // Checking if renderer is exist or not
    if (!rnd) {
        SDL_Log("Renderer is not initialized - [%s]\n", __FUNCTION__) ;
        return false ;
    }
    
    player.setRenderer(rnd) ;
    
    // Loading Textures
    player.setIdleTexLocation("./data/character/knight/idle/idle.png")
        .setRunTexLocation("./data/character/knight/run/run.png")
        .setJumpTexLocation("./data/character/knight/jump/jump.png")
        .setJumpFallTexLocation("./data/character/knight/jump/jump_fall.png") ;
    // Checking for errors
    if (!player.initTextures()) {
        return false ;
    }
    
    return true ;
}

// Events
void RunWick::updateEvent(float dt) {
    // Event Loop
    while (SDL_PollEvent(&evn)) {
        switch(evn.type) {
            // Exit Event
            case SDL_EVENT_QUIT: {
                bRunning = false ;
                break ;
            }
            
            // Key Events
            case SDL_EVENT_KEY_DOWN: {
                SDL_Log("key= %s, scancode: %s\n",
                        SDL_GetKeyName(evn.key.key),
                        SDL_GetScancodeName(evn.key.scancode)
                );
                switch(evn.key.scancode) {
                    case SDL_SCANCODE_ESCAPE: {
                        bRunning = false ;
                        break ;
                    }
                    
                    // Enter key?
                    case SDL_SCANCODE_RETURN: {
                        
                        break ;
                    }
                    
                    default: break ;
                }
                break ;
            }
            
            default: break ;
        }
    }
    
    now = SDL_GetTicksNS() ;
    dt = (now - last) / 1'000'000'000.f ;
    player.update(dt) ;
}

// Render Update
void RunWick::updateRender() {
    SDL_RenderClear(rnd) ;
    
    player.render() ;
    
    SDL_RenderPresent(rnd) ;
    if (sWindow.nDefaultFPS > 0) SDL_Delay(1000 / sWindow.nDefaultFPS) ;
}

void RunWick::msgShow(MsgStats state, const char *message, const char *title) {
    using enum MsgStats ;
    switch(state) {
        case INFO: {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, nullptr) ;
            break ;
        }
        
        case ERROR: {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr) ;
            break ;
        }
        
        default: {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Undefined Reference", "MessageStats Fallback to Default, Unrecognized Enum Status", nullptr) ;
            break ;
        }
    }
}

void RunWick::logError(const char *cause, bool getSDLError) {
    SDL_Log("Failed to %s", std::format("{}{}", cause, getSDLError ? SDL_GetError() : "").c_str()) ;
}

// Constructor
RunWick::RunWick(const WindowSettings windowSettings) : sWindow(windowSettings) {
    
    printf("sWindow:\nWidth: %u\nHeight: %u\nName: %s\nFPS: %d\n", sWindow.nWidth, sWindow.nHeight, sWindow.name.c_str(), sWindow.nDefaultFPS) ;
    
}

// Destructor
RunWick::~RunWick() {
    
    if (surface) {
        SDL_DestroySurface(surface) ;
        surface = nullptr ;
    }
    
    if (rnd) {
        SDL_DestroyRenderer(rnd) ;
    }
    
    if (wnd) {
        SDL_DestroyWindow(wnd) ;
        wnd = nullptr ;
    }
    
    SDL_Quit() ;
}
// Getter
    
// Setter
void RunWick::setRenderColor(SDL_Color color) {
    if (!bInited || bInitFailed) {
        msgShow(MsgStats::ERROR, "Can't set the render color when uninitialized, call init() first/again", "UnInitialized") ;
        return ;
    }
    
    SDL_SetRenderDrawColor(rnd, color.r, color.g, color.b, color.a) ;
}
// Functions
void RunWick::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) { 
        bInitFailed = true ;
        msgShow(MsgStats::ERROR, "Failed to initialize SDL3 VIDEO & AUDIO", "Initialization Error") ;
        return ;
    }
    
    if (!initWindow()) {
        bInitFailed = true ;
    }
    
    if (!initPlayer()) {
        bInitFailed = true ;
    }
    
    bInited = true ;
    return ;
}

// Game Loop
void RunWick::run() {
    if (!bInited) {
        msgShow(MsgStats::ERROR, "Not initialized, call init() first", "UnInitialized") ;
        return ;
    }
    
    if (bInitFailed) {
        msgShow(MsgStats::ERROR, "Initialization failed, call init() again", "Init Failed") ;
        return ;
    }
    
    last = SDL_GetTicksNS() ;
    while(bRunning) {
        updateEvent(dt) ;
        last = now ;
        updateRender() ;
        
    }
}
