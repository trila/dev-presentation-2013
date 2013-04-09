//
//  ExecuteJS.cpp
//  ExecuteJS
//
//  Created by minggo on 4/3/13.
//  Copyright (c) 2013 minggo. All rights reserved.
//

#include "ExecuteJS.h"

#include <string>
#include "jsapi.h"
#include "jsfriendapi.h"

#include "GlobalFunc.h"

/* The class of the global object. */
static JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, NULL,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

/* Execute javascript file. */
int executeJS()
{
    /* JSAPI variables. */
    JSRuntime *rt;
    JSContext *cx;
    JSObject  *global;
    
    /* Create a JS runtime. You always need at least one runtime per process. 
     * @parameter maxbytes: Maximum number of allocated bytes after which garbage collection is run.
     */
    rt = JS_NewRuntime(8 * 1024 * 1024);
    if (rt == NULL)
        return 1;
    
    /*
     * Create a context. You always need a context per thread.
     * Note that this program is not multi-threaded.
     * @parameter stackchunksize: The size, in bytes, of each "stack chunk". 
     *                            This is a memory management tuning parameter which most users should not adjust. 8192 is a good default value.
     */
    cx = JS_NewContext(rt, 8192);
    if (cx == NULL)
        return 1;
    JS_SetOptions(cx, JSOPTION_VAROBJFIX | JSOPTION_METHODJIT);
    JS_SetVersion(cx, JSVERSION_LATEST);
    JS_SetErrorReporter(cx, reportError);
    
    /*
     * Create the global object in a new compartment.
     * You always need a global object per context.
     */
    global = JS_NewGlobalObject(cx, &global_class, NULL);
    if (global == NULL)
        return 1;
    
    /*
     * Populate the global object with the standard JavaScript
     * function and object classes, such as Object, Array, Date.
     */
    if (!JS_InitStandardClasses(cx, global))
        return 1;
    
    /* Your application code here. This may include JSAPI calls
     * to create your own custom JavaScript objects and to run scripts.
     *
     * The following example code creates a literal JavaScript script,
     * evaluates it, and prints the result to stdout.
     *
     * Errors are conventionally saved in a JSBool variable named ok.
     */
    std::string script = "'hello world'";
    jsval rval;
    JSString *str;
    JSBool ok;
    const char *filename = "noname";
    unsigned int lineno = 0;
    
    ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(),
                           filename, lineno, &rval);
    if (ok == JS_FALSE)
        return 1;
    
    str = JS_ValueToString(cx, rval);
    printf("%s\n", JS_EncodeString(cx, str));
    
    /* End of your application code */
    
    /* Clean things up and shut down SpiderMonkey. */
    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    
    return 0;
}
