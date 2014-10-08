#include <SDL.h>

bool runEventLoop();
void drawPixel( SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b );
void drawGradient( SDL_Surface* surface, int x1, int x2, int y, Uint32 color1, Uint32 color2 );

int main() {
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) {
        printf( "Failed to initialize SDL: %s\n", SDL_GetError() );
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow( "Gradient!", NULL, NULL, 800, 600, SDL_WINDOW_SHOWN );

    if ( window == NULL ) {
        printf( "Failed to create SDL Window: %s\n", SDL_GetError() );
        return 0;
    }

    SDL_Surface* surface = SDL_GetWindowSurface( window );

    Uint32 color1, color2;
    color1 = SDL_MapRGB( surface->format, 200, 40, 40 );
    color2 = SDL_MapRGB( surface->format, 40, 40, 200 );
    drawGradient( surface, 0, 799, 299, color1, color2 );

    SDL_UpdateWindowSurface( window );

    while ( runEventLoop() ) {
        SDL_Delay( 20 );
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}

void drawGradient( SDL_Surface* surface, int x1, int x2, int y, Uint32 color1, Uint32 color2 ) {
    int xDist = x1 - x2;
    if ( xDist == 0 ) {
        printf( "We can only draw horizontal gradients.\n" );
        return;
    }

    Uint8 r1;
    Uint8 r2;
    Uint8 g1;
    Uint8 g2;
    Uint8 b1;
    Uint8 b2;
    SDL_GetRGB( color1, surface->format, &r1, &g1, &b1 );
    SDL_GetRGB( color2, surface->format, &r2, &g2, &b2 );

    int rDiff = r1 - r2;
    int gDiff = g1 - g2;
    int bDiff = b1 - b2;

    float rStep = -rDiff / fabs(xDist);
    float gStep = -gDiff / fabs(xDist);
    float bStep = -bDiff / fabs(xDist);

    int i = 0;
    for ( int x = x1; x <= x2; x++ ) {
        drawPixel( surface, x, y, (Uint8)(r1 + ceil(rStep*i)), (Uint8)(g1 + ceil(gStep*i)), (Uint8)(b1 + ceil(bStep*i)) );
        i++;
    }
}

void drawPixel( SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b ) {
    bool lock = SDL_MUSTLOCK( surface );
    if ( lock && SDL_LockSurface( surface ) < 0 ) {
        printf( "Couldn't lock surface for drawing.\n" );
        return;
    }

    if ( x >= surface->w || x < 0 || y >= surface->h || y < 0 ) {
        printf( "Tried to draw a pixel off the surface.\n" );
        return;
    }

    Uint32* buffer;
    Uint32 color = SDL_MapRGB( surface->format, r, g, b );

    buffer = (Uint32*)surface->pixels + y * surface->pitch/4 + x;
    (*buffer) = color;

    if ( lock ) {
        SDL_UnlockSurface( surface );
    }
}

bool runEventLoop() {
    SDL_Event event;

    bool quit = false;
    while ( SDL_PollEvent( &event ) ) {
        quit = event.type == SDL_QUIT;
    }
    return !quit;
}

