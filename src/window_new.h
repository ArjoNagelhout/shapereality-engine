
#ifndef BORED_ENGINE_WINDOW_NEW_H
#define BORED_ENGINE_WINDOW_NEW_H

#include <memory>

class WindowNew
{
public:
	explicit WindowNew(int x, int y, int width, int height);
	~WindowNew();
	void setTitle(std::string const& title);

private:
	struct Implementation;

	std::unique_ptr<Implementation> pImpl;
};

// we want to hide the implementation (NSWindow) from the header file, as this would require any .cpp file creating
// a window to compile with Objective-C++. That should be an implementation detail, not inside the header.

// so we can do two things:
// pImpl
// function pointers

#endif //BORED_ENGINE_WINDOW_NEW_H
