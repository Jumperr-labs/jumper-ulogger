#ifndef JUMPER_ULOGGER_MOCK_CRITICAL_SECTION_H
#define JUMPER_ULOGGER_MOCK_CRITICAL_SECTION_H

void mock_enter_critical_section();

void mock_exit_critical_section();

int mock_critical_section_state();

int mock_critical_section_calls();

#endif //JUMPER_ULOGGER_MOCK_CRITICAL_SECTION_H
