// The main header for the graphics framework
#include <graphics_framework.h>

// The namespaces we are using
using namespace std;
using namespace graphics_framework;

// Our rendering code will go in here
bool render()
{
	return true;
}

void main()
{
	// Create application
	app application;
	// Set render function
	application.set_render(render);
	// Run application
	application.run();
}