// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <functional>
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>

using std::vector;
using std::tuple;
using std::function;
using std::get;
using std::cout;

template<typename T>
bool contains(vector<T> vec, T val) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == val)
            return true;
    }
    return false;
}

//Gives you the sorrounding tiles around a position on the grid
vector<tuple<int, int>> surroundingTiles(vector<vector<Uint8>> &grid, int x, int y) {
    vector<tuple<int, int>> coords;
    for (int iy = -1; iy <= 1; iy++) {
        for (int ix = -1; ix <= 1; ix++) {
            int tempy = iy + y;
            int tempx = ix + x;
            if (iy + y >= grid.size()) {
                tempy = 0;
            }
            else if (y + iy < 0) {
                tempy = grid.size() - 1;
            } 
            if (x + ix >= grid[y + iy].size()) {
                tempx = 0;
            } 
            if (x + ix < 0) {
                tempx = grid[tempy].size() - 1;
            }
            if(ix == 0 && iy == 0) {
                
            }
            else{
                coords.push_back({ tempy, tempx });
            }
        }
    }
    return coords;
}


int main(int argv, char* argc[]) {

    //Width and height respectively
    int ww = 600;
    int wh = 600;

    //Creates the window
    SDL_Window* window = SDL_CreateWindow("Wave function collapse implementation Mattekollo 2023",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    // create texture
    const unsigned int texWidth = ww;
    const unsigned int texHeight = wh;
    SDL_Texture* winTex = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight );
    vector<unsigned char> pixels(texWidth * texHeight * 4, 0);

    // deltatime
    float dt;

    // wfc stuff
    const int resolution = 10;
    vector<vector<Uint8>> grid(resolution, vector<Uint8>(resolution, -1));
    float tile_length = (float)ww / resolution;

    // declare img and get information about it
    vector<Uint8> img = {
        1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 0
    };
    int img_width = 8;
    int img_height = 8;
    int nb_sl = 3;
    int mid_nbh_index = (nb_sl*nb_sl-1)/2;
    int types = 0;    
    for (int i = 0; i < img.size(); i++) {
        if (img[0] > types)
            types = img[0];
    }
    types += 1;

    // gets all neighbourhoods from img
    vector<vector<int>> neighbourhoods((img_width-nb_sl)*(resolution-nb_sl), vector<int>(nb_sl*nb_sl));
    for (int i = 0; i < img_height - nb_sl; i++) {
        for (int j = 0; j < img_width - nb_sl; j++) {
            for (int y = 0; y < nb_sl; y++) {
                for (int x = 0; x < nb_sl; x++) {
                    neighbourhoods[i*(img_width-nb_sl)+j][y*nb_sl+x] = img[(i + y) * img_width + j + x];
                    //neighbourhoods[i*(img_width-nb_sl)+j][(i + y) * img_width + j + x] = img[y * nb_sl + x];
                }
            }
        }
    }

    for (int i = 0; i < neighbourhoods.size(); i++) {
        for (int y = 0; y < nb_sl; y++) {
            for (int x = 0; x < nb_sl; x++) {
                cout << (int)neighbourhoods[i][y*nb_sl+x];
            }
            cout << std::endl;
        }
        cout << std::endl;
    }

    //int start_c = resolution / 2;
    vector<vector<bool>> possibleList(resolution * resolution, vector<bool>(types, true));

    int c_x = resolution / 2;
    int c_y = resolution / 2;

    vector<tuple<int, int>> tilesToCheck = {};

    vector<tuple<int, int>> collapsed = {};

    cout << types << std::endl;

    int reductions = 0;

    //vector<tuple<int, int>> tilesToCheck;// = {{start_c, start_c}};
    int ting = 0;
    while (true) {

        // måste vara fel på hur den väljer att collapsa

        //cout << "iteration: " << ting << std::endl;
        ting++;
        //collapsa
        int collapsed_to = rand() % types;
        for (int i = 0; i < types; i++) {
            possibleList[c_y * resolution + c_x][i] = false;
        }
        possibleList[c_y * resolution + c_x][collapsed_to] = true;

        collapsed.push_back({c_y, c_x});

        //grid[c_y][c_x]
        // add the tiles around the tile that was collapsed to the list of tiles to propogate
        for (int y = -1; y < 2; y++) {
            for (int x = -1; x < 2; x++) {
                if (c_x + x < resolution-1 && c_y + y < resolution-1 && c_x + x > 0 && c_y + y > 0 && x!=0 && y!=0) {
                    tuple<int, int> t = { c_y + y, c_x + x };
                    if (!contains<tuple<int, int>>(tilesToCheck, t)) {
                        tilesToCheck.push_back(t);
                    }
                }
            }
        }

        //cout << tilesToCheck.size() << std::endl;


        /*
        Memories broken the truth goes unspoken I've forgotten my naaaame
        I don't know the season or what is the reason I've even forgotten my naaaame
        I don't know the season or what is the reason I've even forgotten my naaaaame
        I don't know the season or what is the reason I've even forgotten my name!!
        */
        //Propagates through tiles
        while (tilesToCheck.size() != 0) {
            //cout << "check" << std::endl;

            tuple<int, int> tile = tilesToCheck[0];

            //cout << "tile y: " << get<0>(tile) << " tile x: " << get<1>(tile) << std::endl;

            //cout << "tilepos: " << get<0>(tile) << " " << get<1>(tile) << std::endl;

            //cout << "tilestocheck len: " << tilesToCheck.size() << std::endl;

            cout << "tilestocheck len1: " << tilesToCheck.size() << std::endl;
            tilesToCheck.erase(tilesToCheck.begin());
            cout << "tilestocheck len2: " << tilesToCheck.size() << std::endl << std::endl;

            /*
            vector<vector<bool>> nbh = {
                possibleList[(get<0>(tile) - 1) * resolution + get<1>(tile) - 1], possibleList[(get<0>(tile) - 1) * resolution + get<1>(tile)], possibleList[(get<0>(tile) - 1) * resolution + get<1>(tile) + 1],
                possibleList[(get<0>(tile)) * resolution + get<1>(tile) - 1], possibleList[(get<0>(tile)) * resolution + get<1>(tile)], possibleList[(get<0>(tile)) * resolution + get<1>(tile) + 1],
                possibleList[(get<0>(tile) + 1) * resolution + get<1>(tile) - 1], possibleList[(get<0>(tile) + 1) * resolution + get<1>(tile)], possibleList[(get<0>(tile) + 1) * resolution + get<1>(tile) + 1]
            };
            */

            vector<vector<bool>> nbh(9);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {

                    int I = get<0>(tile) + i;
                    int J = get<1>(tile) + j;

                    if (I < 0) {
                        I += resolution;
                    } else if (I > resolution-1) {
                        I -= resolution;
                    }

                    if (J < 0) {
                        J += resolution;
                    } else if (J > resolution-1) {
                        J -= resolution;
                    }

                    nbh[i*3+j] = possibleList[I*resolution+J];

                }
            }


            // behöver ändras?
            vector<tuple<int, int>> sTiles = surroundingTiles(grid, get<1>(tile), get<0>(tile));
            //cout << "test" << std::endl;
            bool reduced = false;
            for (int it = 0; it < nbh[mid_nbh_index].size(); it++) {
                if (nbh[mid_nbh_index][it]) {
                    bool works = false;
                    for (int i = 0; i < neighbourhoods.size(); i++) {
                        // denna if-sats kan vara ett fel
                        if (it == neighbourhoods[i][mid_nbh_index]) {
                            //cout << "TEST" << std::endl;
                            bool fits = true;
                            for (int n = 0; n < nb_sl*nb_sl; n++) {
                                if (!nbh[n][neighbourhoods[i][n]]) {
                                    fits = false;
                                    break;
                                }
                            }
                            if (fits) {
                                works = true;
                                //cout << "testing";
                            }
                        }
                    }
                    if (!works) {
                        //cout << "test2" << std::endl;
                    }
                    possibleList[(get<0>(tile)) * resolution + get<1>(tile)][it] = works;
                    if (!works && !reduced) {
                        //cout << "TEST1" << std::endl;
                        reduced = true;
                    }
                }
            }
            if (reduced) {
                //cout << "TEST2" << std::endl;
                reductions++;
                cout << "reductions: " << reductions << std::endl;
                for (int y = -1; y < 2; y++) {
                    for (int x = -1; x < 2; x++) {
                        if (y != 0 && x != 0) {

                            int I = y + get<0>(tile);
                            int J = x + get<1>(tile);

                            if (I < 0) {
                                I += resolution;
                            } else if (I > resolution-1) {
                                I -= resolution;
                            }

                            if (J < 0) {
                                J += resolution;
                            } else if (J > resolution-1) {
                                J -= resolution;
                            }

                            tilesToCheck.push_back({I, J});

                            /*
                            if ((y != 0 && x != 0 && y + get<0>(tile) < resolution-1 && y + get<0>(tile) > 0 && x + get<1>(tile) < resolution-1 && x + get<1>(tile) > 0) && !contains<tuple<int, int>>(tilesToCheck, {get<0>(tile) + y, get<1>(tile) + x}))
                                tilesToCheck.push_back({get<0>(tile) + y, get<1>(tile) + x });
                            */
                        }
                    }
                }
            }

            /*
            for (int i = 0; i < tilesToCheck.size(); i++) {
                cout << "y: " << get<0>(tilesToCheck[i]) << " x: " << get<1>(tilesToCheck[i]) << " ";
            }
            cout << std::endl << std::endl;
            */

            //vector<bool> boolList = possibleList[get<0>(tile) * resolution + get<1>(tile)];

        }

        // hitta den tile som skall collapsas
        int lowest = types+1;
        int lowest_i = 0;
        int highest = 1;
        for (int i = 0; i < resolution * resolution; i++) {
            int ct_y = (i - (i % resolution)) / resolution;
            int ct_x = (i % resolution);
            //if (/*!contains<tuple<int, int>>(collapsed, {ct_y, ct_x})*/) {
            int sum = 0;
            for (int ib = 0; ib < types; ib++) {
                if (possibleList[i][ib])
                    sum += 1;
            }
            if (sum > highest)
                highest = sum;
            if (sum != 1) {
                if (sum < lowest) {
                    lowest_i = i;
                    lowest = sum;
                }
            }
        }

        //cout << "lowest: " << lowest << std::endl;
        //cout << "highest: " << highest << std::endl;
        //cout << "lowest: " << lowest << std::endl;
        if (highest == 1) {
            //cout << "quit" << std::endl;
            break;
        }

        c_y = (lowest_i - (lowest_i % resolution)) / resolution;
        c_x = (lowest_i % resolution);

        //cout << "c_x: " << c_x << " c_y: " << c_y << std::endl;
        //cout << "lowest_i: " << lowest_i << std::endl;
        
    }

    for (int i = 0; i < resolution*resolution; i++) {
        int sum = 0;
        for (int ib = 0; ib < types; ib++) {
            if (possibleList[i][ib])
                sum += 1;
        }
        //cout << "sum: " << sum << std::endl;
    }

    int count = 0;
    // get possibleList:s data to grid
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            for (int ib = 0; ib < types; ib++) {
                if (possibleList[i * resolution + j][ib]) {
                    grid[i][j] = ib;
                    count++;
                    //cout << "count: " << count << std::endl;
                    //cout << "EXAMPLE!: " << ib << std::endl;
                    break;
                }
            }
        }
    }

    /*
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            cout << (int)grid[i][j] << " ";
        }
        cout << std::endl;
    }
    */

    float tile_sl = (float)ww / resolution;
    
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

        /*
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
        */

        SDL_UpdateTexture(winTex, nullptr, pixels.data(), texWidth * 4);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                if (grid[i][j]) {
                    
                    SDL_Rect r;
                    r.x = j*tile_sl;
                    r.y = i*tile_sl;
                    r.w = tile_sl;
                    r.h = tile_sl;
                    SDL_RenderDrawRect(renderer, &r);
                    
                    //cout << "TEST!!" << std::endl;
                }
            }
        }

		//SDL_RenderCopy(renderer, winTex, nullptr, nullptr);

        SDL_RenderPresent(renderer);

        dt = (SDL_GetTicks() - startTime) / 1000.0f;

    }

    return 0;

}

//TODO:

/*
- get tilerendering up and running
- create a process for drawing images and then inputting them as rules
*/

/*
Bra idéer:
- skapa en funktion för att ge ett neighboardhood av measurements n*n från ett grid
*/

//Play among us

// bopi bopi bop bopi bo
// bopi bopi bop bopi bo

// Fel med koden: den kollar om tiles ska propageras så som om alla hade true på alla möjliga tiles

// lo qwerty
