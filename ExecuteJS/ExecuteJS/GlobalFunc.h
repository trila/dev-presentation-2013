//
//  GlobalFunc.h
//  ExecuteJS
//
//  Created by minggo on 4/3/13.
//  Copyright (c) 2013 minggo. All rights reserved.
//

#ifndef __ExecuteJS__GlobalFunc__
#define __ExecuteJS__GlobalFunc__

#include "jsapi.h"
#include "jsfriendapi.h"

extern void reportError(JSContext *cx, const char *message, JSErrorReport *report);

#endif /* defined(__ExecuteJS__GlobalFunc__) */
