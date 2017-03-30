#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <map>
#include <vector>
#include "pugixml.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "physObject.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER std::string(SDL_GetBasePath()) +
#endif

#define FIXED_TIMESTEP 0.0166666f // 60 FPS (1.0f/60.0f)
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;

GLuint LoadTexture(const std::string& filePath, int& h, int& w) {
    int comp;
    unsigned char* image = stbi_load(filePath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
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
    float width = (size*-0.5)-((spacing*text.size()-1)+(text.size()*size))/2;
    modelMatrix.Translate(width, 0, 0);
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

void updatePhysics(float fixedElapsed, std::vector<simplePhysObject*>& objects, simplePhysObject* ufo, gameObject* ground) {
    for (size_t x = 0; x < objects.size(); x++) {
        if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while (fixedElapsed >= FIXED_TIMESTEP ) {
            fixedElapsed -= FIXED_TIMESTEP;
            objects[x]->physicsStep(FIXED_TIMESTEP);
        }
        objects[x]->physicsStep(fixedElapsed);
    }
    
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
    
    ShaderProgram program((RESOURCE_FOLDER"vertex_textured.glsl").c_str(), (RESOURCE_FOLDER"fragment_textured.glsl").c_str());
    
    std::vector<gameObject*> objects;
    std::vector<simplePhysObject*> physObjects;
    std::vector<gameObject*> enemies;
    
    std::map<std::string, SpriteSheetTexture*> shipSprites;
    GLuint shipSpriteSheet = LoadTexture(RESOURCE_FOLDER"texts/shipSprites.png", objHeight, objWidth);
    loadSpriteSheet(shipSprites, "shipSprites");
    
    std::map<size_t, SpriteSheetTexture*> myFontSprites;
    GLuint myFontSheet = LoadTexture(RESOURCE_FOLDER"texts/myFont.png", objHeight, objWidth);
    loadSpacedSpriteSheet(myFontSprites, objHeight, objWidth, 16, 16);
    Matrix textMatrix;
    
    for (size_t x = 0; x < 10; x++) {
        enemies.push_back(new simplePhysObject(-3 + (0.5 * x), 1.5, shipSpriteSheet, shipSprites[("enemyBlack" + std::to_string((rand() % 5) + 1))]));
        objects.push_back(enemies[x]);
        enemies[x]->setSize(0.4);
    }
    gameObject sky(0, 0.25, int(LoadTexture(RESOURCE_FOLDER"sky.png", objHeight, objWidth)), objHeight, objWidth);
    objects.push_back(&sky);
    sky.setSize(7.5);
    gameObject ground(0, -1.75, int(LoadTexture(RESOURCE_FOLDER"grass.png", objHeight, objWidth)), objHeight, objWidth);
    objects.push_back(&ground);
    ground.skewWidth(16.0);
    ground.setSize(0.5);
    simplePhysObject ufo(shipSpriteSheet, shipSprites["ufo"]);
    objects.push_back(&ufo);
    physObjects.push_back(&ufo);
    //ufo.setSize(0.3);
    physObject ufo2(&ufo);
    objects.push_back(&ufo2);
    physObjects.push_back(&ufo2);
    ufo2.getObjectAccel()->setAccel(1);
    ufo2.getObjectAccel()->setAngle(270);
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
        
        //Collision Rules (need to be moved to updatePhysics())
        if(ufo.collidesWith(&ground) || ((ufo.getPos()->getY() + ufo.getHeight()/2) > 2)) {
            ufo.getObjVelo()->flipY();
        }
        if (((ufo.getPos()->getX() + ufo.getWidth()/2) > 3.55) || ((ufo.getPos()->getX() - ufo.getWidth()/2) < -3.55)) {
            ufo.getObjVelo()->flipX();
        }
        if(ufo2.collidesWith(&ground)) {
            ufo2.getObjVelo()->flipY();
        }
        if (((ufo2.getPos()->getX() + 0.25) > 3.55) || ((ufo2.getPos()->getX() - 0.25) < -3.55)) {
            ufo2.getObjVelo()->flipX();
        }
        updatePhysics(elapsed, physObjects, &ufo, &ground);
        
        /* draw all objects */
        for (std::vector<gameObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++) {(*itr)->drawObj(&program);}
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
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    ufo.getObjVelo()->setVelocity(1);
                    ufo.getObjVelo()->setAngle(290);
                }
                if((event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) || (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)) {
                    ufo.getObjVelo()->setVelocity(1E-16f);
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    SDL_Quit();
    return 0;
}
