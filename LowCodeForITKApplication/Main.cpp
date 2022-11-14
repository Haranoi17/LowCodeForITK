#include "LowCodeForITKGui.hpp"

int Main(int argc, char** argv)
{
	LowCodeForITKGui gui("Gui", argc, argv);

	if (gui.Create())
		return gui.Run();

	return 0;
}