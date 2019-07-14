/*
* Copyright (C) 2018 HANGHZOU YUNPAI NETWORK TECH CO.
* huangtao117@yeah.net QQ:409577078
*/

#ifndef YUNPAI_BANNER_H_
#define YUNPAI_BANNER_H_

#include "Define.h"

namespace yunpai
{
    namespace Banner
    {
        void Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)());
    }
}

#endif // YUNPAI_BANNER_H_
