#pragma once

#include <string>

#define GUI_MAJOR 2
#define GUI_MINOR 1
#define GUI_PATCH 1

namespace tunagui {
    struct Version {
        static const std::string& GIT_SHA1;
        static const std::string& GIT_DATE;
        static const std::string& GIT_COMMIT_SUBJECT;
    };
}
