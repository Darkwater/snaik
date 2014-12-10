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

    std::list<Food>::iterator iter;
    Food* food;
    for ( iter = foods.begin(); iter != foods.end(); iter++ )
    {
        food = &( *iter );
        if ( snaik.getX() == food->x && snaik.getY() == food->y )
        {
            snaik.eat( 1 );

            food->x = rand() % 20;
            food->y = rand() % 15;
        }
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
}


void Stage::cleanup()
{

}
