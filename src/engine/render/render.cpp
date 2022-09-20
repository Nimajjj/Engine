//
// Created by Ben on 20/07/2022.
//

#include "render/render.h"


// constructor & destructor

Render::Render(const std::string& name, const int& w, const int& h) :
    width(w),
    height(h),
    font_size(64),
    window(nullptr),
    renderer(nullptr),
    font(nullptr)
{
    init_sdl();
    init_window(name);
    init_renderer();
}

Render::~Render() {
    Close();
}


// init and close

void Render::Close() {
    if ( font != nullptr ) {
        TTF_CloseFont( font );
        TTF_Quit();
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void Render::init_sdl() {
    if ( SDL_Init(SDL_INIT_VIDEO) > 0 ) {
        std::cout << "SDL_Init(SDL_INIT_VIDEO) HAS FAILED: " << SDL_GetError() << "\n";
    } else {
        std::cout << "SDL_Init(SDL_INIT_VIDEO) SUCCESS\n";
    }
}

void Render::init_window(const std::string& name) {
    window = SDL_CreateWindow(
            name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN // | SDL_WINDOW_RESIZABLE
    );

    if ( !window ){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
    }
}

void Render::init_renderer() {
    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        SDL_DestroyWindow( window );
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
    }
}

// rendering

void Render::Begin() {
    SDL_RenderClear( renderer );
}

void Render::End(const Color& clear_color) {
    SDL_SetRenderDrawColor( renderer, clear_color.r, clear_color.g, clear_color.b, 0xFF );
    SDL_RenderPresent( renderer );
}

SDL_Renderer*& Render::Renderer() {
    return renderer;
}

SDL_Window*& Render::Window() {
    return window;
}


// drawing

void Render::DrawFillRect(const int& x, const int& y, const int& w, const int& h, Color color) {
    auto rect = SDL_Rect{ x, y, w, h };

    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    SDL_RenderFillRect( renderer, &rect );
}

void Render::DrawRect(const int& x, const int& y, const int& w, const int& h, Color color) {
    auto rect = SDL_Rect{ x, y, w, h };

    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    SDL_RenderDrawRect( renderer, &rect );
}

void Render::DrawPixel(const int& x, const int& y, Color color) {
    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    SDL_RenderDrawPoint( renderer, x, y );
}

void Render::DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, Color color) {
    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    SDL_RenderDrawLine( renderer, x1, y1, x2, y2 );
}

void Render::DrawCircle(const int& center_x, const int& center_y, const int& rad, Color color) {
    const int32_t diameter = (rad * 2);

    int32_t x = (rad - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void Render::DrawFillCircle(const int& center_x, const int& center_y, const int& rad, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < rad * 2; w++) {
        for (int h = 0; h < rad * 2; h++) {
            int dx = rad - w; // horizontal offset
            int dy = rad - h; // vertical offset
            if ((dx*dx + dy*dy) <= (rad * rad))
            {
                SDL_RenderDrawPoint(renderer, center_x + dx, center_y + dy);
            }
        }
    }
}


// text

void Render::InitText(const std::string& file) {
    TTF_Init();
    font = TTF_OpenFont( file.c_str(), font_size );
}

void Render::Print(const std::string& text, const int& x, const int& y, const int& size) {
    SDL_Surface* surface = TTF_RenderText_Solid( font, text.c_str(), {0xFF,0xFF,0xFF} );
    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_FreeSurface( surface );

    int w, h;
    SDL_QueryTexture( texture, NULL, NULL, &w, &h );

    double font_size_delta = (double)size / font_size;
    SDL_Rect dstrect = SDL_Rect{
        x,
        y,
        (int)( w * font_size_delta ),
        (int)( h * font_size_delta )
    };

    SDL_RenderCopy( renderer, texture, NULL, &dstrect );
    SDL_DestroyTexture( texture );
}