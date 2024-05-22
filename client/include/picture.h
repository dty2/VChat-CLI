#ifndef LOG_H
#define LOG_H

#include "common.h"

namespace vchat {

static std::string LOGO = "\
                                           \n\
 ██╗   ██╗ ██████╗██╗  ██╗ █████╗ ████████╗\n\
 ██║   ██║██╔════╝██║  ██║██╔══██╗╚══██╔══╝\n\
 ██║   ██║██║     ███████║███████║   ██║   \n\
 ╚██╗ ██╔╝██║     ██╔══██║██╔══██║   ██║   \n\
  ╚████╔╝ ╚██████╗██║  ██║██║  ██║   ██║   \n\
   ╚═══╝   ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   \n\
                                           \n\
             VChat - Vi Chat               \n\
              version 1.0.0                \n\
          by Hunter(执着) el al.           \n\
VChat is open source and freely distributed\n\
                                           \n";

static std::string MASCOT = "\
        *   ^__^               \n\
         *  (oo)\\_______      \n\
          * (__)\\ VChat )\\/\\\n\
                ||-----||      \n\
                ||     ||      \n\
                               \n";

static std::string TALK_START = "\
 _______________________ \n\
< Have you VChat today ?>\n\
 ----------------------- \n";

static std::string TALK_LOG = "\
 _____________________________________ \n\
< Input username and password for log >\n\
 ------------------------------------- \n";

static std::string TALK_SIGN = "\
 ______________________________________ \n\
< Input username and password for sign >\n\
 -------------------------------------- \n";

} // namespace vchat

#endif // LOG_HPP
