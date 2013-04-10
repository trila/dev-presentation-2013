//
//  InvokeJSFunc.cpp
//  ExecuteJS
//
//  Created by minggo on 4/3/13.
//  Copyright (c) 2013 minggo. All rights reserved.
//

#include "InvokeJSFunc.h"

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

static JSBool executeFunctionWithOwner(JSContext *cx, JSObject* owner, const char *name)
{
    JSBool bRet = JS_FALSE;
    JSBool hasAction;
    jsval temp_retval;
    JSObject* obj = owner;
    
    do
    {
        if (JS_HasProperty(cx, obj, name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, name, &temp_retval)) {
                break;
            }
            if(temp_retval == JSVAL_VOID) {
                break;
            }
            
            JSAutoCompartment ac(cx, obj);

            jsval jsret;
            JSString *str = JS_NewStringCopyZ(cx, "hello world");
            jsval arg = STRING_TO_JSVAL(str);
            JS_CallFunctionName(cx, obj, name, 1, &arg, &jsret);
        }
    }while(0);
    return bRet;
}

static JSBool log(JSContext* cx, uint32_t argc, jsval *vp)
{
    if (argc > 0) {
        JSString *string = NULL;
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "S", &string);
        if (string) {
            printf("%s\n", JS_EncodeString(cx, string));
        }
    }
    return JS_TRUE;
}

int invokeJSFunction()
{
    /* JSAPI variables. */
    JSRuntime *rt;
    JSContext *cx;
    JSObject  *global;
    
    /* Create a JS runtime. You always need at least one runtime per process. */
    rt = JS_NewRuntime(8 * 1024 * 1024);
    if (rt == NULL)
        return 1;
    
    /*
     * Create a context. You always need a context per thread.
     * Note that this program is not multi-threaded.
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
    
    JS_DefineFunction(cx, global, "log", log, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    
    /* Your application code here. This may include JSAPI calls
     * to create your own custom JavaScript objects and to run scripts.
     *
     * The following example code creates a literal JavaScript script,
     * evaluates it, and prints the result to stdout.
     *
     * Errors are conventionally saved in a JSBool variable named ok.
     */
    std::string script = "function js_func(s) { log('it is js function ' + s);}";
    jsval rval;
    JSString *str;
    JSBool ok;
    const char *filename = "noname";
    unsigned int lineno = 0;
    
    ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(),
                           filename, lineno, &rval);
    if (ok == JS_FALSE)
        return 1;
    
    
    executeFunctionWithOwner(cx, global, "js_func");
    
    
    /* End of your application code */
    
    /* Clean things up and shut down SpiderMonkey. */
    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    
    return 0;
}
