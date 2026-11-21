#include "RunWick.hpp"
#include <format>
#include <stdexcept>

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
    
    // Load Icon
    surface = SDL_LoadSurface("./data/logo/cpp_logo.png") ;
    if (surface) {
        SDL_SetWindowIcon(wnd, surface) ;
        
        // Cleanup
        SDL_DestroySurface(surface) ;
        surface = nullptr ;
    } else {
        logError("load icon") ;
        // Won't return false (minor problem)
    }
    
    
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

// Background Variables Initialization
bool RunWick::initBackground() {
    if (bInitFailed) {
        SDL_Log("Initialization Failed in previous functions, can't call %s\n", __FUNCTION__) ;
        return false ;
    }
    
    // Checking if renderer is exist or not
    if (!rnd) {
        SDL_Log("Renderer is not initialized - [%s]\n", __FUNCTION__) ;
        return false ;
    }
    
    bg.setRenderer(rnd) ;
    
    bg.setGroundTexLocation("./data/background/ground.png")
        .setSkyTexLocation("./data/background/sky.png") ;
    
    if (!bg.initTextures()) {
        return false ;
    }
    
    return true ;
}

// Font Initialization
bool RunWick::initFont() {
    // Todo: Add font load, check for errors and write "Press "Enter" Enter to Start" and diable the text
    
    font = TTF_OpenFont("./data/fonts/KaushanScript-Regular.otf", FONT_SIZE) ;
    if (!font) {
        logError("Load Font") ;
        return false ;
    }
    
    surface = TTF_RenderText_Blended(font, startMessage.c_str(), startMessage.length(), WHITE) ;
    if (!surface) {
        logError("load font texture") ;
        return false ;
    }
    
    fontTex = SDL_CreateTextureFromSurface(rnd, surface) ;
    // Cleanup
    SDL_DestroySurface(surface) ;
    surface = nullptr ;
    
    // Center Alignment
    float text_w, text_h ;
    SDL_GetTextureSize(fontTex, &text_w, &text_h) ;
    int wnd_w, wnd_h ;
    SDL_GetWindowSize(wnd, &wnd_w, &wnd_h) ;
    
    fontRect = {(wnd_w - text_w) / 2.f, (wnd_h - text_h) / 2.f, text_w, text_h} ;
    
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
                        if (!bStart) {
                            player.start() ;
                            bStart = true ;
                            bShowStartMsg = false ;
                        }
                        
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
    if (dt >= 0.25f) [[unlikely]] { dt = 0.25 ; }
    
    player.update(dt) ;
    if (bStart) [[likely]] {
        bg.update(dt) ;
    }
}

// Render Update
void RunWick::updateRender() {
    SDL_RenderClear(rnd) ;
    
    bg.render() ;
    player.render() ;
    if (bShowStartMsg) [[unlikely]] {
        SDL_RenderTexture(rnd, fontTex, nullptr, &fontRect) ;
    }
        
    
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
        
        case ERROR: [[likely]] {
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
    SDL_Log("Failed to %s", std::format("{}: {}", cause, getSDLError ? SDL_GetError() : "").c_str()) ;
}

// Constructor
RunWick::RunWick(const WindowSettings windowSettings) : sWindow(windowSettings) {
    
    printf("sWindow:\nWidth: %u\nHeight: %u\nName: %s\nFPS: %d\n", sWindow.nWidth, sWindow.nHeight, sWindow.name.c_str(), sWindow.nDefaultFPS) ;
    
}

// Destructor
RunWick::~RunWick() {
    
    if (font) {
        TTF_CloseFont(font) ;
        font = nullptr ;
    }
    
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
    
    TTF_Quit() ;
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
    if (!SDL_Init(SDL_INIT_VIDEO)) { 
        msgShow(MsgStats::ERROR, "Failed to initialize SDL3 VIDEO & AUDIO, Fallback to SW", "Initialization Error") ;
        
        // Experimental, not SW tested (no gpu)
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software") ;
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            msgShow(MsgStats::ERROR, "Failed to Downgrade to SW", "Downgrade Failure") ;
            throw std::runtime_error("Downgrade SW Failure") ;
        }
    }
    
    // Initializing Audio
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("No detected Audio or sound driver, game runs without sounds [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bNoAudio = true ;
    }
    
    if (!TTF_Init()) {
        SDL_Log("Failed to Initialize TTF! [%s]: %s\n", __FUNCTION__, SDL_GetError()) ;
        bInitFailed = true ;
    }
    
    if (!initWindow()) {
        bInitFailed = true ;
    }
    
    if (!initPlayer()) {
        bInitFailed = true ;
    }
    
    if (!initBackground()) {
        bInitFailed = true ;
    }
    
    if (!initFont()) {
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
    
    
    bg.setGroundSize(sWindow.nWidth, sWindow.nHeight) ;
    bg.setSkySize(sWindow.nWidth, sWindow.nHeight) ;
    last = SDL_GetTicksNS() ;
    while(bRunning) [[likely]] {
        updateEvent(dt) ;
        last = now ;
        updateRender() ;
        
    }
}
