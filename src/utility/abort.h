//
//  error_handler.h
//  Fermion_Simulator
//
//  Created by Kun Chen on 10/11/14.
//  Copyright (c) 2014 Kun Chen. All rights reserved.
//

#ifndef __Fermion_Simulator__error_handler__
#define __Fermion_Simulator__error_handler__

#include "logger.h"
#include "convention.h"
#include <exception>

#define EXCEPTION(name)                                        \
    class name : public std::runtime_error {                   \
    public:                                                    \
        name(const std::string& msg) : std::runtime_error(msg) \
        {                                                      \
        }                                                      \
    };

EXCEPTION(IOInvalid);
EXCEPTION(TypeInvalid);
EXCEPTION(KeyInvalid);
EXCEPTION(ValueInvalid);
EXCEPTION(IndexInvalid);
EXCEPTION(MemoryException);
EXCEPTION(RunTimeException);

#define THROW(exception, msg, priority)                                             \
    do {                                                                            \
        std::ostringstream ss;                                                      \
        ss << msg;                                                                  \
        LOGGER(priority, "Exception throwed: " << ss.str());                        \
        std::ostringstream __stream_more__;                                         \
        __stream_more__ << "@[" << __FILE__ << ":" << __LINE__ << "] " << ss.str(); \
        throw exception(__stream_more__.str());                                     \
    } while (0)

#define THROW_ERROR(exception, msg) \
    THROW(exception, msg, ERROR)

#define ABORT(msg) \
    THROW(RunTimeException, msg, ERROR)

#define ERRORCODEABORT(errorcode, msg) \
    do {                               \
        LOG_ERROR(msg);                \
        throw(errorcode);              \
    } while (0)

#define ASSERT_ALLWAYS(expression, msg)                   \
    do {                                                  \
        if ((expression) == false)                        \
            ABORT(#expression " does not hold! " << msg); \
    } while (0)

#endif /* defined(__Fermion_Simulator__error_handler__) */
