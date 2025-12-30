#ifndef __LOG_H
#define __LOG_H

#include <stdint.h>

void log_init(void);
void log_printf(const char *fmt, ...);
void log_task(void const *arg);

#endif
