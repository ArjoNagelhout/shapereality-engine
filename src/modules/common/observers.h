//
// Created by Arjo Nagelhout on 17/03/2024.
//

#ifndef SHAPEREALITY_OBSERVERS_H
#define SHAPEREALITY_OBSERVERS_H

#include <cassert>
#include <vector>

namespace common
{
    template<typename Observer>
    class Observers
    {
    public:
        [[nodiscard]] bool contains(Observer* observer) const
        {
            if (observer == nullptr)
            {
                return false;
            }

            for (Observer* const& o: observers)
            {
                if (o == observer)
                {
                    return true;
                }
            }
            return false;
        }

        void add(Observer* observer)
        {
            assert(!contains(observer) && "observer was already added");
            observers.emplace_back(observer);
        }

        void remove(Observer* observer)
        {
            for (auto it = observers.begin(); it != observers.end(); it++)
            {
                if (*it == observer)
                {
                    observers.erase(it);
                    return;
                }
            }
            assert(false && "trying to remove observer that was not added prior");
        }

        template<auto PointerToMemberFunction, typename... Args>
        void invoke(Args&&... args)
        {
            for (Observer* o: observers)
            {
                (o->*PointerToMemberFunction)(std::forward<Args>(args)...);
                // because std::invoke(PointerToMemberFunction, o, std::forward<Args>(args)...); has worse compile time error messages for when it fails
            }
        }

    private:
        std::vector<Observer*> observers;
    };
}

#endif //SHAPEREALITY_OBSERVERS_H
