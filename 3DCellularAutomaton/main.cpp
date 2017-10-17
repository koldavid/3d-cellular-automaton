#include <iostream>

#include "MainWindow.h"


int main(int argc, char** argv)
{
	MainWindow* appWindow = new MainWindow(800, 600);

	if (appWindow->init()) {
		appWindow->run();
	}
	else {
		return -1;
	}

	delete appWindow;

	return 0;
}