#include <stdio.h>
#include <SDL.h>

#include "StartData.h"
#include "ProcessingData.h"
#include "Bot.h"

// ----------------------------------------------------------------------------
// This function only for start ProcessingData::actionTread()
// in separate thread.
int bridgeActionThread( void* data )
{
    ProcessingData *p = (ProcessingData*) data;
    p->actionThread();
    return 0;
}


// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    StartData startData;
    if ( !( startData.isSuccess ) )
    {
        printf( "Failed to prepare start data." );
        return 1;
    }

    ProcessingData processingData( &startData );

    processingData.refresh();
    SDL_Thread* processingDataThread;
    processingDataThread = SDL_CreateThread( bridgeActionThread,
                                "actionThread", &processingData );

    processingData.keyControl();

    SDL_WaitThread( processingDataThread, nullptr );

    return 0;
}

