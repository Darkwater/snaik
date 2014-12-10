#ifndef _stage_H_
    #define _stage_H_

#include <SDL2/SDL.h>
#include <list>

#include "main.h"
#include "snaik.h"

struct Food { int x, y; };

class Stage
{
    public:

        Stage( );

        void initialize( );

        void onKeyDown( SDL_Keysym* keysym );

        void update( int dt );

        void render( SDL_Renderer* renderer );

        void cleanup( );

    private:

        Snaik snaik;

        std::list< Food > foods;
};

#endif
