// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <functional>
#include <cmath>

struct chunk {
    float x, y;
};

int main(int argv, char* argc[])
{

    std::function<bool(Uint8**,int x, int y)> l = [](Uint8** grid, int x, int y) -> bool {return false;};
    std::cout << "Hola mundo" << std::endl;
    
    int ww = 600;
    int wh = 600;
    SDL_Window* window = SDL_CreateWindow("Wave function collapse implementation Mattekollo 2023",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);


    float dt;
    // wfc stuff
    const int resolution = 100;
    Uint8 grid[resolution][resolution];
    
    
    //SDL_Surface* screen = SDL_CreateRGBSurface(0, ww, wh, 8, 0, 0, 0, SDL_ALPHA_OPAQUE);

    
    bool running = true;
    SDL_Event ev;
    while (running) {

        int startTime = SDL_GetTicks();

        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        dt = (SDL_GetTicks() - startTime) / 1000.0f;

    }

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
