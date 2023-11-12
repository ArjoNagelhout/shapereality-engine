#ifndef BORED_ENGINE_WINDOW_TEST_H
#define BORED_ENGINE_WINDOW_TEST_H

#include <string>

// requirements:
// - no runtime overhead
// - cross-platform using different implementation files
// - static

// so we would have:
// window.h
// window_apple.h
// window_win32.h
// window_linux.h

// identified techniques:
// - pure virtuals
// - pimpl idiom (pointer to an implementation)

// .m file is Objective-C only
// .mm file is Objective-C mixed with C++ (also known as Objective-C++ (https://en.wikipedia.org/wiki/Objective-C#:~:text=Objective%2DC%2B%2B%20is%20a%20language,Objective%2DC%20adds%20to%20C.))

class Platform
{
	void (*createWindow)(int x, int y, int width, int height);

	explicit Platform()
	{
		createWindow(10, 10, 10, 10);
	}
};

#endif //BORED_ENGINE_WINDOW_TEST_H