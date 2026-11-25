#ifndef GAME_TEXTURE_HPP
#define GAME_TEXTURE_HPP

#include <SDL3/SDL.h>
#include <random>
#include <string>
#include <vector>

namespace RW {
    namespace Base {
        enum class EnemyType {
            FLY = 0, GROUND = 1
        };
        
        struct Enemy {
            bool bAlive = true ;
            int xSource, ySource ;
            int nTotalFrame, nFrame = 0 ;
            float fTimer ;
            float x, y, velocity ;
            EnemyType type ;
            Enemy(const float startX, const float startY, const float velocity, EnemyType type, int totalFrames) noexcept
                : x(startX), y(startY), velocity(velocity), type(type), nTotalFrame(totalFrames) {}
        };
    }
    
    // Enemy 
    class Enemy {
    private:
        // Booleans
        bool bLoadFailed = false ;
        bool bInitFailed = false ;
        bool bInited = true ;
        
        // Array
        std::vector<Base::Enemy> enemies ;
        
        // SDL3 Variables
        // Renderer
        SDL_Renderer *rnd = nullptr ;
        
        // Textures
        SDL_Texture *enemySkyTex = nullptr ;
        SDL_Texture *enemyGroundTex = nullptr ;
        SDL_Texture *currentTexture = nullptr ;
        
        std::string enemySkyTexLocation ;
        std::string enemyGroundTexLocation ;
        
        // Animation Variables
        const int FLY_FRAMES = 8 ;
        const int GROUND_FRAMES = 8 ;
        
        const int W_FRAME = 150 ;
        const int H_FRAME = 150 ;
        
        
        // timer animation
        int nCurrentFrame = 0 ;
        int nTotalFrame = 1 ;
        float fTimer = 0.f ; 
        const float FRAME_TIME = 0.075f ;
        
        // Enemy Rectangle
        const int GROUND_Y = 350 ;
        
        std::uniform_int_distribution<int> flyDistLocation ;
        int FLY_Y = 200 ;
        const int START_POINT =  1010 ;
        
        SDL_FRect srcRect {0, 0, (float) W_FRAME, (float) H_FRAME} ;
        SDL_FRect dstRect {0, 0, W_FRAME * 1.5f, H_FRAME * 1.5f} ;
        
        
        // Random Generation
        std::mt19937 gen ;
        
        // Time Spawn
        std::uniform_real_distribution<float> spawnDist ;
        float fSpawnTimer = 0.f ;
        float fCurrentSpawn ;
        
        
        // Movement
        std::uniform_real_distribution<float> speedDist ;
        
        std::uniform_int_distribution<int> enemyTypeDist ;
        
        // Private Functions
        void animate(float dt) ;
        bool initTextures() ;
    public:
        // Constructor
        Enemy(SDL_Renderer *rnd = nullptr, std::string enemySkyLocation = "", std::string enemyGroundLocation = "") ;
        // Destructor
        ~Enemy() ;
        
        // Setters
        Enemy &setEnemySkyTexLocation(std::string location) ;
        Enemy &setEnemyGroundTexLocation(std::string location) ;
        
        Enemy &setRenderer(SDL_Renderer *&rnd) ;
        Enemy &setReserveArraySize(const int reserveCount) ;
        
        // Getters
        const SDL_FRect &getEnemyRect() ;
        
        // Functions
        bool init(std::random_device &rd, std::uniform_real_distribution<float> &spawnDist, std::uniform_real_distribution<float> speedDist);
        void update(float dt) ;
        void render() ;
    };
    
    class Background {
    private:
        // Booleans
        bool bLoadFailed = false ;
        
        // SDL Variables
        SDL_Renderer *rnd = nullptr ;
        
        // Textures
        SDL_Texture *skyTex = nullptr ;
        SDL_Texture *groundTex = nullptr ;
        
        std::string skyTexLoc ;
        std::string groundTexLoc ;
        
        // Texture Rectangle
        SDL_FRect skyRect ;
        SDL_FRect skyRect2 ;
        SDL_FRect groundRect ;
        SDL_FRect groundRect2 ;
        
        // Velocity
        const float V_SKY = 210.f ;
        const float V_BG = 190.f ;
        
        // Private Functions
        
    public:
        // Constructor
        Background(SDL_Renderer *rnd = nullptr, std::string skyTexLocation = "", std::string groundTexLocation = "") ;
        // Destructor
        ~Background() ;
        
        // Setters
        Background &setSkyTexLocation(std::string location) ;
        Background &setGroundTexLocation(std::string location) ;
        Background &setRenderer(SDL_Renderer *&rnd) ;
        // Size Setter
        Background &setSkySize(int width, int height) ;
        Background &setGroundSize(int width, int height) ;
        
        // Getters
        
        // Functions
        bool initTextures() ;
        void update(float dt) ;
        void render() ;
    };
    
    class Player {
    private:
        // Booleans
        bool bLoadFailed = false ;
        
        // Player Textures
        SDL_Texture *runTex = nullptr ;
        SDL_Texture *jumpTex = nullptr ;
        SDL_Texture *jumpFallTex = nullptr ;
        SDL_Texture *idleTex = nullptr ;
        SDL_Texture *currentTex = nullptr ;
        
        // Texture Locations
        std::string runTexLoc ;
        std::string jumpTexLoc ;
        std::string jumpFallTexLoc ;
        std::string idleTexLoc ;
        
        // Texture Frame Counts
        const int RUN_FRAMES = 10 ;
        const int JUMP_FRAMES = 3 ;
        const int JUMPFALL_FRAMES = 2 ;
        const int IDLE_FRAMES = 10 ;
        
        // Animtaion Variables
        int nFrameCount = 1 ;
        int nCurrentFrame = 0 ;
        float fTimer = 0.0f ;
        const float FRAME_TIME = 0.075f ;
        const int W_FRAME = 120 ;
        const int H_FRAME = 80 ;
        
        // SDL Variables
        SDL_Renderer *rnd = nullptr ;
        
        // Velocity & Speed
        const float fGravity = 1200.4f ;
        const float GROUND_Y = 380 ;
        float fPlayerGravity = 0 ;
        
        // Player Variables
        SDL_FRect sourceRect {0, 0, (float) W_FRAME, (float) H_FRAME};
        SDL_FRect locationRect {100, GROUND_Y, (float) W_FRAME * 1.5f, (float) H_FRAME * 1.5f} ;
        struct PlayerStatus {
            bool bIdle, bJump, bRun, bDeath ;
            // Constrcutor
            PlayerStatus(bool idle = true, bool jump = false, bool run = false, bool death = false) 
                : bIdle(idle), bJump(jump), bRun(run), bDeath(death) {}
        }; PlayerStatus pStatus ;
        
        // Functions
        void animate(float dt) ;
        
        // Helper
        void setDefaultTex(SDL_Texture *&tex, const int &frame_count) ;
    public:
        // Constructor
        Player(SDL_Renderer *rnd = nullptr, std::string runTexLocation = "", std::string jumpTexLocation = "", std::string jumpFallTexLocation = "", std::string idleTexLocation = "") ;
        // Destructor
        ~Player() ;
        
        // Getters
        
        // Setters
        Player &setRunTexLocation(std::string location) noexcept ;
        Player &setJumpTexLocation(std::string location) noexcept ;
        Player &setJumpFallTexLocation(std::string location) noexcept ;
        Player &setIdleTexLocation(std::string location) noexcept ;
        Player &setRenderer(SDL_Renderer *&rnd) ;
        
        // Functions
        void jump(float dt) ;
        void render() ;
        void update(float dt) ;
        bool initTextures() ;
        void start() ;
    };
}

#endif // GAME_TEXTURE_HPP