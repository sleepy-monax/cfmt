#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "macro_get_arg_count.h"
#include "macro_map.h"

typedef enum
{
    FORMAT_INT,
    FORMAT_DOUBLE,
    FORMAT_STRING,
} FormatValueType;

typedef struct
{
    FormatValueType type;

    union {
        int _int;
        double _double;
        const char *_string;
    };
} FormatValue;

static inline FormatValue fmtvali(int val)
{
    return (FormatValue){
        FORMAT_INT,
        {._int = val},
    };
}

static inline FormatValue fmtvald(double val)
{
    return (FormatValue){
        FORMAT_DOUBLE,
        {._double = val},
    };
}

static inline FormatValue fmtvals(const char *val)
{
    return (FormatValue){
        FORMAT_STRING,
        {._string = val},
    };
}

// clang-format off
#define SELECT_VALUE(__value) _Generic(\
    (__value), \
        int: fmtvali, \
        double: fmtvald,\
        char*: fmtvals)(__value),
// clang-format on

#define print_format(__format, __arguments...)           \
    __print_format(                                      \
        (__format),                                      \
        (FormatValue[]){MAP(SELECT_VALUE, __arguments)}, \
        GET_ARG_COUNT(__arguments))

typedef struct
{
    const char *format;
    size_t size;
    size_t offset;
} FormatParser;

static void next(FormatParser *parser)
{
    if (parser->offset < parser->size)
    {
        parser->offset++;
    }
}

static char current(FormatParser *parser)
{
    if (parser->offset < parser->size)
    {
        return parser->format[parser->offset];
    }
    else
    {
        return '\0';
    }
}

static inline size_t __print_format(
    const char *format,
    FormatValue *values,
    size_t count)
{
    FormatParser parser = (FormatParser){format, strlen(format), 0};

    size_t current_value = 0;

    while (current(&parser) != '\0')
    {
        if (current(&parser) == '{')
        {
            while (current(&parser) != '}' &&
                   current(&parser) != '\0')
            {
                next(&parser);
            }

            if (current_value < count)
            {
                switch (values[current_value].type)
                {
                case FORMAT_INT:
                    printf("%d", values[current_value]._int);
                    break;

                case FORMAT_DOUBLE:
                    printf("%f", values[current_value]._double);
                    break;

                case FORMAT_STRING:
                    printf("%s", values[current_value]._string);
                    break;

                default:
                    printf("<error>");
                    break;
                }

                current_value++;
            }
            else
            {
                printf("<error>");
            }
        }
        else
        {
            printf("%c", current(&parser));
        }

        next(&parser);
    }
}
