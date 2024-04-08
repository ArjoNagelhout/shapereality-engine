//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include <reflection/type_info.h>

namespace reflection
{
    using FromMap = std::unordered_map<std::string, int>;
    using ToMap = std::unordered_map<int, std::string const*>;

    template<typename Type>
    void anyFromString(std::string const& in, std::any out, FromMap const& from)
    {
        assert(from.contains(in));
        *std::any_cast<Type*>(out) = static_cast<Type>(from.at(in));
    }

    template<typename Type>
    std::string_view anyToString(std::any in, ToMap const& to)
    {
        int value = static_cast<int>(*std::any_cast<Type*>(in));
        assert(to.contains(value));
        return *to.at(value);
    }

    struct EnumInfo final : public TypeInfo
    {
        explicit EnumInfo(std::string name);

        void anyFromString(std::string const& in, std::any out) const;

        [[nodiscard]] std::string_view anyToString(std::any in) const;

        void iterate(std::function<void(std::pair<std::string, int> const& a)> const& callback) const;

        void (* anyFromStringImplementation)(std::string const& in, std::any out, FromMap const& from) = nullptr;

        std::string_view (* anyToStringImplementation)(std::any in, ToMap const& to) = nullptr;

        void buildToMap();

        FromMap from;
        ToMap to;
    };

    template<typename Type>
    class EnumInfoBuilder final
    {
    public:
        explicit EnumInfoBuilder(std::string name) : info(std::make_unique<EnumInfo>(std::move(name)))
        {
            info->anyFromStringImplementation = anyFromString<Type>;
            info->anyToStringImplementation = anyToString<Type>;
        }

        [[nodiscard]] EnumInfoBuilder& case_(Type value, std::string const& name)
        {
            info->from[name] = static_cast<int>(value);
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
