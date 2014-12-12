#include <cstdio>

#include "main.h"
#include "stage.h"


Stage::Stage()
{

}


void Stage::initialize()
{
    foods.push_front( { 2, 5 } );
}


void Stage::onKeyDown( SDL_Keysym* keysym )
{
    snaik.onKeyDown( keysym );
}


void Stage::update( int dt )
{
    snaik.update( dt );

    std::list< Food >::iterator foodIterator;
    Food* food;
    for ( foodIterator = foods.begin(); foodIterator != foods.end(); foodIterator++ )
    {
        food = &( *foodIterator );

        if ( snaik.getX() == food->x && snaik.getY() == food->y )
        {
            snaik.eat( 1 );

            food->x = rand() % 20;
            food->y = rand() % 15;
        }
    }


    std::list< Snaik >::iterator snaikIterator;
    Snaik* iSnaik;
    for ( snaikIterator = snaiks.begin(); snaikIterator != snaiks.end(); snaikIterator++ )
    {
        iSnaik = &( *snaikIterator );

        iSnaik->update( dt );
    }
}


void Stage::render( SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, 0, 175, 255, 255 );

    SDL_Rect r;
    r.w = r.h = Main::gridSize;

    for ( auto food : foods )
    {
        r.x = food.x * ( Main::gridSize + Main::gridPadding );
        r.y = food.y * ( Main::gridSize + Main::gridPadding );

        SDL_RenderFillRect( renderer, &r );
    }

    snaik.render( renderer );


    for ( auto snaik : snaiks )
    {
        snaik.render( renderer );
    }
}


void Stage::cleanup()
{

}
