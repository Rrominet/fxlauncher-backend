#include "./Fxlauncher.h"
#include "ipc.h"
#include "./Desktop.h"
#include "freedesktop.h"

namespace fxlauncher
{
    Fxlauncher* _fxlauncher;
}

Fxlauncher::Fxlauncher() : _desktops()
{
    fxlauncher::_fxlauncher = this;	
    this->reg();

    ipc::receive();
}


Fxlauncher::~Fxlauncher()
{
    fxlauncher::_fxlauncher = nullptr;	
}


void Fxlauncher::reg()
{
    this->regTest();
    this->regGetFromFilesystem();
    this->regSearch();
    this->regExec();
}

void Fxlauncher::regGetFromFilesystem()
{
    auto getfs = [this](const json& args) -> json
    {
        json _r;
        _r["data"] = json::array();
        unsigned int lentgh = 15;
        if (args.contains("length"))
            lentgh = args["length"];

        this->getDesktopsFromFilesystem(true);
        for (unsigned int i = 0; i < _desktops.size(); i++)
        {
            if (i>=lentgh)
                break;
            _r["data"].push_back(_desktops[i]->serialize());
        }

        ipc::success(_r);
        return _r;
    };

    ipc::reg("getFromFilesystem", getfs);
}


void Fxlauncher::regSearch()
{
    auto f = [this](const json& args) -> json
    {
        json _r;
        if (ipc::errorIfNotExists(args, _r, "search"))
            return _r;
        _r["data"] = json::array();
        this->getDesktopsFromFilesystem();
        for (const auto& d : _desktops)
        {
            if (d->match(args["search"]))
                _r["data"].push_back(d->serialize());
        }
        ipc::success(_r);
        return _r;
    };

    ipc::reg("search", f);
}


void Fxlauncher::regExec()
{
    auto f = [this](const json& args) -> json
    {
        json _r;
        if (ipc::errorIfNotExists(args, _r, "path"))
            return _r;

        this->getDesktopsFromFilesystem();
        auto desktop = this->desktop(args["path"]);
        if (!desktop)
        {
            ipc::error(_r, "Desktop file not found : " + args["path"].get<std::string>());
            return _r;
        }

        try
        {
            desktop->exec();
        }
        catch (const std::runtime_error& e)
        {
            ipc::error(_r, "Error during the desktop execution : " + args["path"].get<std::string>() + " : " + e.what());
            return _r;
        }

        ipc::success(_r);
        return _r;
    };

    ipc::reg("exec", f);
}



void Fxlauncher::regTest()
{
    //testing ipc
    auto ftest = [](const json& args) -> json
    {
        json _r;
        _r["success"] = true;
        _r["message"] = "This is a test.";
        _r["data"] = args;
        return _r;
    };

    ipc::reg("test", ftest);
}

void Fxlauncher::getSavedDesktops()
{
	
}

void Fxlauncher::getDesktopsFromFilesystem(bool force)
{
    if (!_desktops.empty() && !force)
        return;

	auto files = freedesktop::desktopFiles();

    for (const auto& file : files)
    {
        auto desktop = std::make_unique<Desktop>();
        desktop->setPath(file);
        _desktops.push_back(std::move(desktop));
    }
}

Desktop* Fxlauncher::desktop(const std::string& path) const
{
    for (const auto& desktop : _desktops)
    {
        if (desktop->path() == path)
            return desktop.get();
    }
    return nullptr;	
}



namespace fxlauncher
{
    Fxlauncher* get(){return _fxlauncher;}
}
