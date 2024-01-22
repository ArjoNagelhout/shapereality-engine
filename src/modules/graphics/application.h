#ifndef SHAPEREALITY_APPLICATION_H
#define SHAPEREALITY_APPLICATION_H

#include <memory>

namespace graphics
{
    // on macOS the operating system calls the application instead of the other way around, so we'll adopt this
    // approach for our framework as well.
    class IApplicationDelegate
    {
    public:
        virtual void applicationDidFinishLaunching() = 0;

        virtual void applicationWillTerminate() = 0;
    };

    class Application final
    {
    public:
        explicit Application();

        ~Application();

        void run();

        [[nodiscard]] IApplicationDelegate* getDelegate() const;

        void setDelegate(IApplicationDelegate* delegate);

        struct Implementation;

    private:
        std::unique_ptr<Implementation> pImplementation;

        IApplicationDelegate* pDelegate{nullptr};
    };
}

#endif //SHAPEREALITY_APPLICATION_H