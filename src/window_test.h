#ifndef BORED_ENGINE_WINDOW_TEST_H
#define BORED_ENGINE_WINDOW_TEST_H

#import <Cocoa/Cocoa.h>

#ifdef __cplusplus
extern "C" {
#endif

class WindowTest
{
public:
	explicit WindowTest();
	~WindowTest();

private:
	NSWindow* pWindow{};
};

#ifdef __cplusplus
}
#endif

#endif //BORED_ENGINE_WINDOW_TEST_H