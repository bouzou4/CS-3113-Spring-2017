#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.h"
#include "phyVector.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "MyGame.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
    int w,h,comp;
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

void drawTexturedObj(ShaderProgram *program, Matrix *modelMatrix, GLuint *texture, float *Verts, float *texVerts) {
    program->setModelMatrix(*modelMatrix);
    modelMatrix->identity();
    
    glBindTexture(GL_TEXTURE_2D, *texture);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, Verts);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texVerts);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void moveObj(phyVector* vec, Matrix* objMatrix, Coord* objLoc) {
    objLoc->x += vec->getXVelocity();
    objLoc->y += vec->getYVelocity();
    objMatrix->Translate(objLoc->x, objLoc->y, 0.0);
}

bool boxCollision(Coord pos1, float height1, float width1, Coord pos2, float height2, float width2) {
    return (abs(pos1.x - pos2.x) * 2 < (width1 + width2)) && (abs(pos1.y - pos2.y) * 2 < (height1 + height2));
}

int main(int argc, char *argv[])
{
    initScene();
    
    const Uint8* keys;
    SDL_Event event;
    bool done = false;
    
    float lastFrameTicks = 0.0f;
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix viewMatrix;
    
    Matrix ufoMatrix;
    phyVector ufoVector;
    Coord ufoPos;
    ufoPos.x = 0;
    ufoPos.y = 0;
    
    Matrix ceilingMatrix;
    Coord ceiling;
    ceiling.x = 0;
    ceiling.y = 0;
    
    Matrix grassMatrix;
    Coord ground;
    ground.x = 0;
    ground.y = -1.75;
    
    Matrix skyMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    //load all relevant textures
    GLuint ufoTexture = LoadTexture(RESOURCE_FOLDER"ufo.png");
    GLuint grassTexture = LoadTexture(RESOURCE_FOLDER"grass.png");
    GLuint skyTexture = LoadTexture(RESOURCE_FOLDER"sky.png");
    
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
        
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        float vertices[] = {-3.55, -2.0, 3.55, -2.0, 3.55, -1.5, -3.55, -2.0, 3.55, -1.5, -3.55, -1.5};
        float vertices2[] = {-3.55, -1.5, 3.55, -1.5, 3.55, 2.0, -3.55, -1.5, 3.55, 2.0, -3.55, 2.0};
        float vertices3[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        
        drawTexturedObj(&program, &grassMatrix, &grassTexture, vertices, texCoords);
        drawTexturedObj(&program, &skyMatrix, &skyTexture, vertices2, texCoords);
        drawTexturedObj(&program, &ufoMatrix, &ufoTexture, vertices3, texCoords);
        
        if(keys[SDL_SCANCODE_LEFT]) {
            ufoVector.setVelocity(0.03);
            ufoVector.setAngle(180);
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            ufoVector.setVelocity(0.03);
            ufoVector.setAngle(0);
        }
        if (keys[SDL_SCANCODE_UP]) {
            ufoVector.setVelocity(0.03);
            ufoVector.setAngle(90);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            ufoVector.setVelocity(0.03);
            ufoVector.setAngle(270);
        }
        
        if(boxCollision(ufoPos, 1, 1, ground, 0.5, 7.1) || ((ufoPos.y + 0.5) > 2)) {
            ufoVector.flipY();
        }
        if (((ufoPos.x + 0.5) > 3.55) || ((ufoPos.x - 0.5) < -3.55)) {
            ufoVector.flipX();
        }
        
        
        moveObj(&ufoVector, &ufoMatrix, &ufoPos);
        
        
        //switch to game window
        SDL_GL_SwapWindow(displayWindow);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_E) {
                    std::cout << "key e pressed" << std::endl;
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    SDL_Quit();
    return 0;
}
