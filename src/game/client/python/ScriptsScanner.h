#ifndef DDNET_SCRIPTSSCANNER_H
#define DDNET_SCRIPTSSCANNER_H

#include <vector>
#include "PythonScript.h"

class ScriptsScanner
{
public:
	ScriptsScanner() : directoryForScanning("release\\python") {}
	ScriptsScanner(const char* directoryForScanning)
	{
		this->directoryForScanning = directoryForScanning;
	}

	std::vector<PythonScript*> scan();
protected:
	const char *directoryForScanning;
};

#endif // DDNET_SCRIPTSSCANNER_H
