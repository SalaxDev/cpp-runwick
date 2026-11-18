#ifndef RUNWICK_HPP
#define RUNWICK_HPP

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


#include "GameTexture.hpp"
#include "Settings.hpp"

class RunWick {
private:
    // Private Variables
    
    // Booleans
    bool bInited = false ;
    bool bInitFailed = false ;
    bool bRunning = true ;
    bool bStart = false ;
    
    // MessageBox Settings
    enum class MsgStats {
        INFO, ERROR
    }; 
    
    // Window Settings Structure, for holding initial variables building a window
    WindowSettings sWindow ;
    
    // SDL3 Variables
    SDL_Window *wnd = nullptr ;
    SDL_Renderer *rnd = nullptr ;
    
    // Images & Textures
    SDL_Surface *surface = nullptr ;
    
    // Player & Background Class
    RW::Player player ;
    RW::Background bg ;
    
    // Event Handler
    SDL_Event evn ;
    
    // Timer & dt
    Uint64 last, now;
    float dt ;
    
    // Private Functions
    bool initWindow() ; // Initialization
    bool initPlayer() ; // Player Variables Initialization
    bool initBackground() ; // Background Variables Initialization
    void updateEvent(float dt) ; // Events
    void updateRender() ;
    
    // Loggers
    void msgShow(MsgStats state, const char *message, const char *title) ;
    void logError(const char *cause, bool getSDLError = true) ;
public:
    // Constructor
    RunWick(WindowSettings windowSettings = {}) ;
    // Destructor
    ~RunWick() ;
    
    // Getter
    
    // Setter
    void setRenderColor(SDL_Color color) ;
    // Functions
    void init() ;
    void run() ;
};

#endif // RUNWICK_HPP
