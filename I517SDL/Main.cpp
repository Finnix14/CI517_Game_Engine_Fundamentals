#include "Engine.h"

//first file which executes engine initialisation and main loop
int main(int argc, char* args[])
{
    //running the main loop, and cleanup
    Engine engine;
    engine.Run();
    return 0;
}