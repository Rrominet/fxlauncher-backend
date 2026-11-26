#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "vec.h"

class Desktop;
class Fxlauncher
{
    public : 
        Fxlauncher();
        ~Fxlauncher();

        //regiester all ipc commands
        void reg();
        void regTest();
        void regGetFromFilesystem();
        void regSearch();
        void regExec();

        // only read the cached version
        // so faster but could be outdated.
        void getSavedDesktops();
        void getDesktopsFromFilesystem(bool force = false);

        Desktop* desktop(const std::string& path) const;

    private : 
        ml::Vec<std::unique_ptr<Desktop>> _desktops;
};

namespace fxlauncher
{
    Fxlauncher* get();
}
