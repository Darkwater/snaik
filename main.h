#ifndef _main_H_
    #define _main_H_

#include <SDL2/SDL.h>
#include <enet/enet.h>

#include "stage.h"

class Main
{
    public:

        static const int gridSize = 32;
        static const int gridPadding = 1;

        static const int windowWidth = 800;
        static const int windowHeight = 600;


        Main( );

        int execute( );

    private:

        bool running;

        SDL_Window* window;
        SDL_Renderer* renderer;

        ENetHost* client;
        ENetPeer* peer;

        Stage stage;


        bool initialize( );

        void handleEvent( SDL_Event* event );
        void handleNetEvent( ENetEvent event );

        void update( int dt );

        void render( );

        void cleanup( );
};

#endif
