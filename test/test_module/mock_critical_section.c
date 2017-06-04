#include "mock_critical_section.h"

static int is_critical_section;
static int critical_section_calls = 0;


void mock_enter_critical_section() {
    is_critical_section = 1;
    critical_section_calls++;
}

void mock_exit_critical_section() {
    is_critical_section = 0;
}

int mock_critical_section_calls() {
    return critical_section_calls;
}

int mock_critical_section_state() {
    return is_critical_section;
}