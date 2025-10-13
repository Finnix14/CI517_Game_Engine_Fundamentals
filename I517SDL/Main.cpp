#include "Engine.h"

int main(int argc, char* args[])
{
    //running the main loop, and cleanup
    Engine engine;

    // follows game engine architecture for modularity
    engine.Run();

    return 0;
}
