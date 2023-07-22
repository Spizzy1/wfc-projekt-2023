// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <functional>
#include <cmath>
#include <vector>
#include <tuple>

#define resolution 100
using std::vector;
using std::tuple;
using std::function;
using std::get;

struct Chunk {
    float x, y;
};



vector<tuple<int, int>> surroundingTiles(vector<vector<Uint8>> grid, int x, int y) {
    vector<tuple<int, int>> coords;
    for (int iy = -1; iy <= 1; iy++) {
        for (int ix = -1; ix <= 1; ix++) {
            if (iy + y > grid.size() || y + iy < 0 || x + ix > grid[y + iy].size() || x + ix < 0 || (ix == 0 && iy == 0)) {}
            else {
                coords.push_back({ iy + y, x + ix });
            }
        }
    }
    return coords;
}


int main(int argv, char* argc[])
{

    vector<function<bool(vector<vector<Uint8>>, int x, int y)>> tile_functions = { [](vector<vector<Uint8>> grid, int x, int y) -> bool {return true; } };
    std::cout << "Hola mundo" << std::endl;
    
    int ww = 600;
    int wh = 600;
    SDL_Window* window = SDL_CreateWindow("Wave function collapse implementation Mattekollo 2023",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    // create texture
    const unsigned int texWidth = ww;
    const unsigned int texHeight = wh;
    SDL_Texture* winTex = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight );
    vector<uint32_t> pixels(texWidth * texHeight * 4, 0);

    float dt;
    // wfc stuff
    vector<vector<Uint8>> grid(resolution, vector<Uint8>(resolution));
    vector<vector<Uint8>> neighbourhoods = {};
    float tile_length = (float)ww / resolution;

    int types = 2;


    
    int start_c = resolution / 2;
    vector<tuple<int, int>> surrounding_tiles = surroundingTiles(grid, start_c, start_c);
    std::cout << get<0>(surrounding_tiles[0]) << std::endl;
    grid[start_c][start_c] = rand() % types + 1;
    vector<vector<bool>> possibleList(resolution * resolution, vector<bool>(true, 2));

    vector<tuple<int, int>> tilesToCheck = {{start_c, start_c}};

    while (tilesToCheck.data()) {

        tuple<int, int> tile = tilesToCheck[0];
        tilesToCheck.pop_back();
        vector<vector<bool>> nbh = {
            possibleList[(get<0>(tile)-1)*resolution + get<1>(tile)-1], possibleList[(get<0>(tile)-1)*resolution + get<1>(tile)], possibleList[(get<0>(tile)-1)*resolution + get<1>(tile)+1],
            possibleList[(get<0>(tile))*resolution + get<1>(tile)-1], possibleList[(get<0>(tile))*resolution + get<1>(tile)], possibleList[(get<0>(tile))*resolution + get<1>(tile)+1],
            possibleList[(get<0>(tile)+1)*resolution + get<1>(tile)-1], possibleList[(get<0>(tile)+1)*resolution + get<1>(tile)], possibleList[(get<0>(tile)+1)*resolution + get<1>(tile)+1]
        };
        vector<tuple<int, int>> sTiles = surroundingTiles(grid, get<1>(tile), get<0>(tile));

        bool reduced = false;
        for (int it = 0; it < nbh[4].size(); it++) {
            bool works = false;
            if (nbh[4][it]) {
                for (int i = 0; i < neighbourhoods.size(); i++) {

                    if (it == neighbourhoods[i][4]) {
                        bool fits = true;
                        for (int n = 0; n < 9; n++) {
                            if (!nbh[n][neighbourhoods[i][n]]) {
                                fits = false;
                                break;
                            }
                        }
                        if (fits) {
                            works = true;
                        }
                        else if(!reduced) {
                            reduced = true;
                            for (int y = -1; y < 2; y++) {
                                for (int x = -1; x < 2; x++) {
                                    if (y != 0 && x != 0)
                                        tilesToCheck.push_back({y, x});
                                }
                            }
                        }
                    }
                }
            }
            possibleList[(get<0>(tile)) * resolution + get<1>(tile)][it] = works;
        
        }

        vector<bool> boolList = possibleList[get<0>(tile) * resolution + get<1>(tile)];
    }
    
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
        
        // splat down some random pixels
        for( unsigned int i = 0; i < ww*wh; i++ ) {
            const unsigned int x = rand() % texWidth;
            const unsigned int y = rand() % texHeight;

            const unsigned int offset = ( texWidth * y * 4 ) + x * 4;
            pixels[ offset + 0 ] = rand() % 256;        // b
            pixels[ offset + 1 ] = rand() % 256;        // g
            pixels[ offset + 2 ] = rand() % 256;        // r
            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        }

        SDL_UpdateTexture(winTex, nullptr, pixels.data(), texWidth * 4);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, winTex, nullptr, nullptr);

        SDL_RenderPresent(renderer);

        dt = (SDL_GetTicks() - startTime) / 1000.0f;

    }

    return 0;

}

//TODO:

/*
- get tilerendering up and running
- get surrounding tiles
- get list that contains rules for each type of tile (lambda functions)
- generate tiles in a grid based on the rules:
    pick a tule in the middle of the grid, then get the possible tiles for the surrounding ones, and pick a random tile for the tile with
    the most possibilities. Repeat this for all surrounding tiles.
*/

//Play among us
// 
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
