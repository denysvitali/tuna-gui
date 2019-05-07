#ifdef _WINDOWS
#include "stdafx.h"
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunagui;
const std::string& Version::GIT_SHA1 = "59e6a92-dirty";
const std::string& Version::GIT_DATE = "Sun May 5 15:57:31 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "using new scene, index recognition";
