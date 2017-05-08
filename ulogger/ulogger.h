/**
 * @file
 * @brief Main uLogger interface.
 *
 * @defgroup ulogger_core uLogger Core Module
 * @{
 */
#ifndef ULOGGER
#define ULOGGER

#include <stdint.h>
#include <stdlib.h>

/**
 * @mainpage Jumper Ulogger
 * @section Introduction
 * This is the introduction
 *
 * @section
 * Documentation: ulogger.h
 */

#include "ulogger_events.h"

/**
 * @brief Return codes for various ulogger methods.
 */
typedef enum {
    ULOGGER_SUCCESS = 0,
    ULOGGER_FAIL
} uLoggerErrorCode;

typedef uint32_t EventType;

/**
 * Macro to conveniently define events with no additional data.
 */
#define ULOGGER_LOG(logger, level, event) ulogger_log(logger, level, event, 0, 0);

/**
 * @brief Logging levels for events.
 */
typedef enum {
    ULOGGER_DEBUG = 0,
    ULOGGER_INFO,
    ULOGGER_WARNING,
    ULOGGER_ERROR,
    ULOGGER_FATAL
} LogLevel;

typedef enum {
    HANDLER_SUCCESS = 0,
    HANDLER_FAIL
} HandlerReturnType;

typedef uint32_t timestamp;

typedef HandlerReturnType (*handler_func)(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);

/**
 * @brief The data structure for storing the ulogger context.
 */
typedef struct {
    handler_func* handlers;
    void** handlers_data;
    size_t num_handlers;
} uLogger;

#define SIZEOF_ULOGGER sizeof(uLogger)

void get_timestamp(timestamp *data);

/**
 * @brief Initializes the supplied uLogger structure
 * @param ulogger An allocated memory in the size of SIZEOF_ULOGGER
 * @param handlers A pointer to an array of logging handlers, handlers publish the data passed to the ulogger, in ulogger_log().
 * @param handlers_data Handlers context, a handler can define a context that will be supplied when passing log data. For example
 * it can be useful in handlers relating to networking.
 * @param num_handlers
 * @return HANDLER_SUCCESS when successful, error code otherwise.
 */
uLoggerErrorCode ulogger_init(void *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers);

/**
 *
 * @param ulogger
 * @param level
 * @param event_type
 * @param log_data
 * @param data_length
 * @return HANDLER_SUCCESS when successful, error code otherwise.
 */
uLoggerErrorCode ulogger_log(void *ulogger, LogLevel level, EventType event_type, void * log_data, size_t data_length);

#endif // ULOGGER

/**
 * @}
*/