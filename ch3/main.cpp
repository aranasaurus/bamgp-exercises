#include <SDL.h>
#include <time.h>

bool runEventLoop();
void drawPixel( SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b );
void drawGradient( SDL_Surface* surface, int x1, int x2, int y, Uint32 color1, Uint32 color2 );
bool loadFiles();
void freeFiles();
SDL_Surface* loadImage( char* fileName );

void ex1();
void ex2();
void ex3();

const int WIN_W = 1440;
const int WIN_H = 900;

SDL_Surface* backBuffer = NULL;
SDL_Surface* Background = NULL;
SDL_Surface* SpriteImage = NULL;

int main() {
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) {
        printf( "Failed to initialize SDL: %s\n", SDL_GetError() );
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow( "Let's Rock!", NULL, NULL, WIN_W, WIN_H, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN );

    if ( window == NULL ) {
        printf( "Failed to create SDL Window: %s\n", SDL_GetError() );
        return 0;
    }

    backBuffer = SDL_GetWindowSurface( window );
    srand( time( NULL ) );

    if ( !loadFiles() ) {
        printf( "Failed to load files!\n" );
        freeFiles();
        SDL_Quit();
        return 0;
    }

    /*
    ex1();
    ex2();
    */
    ex3();
    while ( runEventLoop() ) {
        SDL_UpdateWindowSurface( window );
        SDL_Delay( 20 );
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}

void ex1() {
    SDL_SetWindowTitle( SDL_GL_GetCurrentWindow(), "ex1" );
    Uint32 color1, color2;
    color1 = SDL_MapRGB( backBuffer->format, 200, 40, 40 );
    color2 = SDL_MapRGB( backBuffer->format, 40, 40, 200 );
    drawGradient( backBuffer, 0, WIN_W-1, WIN_H/2-1, color1, color2 );
}

void ex2() {
    SDL_SetWindowTitle( SDL_GL_GetCurrentWindow(), "ex2" );
    SDL_Color c;
    c.r = 40;
    c.g = 225;
    c.b = 40;
    c.a = 255;

    SDL_Rect rects[10];

    for ( int i = 0; i < 10; i++ ) {
        SDL_Rect r;
        r.x = rand()%WIN_W;
        r.y = rand()%WIN_H;
        r.w = 50;
        r.h = 100;

        bool colliding = false;
        for ( int j = i-1; j >= 0; j-- ) {
            SDL_Rect r2 = rects[j];
            int cx1 = r.x + (r.w/2);
            int cx2 = r2.x + (r2.w/2);
            int cy1 = r.y + (r.y/2);
            int cy2 = r2.y + (r2.y/2);

            int xDist = abs( cx1 - cx2 );
            int yDist = abs( cy1 - cy2 );
            int touchDistX = (r.w/2) + (r2.w/2);
            int touchDistY = (r.h/2) + (r2.h/2);

            if ( xDist <= touchDistX || yDist <= touchDistY ) {
                colliding = true;
                break;
            }
        }

        if ( colliding ) {
            i--;
        } else {
            rects[i] = r;
        }
    }

    SDL_FillRects( backBuffer, rects, 10, SDL_MapRGB( backBuffer->format, c.r, c.g, c.b ) );
}

void ex3() {
    SDL_SetWindowTitle( SDL_GL_GetCurrentWindow(), "ex3" );
    SDL_Rect sprite1;
    SDL_Rect sprite2;

    sprite1.x = WIN_W/2;
    sprite1.y = WIN_H/2;
    sprite2.x = sprite1.x + 10;
    sprite2.y = sprite1.y + 10;

    SDL_BlitSurface( Background, NULL, backBuffer, NULL );
    SDL_BlitSurface( SpriteImage, NULL, backBuffer, &sprite1 );
    SDL_BlitSurface( SpriteImage, NULL, backBuffer, &sprite2 );
}

bool loadFiles() {
    Background = loadImage( (char*)"res/background.bmp" );
    SpriteImage = loadImage( (char*)"res/spaceship.bmp" );

    return Background != NULL && SpriteImage != NULL;
}

void freeFiles() {
}

SDL_Surface* loadImage( char* fileName ) {
    SDL_Surface *image = NULL;
    SDL_Surface *procImage = NULL;

    image = SDL_LoadBMP( fileName );
    if ( image == NULL ) {
        return NULL;
    }

    procImage = SDL_ConvertSurface( image, backBuffer->format, 0 );
    SDL_FreeSurface( image );

    if ( procImage == NULL ) {
        return NULL;
    }

    Uint32 colorKey = SDL_MapRGB( procImage->format, 0xff, 0, 0xff );
    SDL_SetColorKey( procImage, SDL_TRUE, colorKey );

    return procImage;
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

