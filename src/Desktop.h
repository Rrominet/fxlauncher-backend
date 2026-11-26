#pragma once
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Desktop
{
    public:
        Desktop(){}
        ~Desktop(){}

        json serialize() const;
        void deserialize(const json& j);

        void exec() const;
        const void setPath(const std::string& path);

        bool match(const std::string& query) const;
        bool match(const std::string& query, const std::string entryName) const;

    private:

        //path of the .destop file
        std::string _path; //bp cg
        std::string _iconPath = ""; //bp cg
        std::unordered_map<std::string, std::string> _entries;

    public : 
#include "./Desktop_gen.h"
};
