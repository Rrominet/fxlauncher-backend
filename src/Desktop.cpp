#include "./Desktop.h"
#include <cstdlib>
#include "freedesktop.h"
#include "files.2/files.h"
#include "str.h"

json Desktop::serialize() const
{
    json _r;
    _r["path"] = _path;
    _r["icon"] = _iconPath;
    _r["entries"] = {};
    for (const auto& e : _entries)
        _r["entries"][e.first] = e.second;

    return _r;
}

void Desktop::deserialize(const json& j)
{
    _path = j["path"];
    _iconPath = j["icon"];
    for (const auto& e : j["entries"].items())
        _entries[e.key()] = e.value();
}

void Desktop::exec() const
{
    if (_entries.find("Exec") == _entries.end())
        throw std::runtime_error( _entries.at("Name") + " : Exec key not found.");

    auto exec = _entries.at("Exec");
    exec = freedesktop::execNoCodes(exec);
    exec += " &";
    if (_entries.find("Terminal") != _entries.end())
    {
        if (_entries.at("Terminal") == "true")
            exec = "gnome-terminal -- " + exec;
    }

    lg("Executing : " << exec);
    auto res = std::system(exec.c_str());
    if (res != 0)
        throw std::runtime_error( _entries.at("Name") + " : Exec failed with code " + std::to_string(res));
}

const void Desktop::setPath(const std::string& path)
{
    _path = path;
    _entries = freedesktop::desktopFromFile(path);
    std::string icon;
    if (_entries.find("Icon") == _entries.end())
        return;
    icon = _entries.at("Icon");
    if (files::exists(icon))
        _iconPath = icon;
    else 
        _iconPath = freedesktop::iconPathFromName(_entries.at("Icon"));
}

bool Desktop::match(const std::string& query, const std::string entryName) const
{
    if (_entries.find(entryName) == _entries.end())
        return false;
    std::string name = _entries.at(entryName);
    name = str::clean(name, true);
    return str::contains(name, query);
}

bool Desktop::match(const std::string& query) const
{
    std::string searched = query;	
    searched = str::clean(searched, true);

    bool found = false;
    found = match(searched, "Name");
    if (found)
        return true;
    found = match(searched, "Comment");
    if (found)
        return true;
    found = match(searched, "Keywords");
    if (found)
        return true;
    found = match(searched, "Exec");
    if (found)
        return true;
    found = match(searched, "Icon");
    if (found)
        return true;

    return false;
}

