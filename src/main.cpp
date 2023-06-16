#include "engine.hpp"

int main()
{
    const tme::WindowContext ctx = {800, 800, "Two Mans' Game Engine", 144};
    tme::Engine              engine(ctx);
    engine.run();
    return 0;
}