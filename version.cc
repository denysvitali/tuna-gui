#ifdef _WINDOWS
#include "stdafx.h"
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunagui;
const std::string& Version::GIT_SHA1 = "fbfa03c-dirty";
const std::string& Version::GIT_DATE = "Tue May 7 13:38:17 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Leapmotion implementation";
