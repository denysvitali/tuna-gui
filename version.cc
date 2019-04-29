#ifdef _WINDOWS
#include "stdafx.h"
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunagui;
const std::string& Version::GIT_SHA1 = "eb30ef1-dirty";
const std::string& Version::GIT_DATE = "Wed Jan 23 10:04:34 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "hotfix list refill every iteration";
