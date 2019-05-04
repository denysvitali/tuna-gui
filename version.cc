#ifdef _WINDOWS
#include "stdafx.h"
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunagui;
const std::string& Version::GIT_SHA1 = "5ad2bb9-dirty";
const std::string& Version::GIT_DATE = "Mon Apr 29 18:09:42 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Simplified main";
