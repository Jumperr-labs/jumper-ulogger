/**@file
 * @brief Hlper methods for json encoding.
 * @ingroup cc3200_platform
 * @{
 */
#ifndef JSON_ENCODING_HELPER_H
#define JSON_ENCODING_HELPER_H

#define START_ARRAY(buf, name) \
    do { \
        (buf)+= sprintf((buf), "\"%s\": [", name); \
    } while(0);

#define END_ARRAY(buf) \
    do { \
        (buf)+= sprintf((buf), "], "); \
    } while(0);

#define START_OBJECT(buf) \
    do { \
        (buf)+= sprintf((buf), "{"); \
    } while(0);

#define END_OBJECT(buf) \
    do { \
        (buf) -= 2; \
        (buf) += sprintf((buf), "}"); \
    } while(0);

#define PACK_NAME_AND_INT(buf, obj, param) \
        (buf) += sprintf((buf), "\"%s\": %d, ", #param, (obj)->param)


#define PACK_NAME_AND_STRING(buf, obj, param) \
        (buf) += sprintf((buf), "\"%s\": \"%s\", ", #param, (obj)->param)

#define PACK_NAME_AND_MAC_ADDRESS(buf, field_name, mac) \
        (buf) += sprintf((buf), "\"%s\": \"%x:%x:%x:%x:%x:%x\", ", field_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5])

#define PACK_EVENT_NAME(name) #name

#endif //JSON_ENCODING_HELPER_H
/**
/* @}
 */
