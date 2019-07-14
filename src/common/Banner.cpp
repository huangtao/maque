/*
* Copyright (C) 2018 HANGHZOU YUNPAI NETWORK TECH CO.
* huangtao117@yeah.net QQ:409577078
*/

#include "Banner.h"
#include <boost/format.hpp>

// Éú³É×Ö·ûÍ¼ÐÎÍøÖ·
// http://www.network-science.de/ascii/
void yunpai::Banner::Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)())
{
    log((boost::format("yunpai (%1%)") % applicationName).str().c_str());
    log("<Ctrl-C> to stop.\n");
    log("                              (_)");
    log(" _   _ _   _ _ __  _ __   __ _ _ ");
    log("| | | | | | | '_ \\| '_ \\ / _` | |");
    log("| |_| | |_| | | | | |_) | (_| | |");
    log("\\__, |\\__,_|_| |_| .__/ \\__,_|_|");
    log("  __/ |           | |            ");
    log(" |___/            |_|");
    if (logExtraInfo)
        logExtraInfo();
}

