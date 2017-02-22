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
#include "physObject.h"

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
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix viewMatrix;
    
    physObject ufo;
    physObject box1;
    physObject box2;
    gameObject sky(0, 0.25);
    gameObject ground(0, -1.75);
    
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
        
        float groundVerts[] = {-3.55, -0.25, 3.55, -0.25, 3.55, 0.25, -3.55, -0.25, 3.55, 0.25, -3.55, 0.25};
        float skyVerts[] = {-3.55, -1.75, 3.55, -1.75, 3.55, 1.75, -3.55, -1.75, 3.55, 1.75, -3.55, 1.75};
        float ufoVerts[] = {-0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25};
        float boxVerts[] ={-0.2, -0.5, 0.2, -0.5, 0.2, 0.5, -0.2, -0.5, 0.2, 0.5, -0.2, 0.5};
        
        drawTexturedObj(&program, sky.getMatrix(), &skyTexture, skyVerts, texCoords);
        drawTexturedObj(&program, ground.getMatrix(), &grassTexture, groundVerts, texCoords);
        drawTexturedObj(&program, ufo.getMatrix(), &ufoTexture, ufoVerts, texCoords);
        drawTexturedObj(&program, box1.getMatrix(), &ufoTexture, boxVerts, texCoords);
        drawTexturedObj(&program, box2.getMatrix(), &ufoTexture, boxVerts, texCoords);
        
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
        
        if(boxCollision(*ufo.getPos(), .5, .5, *ground.getPos(), 0.5, 7.1) || ((ufo.getPos()->getY() + 0.25) > 2)) {
            ufo.getVector()->flipY();
        }
        if (((ufo.getPos()->getX() + 0.25) > 3.55) || ((ufo.getPos()->getX() - 0.25) < -3.55)) {
            ufo.getVector()->flipX();
        }
        
        sky.drawObj();
        ground.drawObj();
        ufo.moveObj();
        
        
        //switch to game window
        SDL_GL_SwapWindow(displayWindow);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_E) {
                    std::cout << "key e pressed" << std::endl;
                    ufo.getVector()->rotateCW();
                }
                else if(event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    std::cout << "key q pressed" << std::endl;
                    ufo.getVector()->rotateCCW();
                }
                if((event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) || (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)) {
                    std::cout << "key shift pressed" << std::endl;
                    ufo.getVector()->setVelocity(0);
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    SDL_Quit();
    return 0;
}
