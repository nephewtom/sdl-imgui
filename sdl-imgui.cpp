#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "constants.h"
#include "ltexture.h"
#include "dot.h"

void initSDL() {    
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        exit(-1);
    }
}

void initIMG() {
//Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        exit(-1);
    }
}

SDL_Window* createWindow() {
    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *window = SDL_CreateWindow("SDL2+ImGui",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);    
    if (window == 0) {
        exit(-1);
    }
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( gRenderer == NULL ) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        exit(-1);
    }
    //printf("init - gRenderer:0x%x\n", gRenderer);
    
    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    return gRenderer;
}

void loadMedia(LTexture& texture)
{
    if( !texture.loadFromFile( "./res/dot.bmp" ) ) {
        printf( "Failed to load dot texture!\n" );
        exit(-1);
    }
}

SDL_Renderer* gRenderer;

int main(int, char**)
{
    initSDL();
    initIMG();
    SDL_Window* window = createWindow();
    gRenderer = createRenderer(window);

    ImGui_ImplSdlGL2_Init(window);
    glewInit();
    
    bool show_test_window = false;
    bool parameters_window = true;
    ImVec4 background_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 rect_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

    LTexture gDotTexture;    
    loadMedia(gDotTexture);

    int dw = 20; int dh = 20;
    Dot dot(10, 10, dw, dh);
    Dot otherDot(10, 100, dw, dw);

    SDL_Rect wall;
    wall.x = 300;
    wall.y = 40;
    wall.w = 40;
    wall.h = 400;

    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = dw;
    clip.h = dh;
    
    ImVec2 initSize = ImVec2(400, 400);
    bool done = false;
    bool imgui = true;
    while (!done)
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdlGL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;

            
            if( event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_ESCAPE) done = true;
            if( event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_i) imgui = !imgui;
            
            dot.handleEvent( event );
        }

        // ImGui stuff
        ImGui_ImplSdlGL2_NewFrame(window);
        if (imgui)
        {
            ImGui::Begin("Parameters", &parameters_window);
            ImGui::SetWindowSize("Parameters", initSize);
            ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Wall", "", true, true))
            {
                ImGui::SliderInt("x", &wall.x, 100, 1000);
                ImGui::SliderInt("y", &wall.y, 10, 700);
                ImGui::SliderInt("w", &wall.w, 10, 1000);
                ImGui::SliderInt("h", &wall.h, 10, 700);
                ImGui::ColorEdit3("Rectangle color", (float*)&rect_color);
            }
            if (ImGui::CollapsingHeader("Dot", "", true, true))
            {
                ImGui::SliderInt("dx", &dot.mPosX, 10, 1000);
                ImGui::SliderInt("dy", &dot.mPosY, 10, 700);
                ImGui::Text("collider.x: %d", dot.collider.x);
                ImGui::Text("collider.y: %d", dot.collider.y);
                ImGui::SliderInt("Velocity", &dot.vel, 1, 32);
            }
            if (ImGui::CollapsingHeader("Help", "", true, true)) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::ColorEdit3("Background color", (float*)&background_color);
                if (ImGui::Button("Samples Window")) show_test_window ^= 1;
            }
            ImGui::End();

            // Most of the sample code is in ImGui::ShowTestWindow().
            if (show_test_window) {
                ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
                ImGui::ShowTestWindow(&show_test_window);
            }
        }


        //Move the dot and check collision
        dot.move( wall, otherDot.collider );

        //Clear screen
        SDL_SetRenderDrawColor (gRenderer, background_color.x * 255.0f, background_color.y * 255.0f, background_color.z * 255.0f, background_color.w * 255.0f);
        SDL_RenderClear( gRenderer );

        //Render wall
        SDL_SetRenderDrawColor (gRenderer, rect_color.x * 255.0f, rect_color.y * 255.0f, rect_color.z * 255.0f, rect_color.w * 255.0f);
        SDL_RenderFillRect( gRenderer, &wall );
				
        //Render dots
        dot.render(gDotTexture, clip);
        otherDot.render(gDotTexture, clip);


        // Last calls before end of mainLoop
        glUseProgram(0);
        ImGui::Render();
        SDL_RenderPresent( gRenderer );
    }

    // Cleanup
    gDotTexture.free();
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( window );
    ImGui_ImplSdlGL2_Shutdown();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
