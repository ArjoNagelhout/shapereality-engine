//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include <reflection/type_info.h>
#include <reflection/reflection.h>

namespace reflection
{
    namespace enum_implementation
    {
        template<typename Type>
        void anyFromString(std::string const& in, std::any out, std::unordered_map<std::string, int> const& from)
        {
            assert(from.contains(in));
            *std::any_cast<Type*>(out) = static_cast<Type>(from.at(in));
        }

        template<typename Type>
        std::string_view anyToString(std::any in, std::unordered_map<int, std::string const*> const& to)
        {
            int value = static_cast<int>(*std::any_cast<Type*>(in));
            if (!to.contains(value))
            {
                return "EnumValueOutOfRange"; // this can happen when the enum has a value that was not registered, or if a mask is passed as the value, todo: support mask toString that creates the following string format: "EnumValue1 | EnumValue2 | EnumValue3"
            }
            return *to.at(value);
        }
    }

    struct EnumInfo;

    struct EnumIterator final
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = size_t;
        using value_type = std::pair<int, std::string_view>;
        using const_reference = value_type const&;

        explicit EnumIterator(EnumInfo const& info, size_t index);

        [[nodiscard]] const_reference operator*() const;

        // prefix increment
        [[nodiscard]] EnumIterator& operator++();

        // equality
        friend bool operator==(EnumIterator const& lhs, EnumIterator const& rhs);

        friend bool operator!=(EnumIterator const& lhs, EnumIterator const& rhs);

    private:
        size_t index;
        EnumInfo const& info;
        value_type current;

        void updateCurrent();
    };

    namespace register_
    {
        template<typename Type>
        class Enum;
    }

    struct EnumInfo final : public TypeInfo
    {
        explicit EnumInfo(std::string name);

        void anyFromString(std::string const& in, std::any out) const;

        [[nodiscard]] std::string_view anyToString(std::any in) const;

        [[nodiscard]] EnumIterator begin() const;

        [[nodiscard]] EnumIterator end() const;

        // amount of cases
        [[nodiscard]] size_t size() const;

        [[nodiscard]] bool empty() const;

    private:
        std::vector<int> cases;
        std::unordered_map<std::string, int> from;
        std::unordered_map<int, std::string const*> to;

        void (* fromImplementation)(std::string const&, std::any, std::unordered_map<std::string, int> const&) = nullptr;

        std::string_view (* toImplementation)(std::any, std::unordered_map<int, std::string const*> const&) = nullptr;

        void updateToMap();

        template<typename Type>
        friend class register_::Enum;

        friend class EnumIterator;
    };

    namespace register_
    {
        template<typename Type>
        class Enum final
        {
        public:
            explicit Enum(std::string name) : info(std::make_unique<EnumInfo>(std::move(name)))
            {
                info->fromImplementation = enum_implementation::anyFromString<Type>;
                info->toImplementation = enum_implementation::anyToString<Type>;
            }

            [[nodiscard]] Enum& case_(Type value, std::string const& name)
            {
                int v = static_cast<int>(value);
                info->cases.emplace_back(v);
                info->from[name] = v;
                return *this;
            }

            void emplace(TypeRegistry& r)
            {
                info->updateToMap();
                r.emplace<Type>(std::move(info));
            }

        private:
            std::unique_ptr<EnumInfo> info;
        };
    }

    // convenience function for converting an enum
    template<typename Type>
    [[nodiscard]] std::string_view enumToString(Type value)
    {
        TypeInfo& info = reflection::Reflection::shared().types.get<Type>();
        std::any v = &value;
        EnumInfo& enum_ = info.enum_();
        return enum_.anyToString(v);
    }
}

#endif //SHAPEREALITY_ENUM_H
