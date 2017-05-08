/**@file
 * @brief Buffer implementation for use in networking loggers.
 */
#ifndef UBUFFER
#define UBUFFER

#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

typedef struct {
    char *start;
    size_t head;
    size_t size;
    size_t capacity;
    size_t num_empty_bytes_at_end;
} uBuffer;

/**
 * The size of the memory that should be allocated for the uBuffer struct and passed to ubuffer_init().
 */
#define SIZEOF_UBUFFER sizeof(uBuffer)

/**
 * @brief Return codes for various uBuffer methods
 */
typedef enum {
    UBUFFER_SUCCESS = 0,
    UBUFFER_FULL,
    UBUFFER_EMPTY
} uBufferErrorCode;

/**
 * @brief Initialize a given uBuffer instance with the supplied buffer.
 *
 * @param ubuffer The buffer structure memory.
 * @param start Pointer to a start of the array to be used as  a buffer.
 * @param buffer_capacity The length of the previous array.
 * @returns UBUFFER_SUCCESS if successful, otherwise an error code is returned.
 */
uBufferErrorCode ubuffer_init(void *ubuffer, char *start, size_t buffer_capacity);

/**
 * @brief Allocates memory at the end of the buffer
 * @param ubuffer An initialized uBuffer instance
 * @param item A pointer to a pointer that will be initialized to an allocated memory
 * @param item_size The size of the memory to allocate
 * @return UBUFFER_SUCCESS if successful, otherwise an error code is returned.
 */
uBufferErrorCode ubuffer_allocate_next(void *ubuffer, void **item, size_t item_size);

/**
 * @brief Free item_size amount of memory from the beginning of the buffer
 * @param ubuffer An initialized uBuffer instance
 * @param item A pointer to the begining of the release memory
 * @param item_size The size of the memory to release
 * @return UBUFFER_SUCCESS if successful, otherwise an error code is returned.
 */
uBufferErrorCode ubuffer_free_first(void *ubuffer, void **item, size_t item_size);

/**
 * @brief Peeks at the first item in the buffer
 * @param ubuffer An initialized uBuffer instance
 * @param item A pointer to a pointer that will be initialized to the start of the first item
 * @param item_size The length of the item to be peeked
 * @return UBUFFER_SUCCESS if successful, otherwise an error code is returned.
 */
uBufferErrorCode ubuffer_peek_first(void *ubuffer, void **item, size_t item_size);

#endif //UBUFFER