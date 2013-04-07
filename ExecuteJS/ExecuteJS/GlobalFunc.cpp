//
//  GlobalFunc.cpp
//  ExecuteJS
//
//  Created by minggo on 4/3/13.
//  Copyright (c) 2013 minggo. All rights reserved.
//

#include "GlobalFunc.h"

/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u:%s\n",
            report->filename ? report->filename : "<no filename=\"filename\">",
            (unsigned int) report->lineno,
            message);
}
