#ifndef JUMPER_ULOGGER_LOGGING_CONFIG_H
#define JUMPER_ULOGGER_LOGGING_CONFIG_H

#include "mock_critical_section.h"

#define ULOGGER_ENTER_CRITICAL_SECTION() mock_enter_critical_section()
#define ULOGGER_EXIT_CRITICAL_SECTION() mock_exit_critical_section()

#endif //JUMPER_ULOGGER_LOGGING_CONFIG_H
