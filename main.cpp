#include <iostream>

#include "main.h"


Main::Main( )
{
    running = true;

    window = NULL;
    renderer = NULL;
}


int Main::execute( )
{
    if ( !initialize( ) )
        return -1;

    SDL_Event event;

    Uint32 lastFrame = SDL_GetTicks();
    Uint32 now;
    int dt;

    while ( running )
    {
        while ( SDL_PollEvent( &event ))
            handleEvent( &event );

        now = SDL_GetTicks();
        dt = now - lastFrame;
        lastFrame = now;

        update( dt );
        render( );

        SDL_Delay( 1 );
    }

    cleanup( );

    return 0;
}


bool Main::initialize( )
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
        return false;

    window = SDL_CreateWindow(
            "Snaik",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            0 );

    if ( window == NULL )
        return false;

    renderer = SDL_CreateRenderer( window, -1, 0 );

    if ( renderer == NULL )
        return false;

    SDL_RenderSetLogicalSize( renderer, windowWidth, windowHeight );

    stage.initialize( );

    return true;
}


void Main::update( int dt )
{
    stage.update( dt );
}


void Main::render( )
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

    SDL_RenderClear( renderer );

    stage.render( renderer );

    SDL_RenderPresent( renderer );
}


void Main::handleEvent( SDL_Event* event )
{
    if ( event->type == SDL_QUIT )
        running = false;

    if ( event->type == SDL_KEYDOWN )
    {
        if ( event->key.keysym.sym == SDLK_ESCAPE )
        {
            running = false;
            return;
        }

        stage.onKeyDown( &event->key.keysym );
    }
}


void Main::cleanup( )
{
    SDL_Quit( );
}


int main( int argc, char* argv[] )
{
    Main game;

    return game.execute( );
}
