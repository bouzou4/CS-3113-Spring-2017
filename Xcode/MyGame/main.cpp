#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <map>
#include <vector>
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

void loadSpriteSheet(std::map<std::string, SpriteSheetTexture*>& sprites, const std::string& name) {
    pugi::xml_document xFile;
    xFile.load_file((RESOURCE_FOLDER"texts/" + name + ".xml").c_str());
    std::string tempName = "";
    float height = atof(xFile.first_child().attribute("height").value());
    float width = atof(xFile.first_child().attribute("width").value());
    SpriteSheetTexture* tSprite;
    
    for (pugi::xml_node_iterator xIt = xFile.first_child().begin(); xIt != xFile.first_child().end(); ++xIt) {
        tempName = xIt->attribute("name").value();
        tempName = tempName.substr(0, tempName.find_last_of("."));
        
        tSprite = new SpriteSheetTexture(atof(xIt->attribute("x").value())/width, atof(xIt->attribute("y").value())/height, atof(xIt->attribute("height").value())/height, atof(xIt->attribute("width").value())/width, width);
        sprites[tempName] = tSprite;
    }
}

void loadSpacedSpriteSheet(std::map<size_t, SpriteSheetTexture*>& sprites, const GLfloat& height, const GLfloat& width, const GLfloat& rows, const GLfloat& columns) {
    SpriteSheetTexture* tSprite;
    size_t index = 0;
    const float sprHeight = height/rows;
    const float sprWidth = width/columns;
    const float tempHeight = sprHeight/height;
    const float tempWidth = sprWidth/width;
    
    for (size_t y = 0; y < columns; y++) {
        for (size_t x = 0; x < rows; x++) {
            tSprite = new SpriteSheetTexture(x*tempWidth, y*tempHeight, tempHeight, tempWidth, 1.0);
            sprites[index] = tSprite;
            index++;
        }
    }
}

void DrawText(ShaderProgram* program, Matrix& modelMatrix, std::map<size_t, SpriteSheetTexture*>& font, const GLuint& fontSheet, const std::string& text, const GLfloat& size, const GLfloat& spacing) {
    program->setModelMatrix(modelMatrix);
    modelMatrix.identity();
    int vertLen = 12 * text.size();
    std::vector<float> texCoordData;
    std::vector<float> vertexData;
    texCoordData.resize(vertLen);
    for(int i=0; i < text.size(); i++) {
        int code = (int)text[i];
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size),  -0.5f * size,
            ((size+spacing) * i) + (0.5f * size),   0.5f * size,
            ((size+spacing) * i) + (-0.5f * size),  0.5f * size,
            ((size+spacing) * i) + (0.5f * size),   0.5f * size,
            ((size+spacing) * i) + (-0.5f * size),  -0.5f * size,
            ((size+spacing) * i) + (0.5f * size),   -0.5f * size,
        });
        memcpy((texCoordData.data() + (12*i)), font[code]->getTexCoordsPtr(), 48);
    }
    glBindTexture(GL_TEXTURE_2D, fontSheet);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, vertLen/2);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    
}

void initScene() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1600, 900);
    
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
    
    float lastFrameTicks = 0.0f;
    int objWidth, objHeight;
    Matrix projectionMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    float pixelRatioX(7.1/1600.0);
    float pixelRatioY(4.0/900.0);
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    std::vector<gameObject*> objects;
    std::vector<gameObject*> enemies;
    
    std::map<std::string, SpriteSheetTexture*> shipSprites;
    std::map<size_t, SpriteSheetTexture*> myFontSprites;
    
    GLuint myFontSheet = LoadTexture(RESOURCE_FOLDER"texts/myFont.png", objHeight, objWidth);
    Matrix textMatrix;
    loadSpacedSpriteSheet(myFontSprites, objHeight, objWidth, 16, 16);
    GLuint shipSpriteSheet = LoadTexture(RESOURCE_FOLDER"texts/shipSprites.png", objHeight, objWidth);
    loadSpriteSheet(shipSprites, "shipSprites");
    
    for (size_t x = 0; x < 40; x++) {
        enemies.push_back(new physObject(-3 + ((x % 10) * 0.5), 1.5 - (int(x/10) * 0.5), shipSpriteSheet, shipSprites[("enemyBlack" + std::to_string((rand() % 5) + 1))]));
        objects.push_back(enemies[x]);
        enemies[x]->setSize(0.4);
    }
    gameObject sky(0, 0.25, int(LoadTexture(RESOURCE_FOLDER"sky.png", objHeight, objWidth)), objHeight, objWidth);
    sky.setSize(7.5);
    gameObject ground(0, -1.75, int(LoadTexture(RESOURCE_FOLDER"grass.png", objHeight, objWidth)), objHeight, objWidth);
    ground.skewWidth(16.0);
    ground.setSize(0.5);
    physObject ufo(shipSpriteSheet, shipSprites["ufo"]);
    ufo.setSize(0.3);
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
        
        //Ufo Controls
        if(keys[SDL_SCANCODE_LEFT]) {
            ufo.translateX(-0.03);
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            ufo.translateX(0.03);
        }
        if (keys[SDL_SCANCODE_UP]) {
            ufo.translateY(0.03);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            ufo.translateY(-0.03);
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
        for (std::vector<gameObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++) {
            (*itr)->drawObj(&program);
        }
        ufo.moveObj(&program);
        DrawText(&program, textMatrix, myFontSprites, myFontSheet, "just some text", 0.25, -0.12);
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
