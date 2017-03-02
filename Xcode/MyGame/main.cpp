#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <map>
#include <math.h>
#include "pugixml.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "physObject.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "MyGame.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath, int& h, int& w) {
    int comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    if(image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(image);
    return retTexture;
}

void loadSpriteSheet(std::map<std::string, SpriteSheetTexture*>& textures, const GLuint& texID, float texSize) {
    pugi::xml_document xFile;
    xFile.load_file(RESOURCE_FOLDER"texts/sprites.xml");
    std::string tempName = "";
    float size = atof(xFile.first_child().attribute("size").value());
    SpriteSheetTexture* tSprite;
    
    for (pugi::xml_node_iterator xIt = xFile.first_child().begin(); xIt != xFile.first_child().end(); ++xIt) {
        tempName = xIt->attribute("name").value();
        tempName = tempName.substr(0, tempName.find_last_of("."));
        
        tSprite = new SpriteSheetTexture(texID, atof(xIt->attribute("x").value())/size, atof(xIt->attribute("y").value())/size, atof(xIt->attribute("width").value())/size, atof(xIt->attribute("height").value())/size, texSize);
        textures[tempName] = tSprite;
        std::cout << tempName << std::endl;
    }
}

void initScene() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool boxCollision(Coord& pos1, float height1, float width1, Coord& pos2, float height2, float width2) {
    float top1 = (pos1.getY() + height1/2);
    float top2 = (pos2.getY() + height2/2);
    float right1 = (pos1.getX() + width1/2);
    float right2 = (pos2.getX() + width2/2);
    float bottom1 = (pos1.getY() - height1/2);
    float bottom2 = (pos2.getY() - height2/2);
    float left1 = (pos1.getX() - width1/2);
    float left2 = (pos2.getX() - width2/2);
    
    bool top = ((top1 > bottom2) && (top1 < top2));
    bool right = ((right1 > left2) && (right1 < right2));
    bool bottom = ((bottom1 > bottom2) && (bottom1 < top2));
    bool left = ((right1 > right2) && (left1 < right2));
    
    
    if ((top || bottom) && (left || right)) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    initScene();
    
    const Uint8* keys;
    SDL_Event event;
    bool done = false;
    
    std::map<std::string, SpriteSheetTexture*> sprites;
    float lastFrameTicks = 0.0f;
    int objWidth, objHeight;
    Matrix projectionMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    float pixelRatioX(7.1/1280.0);
    float pixelRatioY(4.0/720.0);
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    GLuint spritesheet = LoadTexture(RESOURCE_FOLDER"texts/sprites.png", objWidth, objHeight);
    loadSpriteSheet(sprites, spritesheet, 1);
    gameObject ship(spritesheet, sprites["enemyBlack4"]);
    gameObject sky(0, 0.25, int(LoadTexture(RESOURCE_FOLDER"sky.png", objHeight, objWidth)), objHeight, objWidth);
    sky.setSize(7.5);
    gameObject ground(0, -1.75, int(LoadTexture(RESOURCE_FOLDER"grass.png", objHeight, objWidth)), objHeight, objWidth);
    ground.skewWidth(16.0);
    ground.setSize(0.5);
    physObject ufo(int(LoadTexture(RESOURCE_FOLDER"ufo.png", objHeight, objWidth)), objHeight, objWidth);
    ufo.setSize(0.5);
    gameObject cursor(int(LoadTexture(RESOURCE_FOLDER"cursor.png", objHeight, objWidth)), objHeight, objWidth);
    cursor.setSize(0.3);
    
    glUseProgram(program.programID);
                          
    while (!done) {
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        keys = SDL_GetKeyboardState(NULL);
        
        glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        
        //Keyboard Input
        if(keys[SDL_SCANCODE_LEFT]) {
            ufo.getVector()->setVelocity(0.03);
            ufo.getVector()->setAngle(180);
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            ufo.getVector()->setVelocity(0.03);
            ufo.getVector()->setAngle(0);
        }
        if (keys[SDL_SCANCODE_UP]) {
            ufo.getVector()->setVelocity(0.03);
            ufo.getVector()->setAngle(90);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            ufo.getVector()->setVelocity(0.03);
            ufo.getVector()->setAngle(270);
        }
        
        //Collision Rules
        if(boxCollision(*ufo.getPos(), .5, .5, *ground.getPos(), 0.5, 8.0) || ((ufo.getPos()->getY() + 0.25) > 2)) {
            ufo.getVector()->flipY();
        }
        if (((ufo.getPos()->getX() + 0.25) > 3.55) || ((ufo.getPos()->getX() - 0.25) < -3.55)) {
            ufo.getVector()->flipX();
        }
        
        sky.drawObj(&program);
        ground.drawObj(&program);
        ship.drawObj(&program);
        ufo.moveObj(&program);
        cursor.drawObj(&program);
        
        
        //switch to game window
        SDL_GL_SwapWindow(displayWindow);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if(event.type == SDL_MOUSEMOTION) {
                cursor.getPos()->setX((event.motion.x * pixelRatioX) - 3.45);
                cursor.getPos()->setY(((event.motion.y * pixelRatioY) - 1.9)*-1);
            }
            else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_E) {
                    ufo.getVector()->rotateCW();
                }
                else if(event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    ufo.getVector()->rotateCCW();
                }
                if((event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) || (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)) {
                    ufo.getVector()->setVelocity(0);
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    SDL_Quit();
    return 0;
}
