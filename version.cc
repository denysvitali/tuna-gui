#ifdef _WINDOWS
#include "version.hh"
#else
#include "../../tuna-gui/version.hh"
#endif

using namespace tunagui;
const std::string Version::GIT_SHA1 = "c1c72f7-dirty";
const std::string Version::GIT_DATE = "Sun Nov 18 16:29:53 2018";
const std::string Version::GIT_COMMIT_SUBJECT = "Fix project path";
