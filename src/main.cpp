#include <iostream>
#include <SDL.h>
#include "engine/engine.h"
#include "cmath"

double w = 640; // for debug use only
double h = 480;

void func1() { std::cout << "YEAAAAAH 1" << "\n"; }
void func2() { std::cout << "YEAAAAAH 2" << "\n"; }
void func3() { std::cout << "YEAAAAAH 3" << "\n"; }

int main(int argc, char** argv) {
    srand(time(NULL));
    auto engine = Engine("Coink [core] v0.1", w, h, 60);
    engine.SetClearColor({43, 42, 51});

    auto b1 = engine.ButtonNew(&func1, "Press me 1", 32, 32, 32);
    auto b2 = engine.ButtonNew(&func2, "Press me 2", 32, 32, 128);
    auto b3 = engine.ButtonNew(&func3, "Press me 3", 32, 32,  128 + (128 - 32));


    unsigned x = 0;
    double elapsed_time = 0;
    bool should_quit = false;
    while (!should_quit) {
        engine.LoopBegin();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
            }
            break;
        }
        engine.RenderBegin();

        engine.RenderEnd();
        engine.LoopEnd();
    }

    engine.Close();
    return 0;
}
