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

    SDL_Event sdlEvent;
    ENetEvent enetEvent;

    Uint32 lastFrame = SDL_GetTicks();
    Uint32 now;
    int dt;

    while ( running )
    {
        while ( SDL_PollEvent( &sdlEvent ))
            handleEvent( &sdlEvent );

        while ( enet_host_service( client, &enetEvent, 0 ) )
            handleNetEvent( enetEvent );

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


    client = enet_host_create( NULL, 1, 2, 0, 0 );

    if ( client == NULL )
    {
        fprintf( stderr, "Could not create enet host.\n" );
        return false;
    }

    ENetAddress address;
    enet_address_set_host( &address, "novaember.com" );
    address.port = 7182;

    peer = enet_host_connect( client, &address, 2, 0 );

    if ( peer == NULL )
    {
        fprintf( stderr, "No available peers for initiating an ENet connection.\n" );
        return false;
    }

    ENetEvent event;
    if ( !( enet_host_service( client, &event, 5000 ) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT ) )
    {
        enet_peer_reset( peer );

        fprintf( stderr, "Could not connect to da server.\n" );
        return false;
    }


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


void Main::handleNetEvent( ENetEvent event )
{
    switch (event.type)
    {
        case ENET_EVENT_TYPE_CONNECT:

            printf( "A new client connected from %x:%u.\n",
                    event.peer->address.host,
                    event.peer->address.port );
            /* Store any relevant client information here. */
            event.peer->data = (void*) "Client information";

            break;

        case ENET_EVENT_TYPE_RECEIVE:

            printf ("A packet of length %lu containing %s was received from %s on channel %u.\n",
                    event.packet->dataLength,
                    event.packet->data,
                    (char*) event.peer->data,
                    event.channelID);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy (event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:

            printf ("%s disconnected.\n", (char*) event.peer->data);
            /* Reset the peer's client information. */
            event.peer->data = NULL;
    }
}


void Main::cleanup( )
{
    SDL_Quit( );

    enet_host_destroy( client );
}


int main( int argc, char* argv[] )
{
    Main game;

    return game.execute( );
}
