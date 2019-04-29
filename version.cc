#ifdef _WINDOWS
#include "stdafx.h"
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunagui;
const std::string& Version::GIT_SHA1 = "d0d285c-dirty";
const std::string& Version::GIT_DATE = "Mon Apr 29 18:08:09 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Merge branch 'master' of https://git.ded1.denv.it/shrug/tuna-gui";
