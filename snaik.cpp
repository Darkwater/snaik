#include <cstdio>

#include "main.h"
#include "snaik.h"


bool operator==( const Tile& l, const Tile& r )
{
    return l.x == r.x && l.y == r.y;
}


Snaik::Snaik( )
{
    x = 0;
    y = 0;

    length = 7;

    direction = RIGHT;

    tiles.push_front( { x, y } );

    tick( );

    tickTimer = 0;
}


void Snaik::setLength( int length_ )
{
    length = length_;

    for ( int i = length; i < tiles.size(); i++ )
    {
        tiles.pop_back( );
    }
}


void Snaik::tick( )
{
    if ( directionQueue.size() > 0 )
    {
        direction = directionQueue.front();
        directionQueue.pop();
    }

    switch ( direction )
    {
        case UP:    y--; break;
        case DOWN:  y++; break;
        case LEFT:  x--; break;
        case RIGHT: x++; break;

        default: direction = RIGHT; break;
    }

    if ( direction == UP || direction == DOWN ) orientation = VERTICAL;
    else                                        orientation = HORIZONTAL;

    while ( tiles.size() >= length )
        tiles.pop_back();

    tiles.push_front( { x, y } );
}


void Snaik::update( int dt )
{
    tickTimer += dt;

    if (tickTimer >= tickInterval )
    {
        tickTimer -= tickInterval;

        tick( );
    }
}


void Snaik::render( SDL_Renderer* renderer )
{
    if ( tiles.size() < 2 ) return;

    SDL_SetRenderDrawColor( renderer, 255, 175, 0, 255 );

    SDL_Rect r;
    r.w = r.h = Main::gridSize;

    bool first = true;


    // Render first tile
    Tile* tile = &tiles.front();

    r.x = tile->x * ( Main::gridSize + Main::gridPadding );
    r.y = tile->y * ( Main::gridSize + Main::gridPadding );

    // Smooth animation
    if ( orientation == VERTICAL )
    {
        r.h = Main::gridSize * ( (float) tickTimer / tickInterval );

        if ( ( direction == UP   ) ) r.y += Main::gridSize - r.h;
    }

    if ( orientation == HORIZONTAL )
    {
        r.w = Main::gridSize * ( (float) tickTimer / tickInterval );

        if ( ( direction == LEFT ) ) r.x += Main::gridSize - r.w;
    }

    SDL_RenderFillRect( renderer, &r );

    r.w = r.h = Main::gridSize;


    // Draw other tiles
    std::deque<Tile>::iterator iter;
    for ( iter = ++tiles.begin(); iter != --tiles.end(); iter++ )
    {
        tile = &( *iter );
        r.x = tile->x * ( Main::gridSize + Main::gridPadding );
        r.y = tile->y * ( Main::gridSize + Main::gridPadding );

        SDL_RenderFillRect( renderer, &r );
    }


    // Draw last tile
    Tile prevTile = tiles.at( tiles.size() - 2 );

    tile = &tiles.back();

    r.x = tile->x * ( Main::gridSize + Main::gridPadding );
    r.y = tile->y * ( Main::gridSize + Main::gridPadding );

    // Only show animation when this is actually the last tile
    //  (ie. no more tiles are to be added after this before eating more food)
    if ( tiles.size() == length )
    {
        Orientation lastOrientation = ( prevTile.x == tile->x )
                                    ? VERTICAL
                                    : HORIZONTAL;

        Direction lastDirection = ( lastOrientation == VERTICAL )
                                ? ( prevTile.y > tile->y ? UP   : DOWN  )
                                : ( prevTile.x > tile->x ? LEFT : RIGHT );

        if ( lastOrientation == VERTICAL )
        {
            r.h = Main::gridSize * ( 1 - ( (float) tickTimer / tickInterval ) );

            if ( ( lastDirection == UP   ) ) r.y += Main::gridSize - r.h;
        }

        if ( lastOrientation == HORIZONTAL )
        {
            r.w = Main::gridSize * ( 1 - ( (float) tickTimer / tickInterval ) );

            if ( ( lastDirection == LEFT ) ) r.x += Main::gridSize - r.w;
        }
    }

    SDL_RenderFillRect( renderer, &r );
}


void Snaik::setDirection( Direction direction )
{
    switch ( direction )
    {
        case UP:    if ( lastDirection == DOWN  ) return; break;
        case DOWN:  if ( lastDirection == UP    ) return; break;
        case LEFT:  if ( lastDirection == RIGHT ) return; break;
        case RIGHT: if ( lastDirection == LEFT  ) return; break;
    }

    directionQueue.push( direction );

    lastDirection = direction;
}


void Snaik::onKeyDown( SDL_Keysym* keysym )
{
    if ( keysym->sym == SDLK_UP    ) setDirection( UP    );
    if ( keysym->sym == SDLK_DOWN  ) setDirection( DOWN  );
    if ( keysym->sym == SDLK_LEFT  ) setDirection( LEFT  );
    if ( keysym->sym == SDLK_RIGHT ) setDirection( RIGHT );
}


void Snaik::eat( int amount )
{
    length += amount;
}


void Snaik::cleanup( )
{
    SDL_Quit( );
}
