#include "LowCodeForITKApplication.hpp"

int Main(int argc, char **argv)
{
    LowCodeForITKApplication gui("Gui", argc, argv);

    if (gui.Create())
        return gui.Run();

    return 0;
}