#ifndef JUMPER_ULOGGER_COMPILERS_H_H
#define JUMPER_ULOGGER_COMPILERS_H_H

#if defined ( __CC_ARM )
#ifndef __PACKED
#define __PACKED __packed
#endif

#elif defined ( __ICCARM__ )
#ifndef __PACKED
#define __PACKED __packed
#endif

#elif defined ( __ICCARM__ )
#ifndef __PACKED
#define __PACKED __packed
#endif

#elif defined   ( __GNUC__ )
#ifndef __PACKED
#define __PACKED __attribute__((packed))
#endif

#elif defined(__TI_COMPILER_VERSION__)
#ifndef __PACKED
#define __PACKED __attribute__((packed))
#endif
#endif

#endif //JUMPER_ULOGGER_COMPILERS_H_H
