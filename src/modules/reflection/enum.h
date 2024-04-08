//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include <reflection/type_info.h>

namespace reflection
{
    namespace implementation
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
            assert(to.contains(value));
            return *to.at(value);
        }
    }

    struct EnumInfo final : public TypeInfo
    {
        explicit EnumInfo(std::string name);

        void anyFromString(std::string const& in, std::any out) const;

        [[nodiscard]] std::string_view anyToString(std::any in) const;

        void iterate(std::function<void(std::pair<int, std::string_view> const& a)> const& callback) const;

    private:
        std::vector<int> cases;
        std::unordered_map<std::string, int> from;
        std::unordered_map<int, std::string const*> to;

        void
        (* fromImplementation)(std::string const&, std::any, std::unordered_map<std::string, int> const&) = nullptr;

        std::string_view
        (* toImplementation)(std::any, std::unordered_map<int, std::string const*> const&) = nullptr;

        void buildToMap();

        template<typename Type>
        friend
        class EnumInfoBuilder;
    };

    template<typename Type>
    class EnumInfoBuilder final
    {
    public:
        explicit EnumInfoBuilder(std::string name) : info(std::make_unique<EnumInfo>(std::move(name)))
        {
            info->fromImplementation = implementation::anyFromString<Type>;
            info->toImplementation = implementation::anyToString<Type>;
        }

        [[nodiscard]] EnumInfoBuilder& case_(Type value, std::string const& name)
        {
            int v = static_cast<int>(value);
            info->cases.emplace_back(v);
            info->from[name] = v;
            return *this;
        }

        void emplace(TypeInfoRegistry& r)
        {
            info->buildToMap();
            r.emplace<Type>(std::move(info));
        }

        /**
         * emplace in shared registry
         */
        void emplace()
        {
            emplace(TypeInfoRegistry::shared());
        }

    private:
        std::unique_ptr<EnumInfo> info;
    };
}

#endif //SHAPEREALITY_ENUM_H
