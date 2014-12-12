#ifndef _snaik_H_
    #define _snaik_H_

#include <SDL2/SDL.h>
#include <deque>
#include <queue>

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Orientation { VERTICAL, HORIZONTAL };

struct Tile { int x, y; };

class Snaik
{
    public:

        Snaik( );

        void setLength( int length );

        void onKeyDown( SDL_Keysym* keysym );

        void update( int dt );

        void render( SDL_Renderer* renderer );

        void setDirection( Direction direction );

        int getX( ) { return x; };
        int getY( ) { return y; };

        void eat( int amount );

    private:

        int x;
        int y;

        int length;

        Direction direction;
        Direction lastDirection;
        std::queue< Direction > directionQueue;

        Orientation orientation;

        std::deque< Tile > tiles;

        int tickTimer;
        static const int tickInterval = 120;

        void tick( );

        void cleanup( );
};

#endif
