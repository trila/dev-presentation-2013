//
//  RegisterC++Class.cpp
//  ExecuteJS
//
//  Created by minggo on 4/3/13.
//  Copyright (c) 2013 minggo. All rights reserved.
//

#include "RegisterC++Class.h"

#include <string>
#include "jsapi.h"
#include "jsfriendapi.h"

#include "GlobalFunc.h"

class Father
{
public:
    void func1()
    {
        printf("father: func1 invoked\n");
    }
    
    virtual ~Father()
    {
        printf("father is destroyed\n");
    }
};

class Son : public Father
{
public:
    Son() : m1(1), m2(2){}
    ~Son()
    {
        printf("son is destroyed\n");
    }
    
    int func2(int a, int b)
    {
        printf("son: func2 invoked a=%d, b=%d\n", a, b);
        return (a + b);
    }
    
    static void func3()
    {
        printf("son: func3 invoked\n");
    }
    
public:
    int m1;
    int m2;
};

/* The class of the global object. */
static JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, NULL,
    JSCLASS_NO_OPTIONAL_MEMBERS
};


/***************************************************************************
 register father class
 ***************************************************************************/

static JSClass *father_class = NULL;
static JSObject *father_prototype = NULL;
// We only create an instance, in fact, every js object should have corresponding c++ object
static Father *father = NULL;
static JSObject *father_js = NULL;

static void father_finalize(JSFreeOp *fop, JSObject *obj)
{
    if (obj == father_js)
    {
        delete father;
        father = NULL;
    }
}

static JSBool func1_wrapper(JSContext *cx, uint32_t argc, jsval *vp)
{
    father->func1();
    return JS_TRUE;
}

static JSBool father_constructor(JSContext *cx, uint32_t  argc, jsval *vp)
{
    father = new Father();
    father_js = JS_NewObject(cx, father_class, father_prototype, NULL);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(father_js));
    
    return JS_TRUE;
}

static void register_father(JSContext *cx, JSObject *global)
{
    // register Father
    father_class = (JSClass*)calloc(1, sizeof(JSClass));
    father_class->name = "Father";
    father_class->addProperty = JS_PropertyStub;
    father_class->delProperty = JS_PropertyStub;
    father_class->getProperty = JS_PropertyStub;
    father_class->setProperty = JS_StrictPropertyStub;
    father_class->enumerate = JS_EnumerateStub;
    father_class->resolve = JS_ResolveStub;
    father_class->convert = JS_ConvertStub;
    father_class->finalize = father_finalize;
    father_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);
    
    static JSFunctionSpec father_funcs[] = {
        JS_FN("func1", func1_wrapper, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    father_prototype = JS_InitClass(cx, global,
                                    NULL,
                                    father_class,
                                    father_constructor,
                                    0,                     // number of args is 0
                                    NULL,                  // no property
                                    father_funcs,          // member function
                                    NULL,                  // no static property
                                    NULL                   // no static functions
                                    );
}


/***************************************************************************
 register son class
 ***************************************************************************/

static JSClass *son_class = NULL;
static JSObject *son_prototype = NULL;
static Son *son = NULL;
static JSObject *son_js = NULL;

static void son_finalize(JSFreeOp *fop, JSObject *obj)
{
    if (obj == son_js)
    {
        delete son;
        son = NULL;
    }
}

static JSBool func2_wrapper(JSContext *cx, uint32_t argc, jsval *vp)
{
    int arg0 = 0;
    int arg1 = 0;
    // parmater types: https://developer.mozilla.org/en-US/docs/SpiderMonkey/JSAPI_Reference/JS_ConvertArguments
    if (! JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "ii", &arg0, &arg1))
    {
        printf("error parameters");
        return JS_FALSE;
    }
    
    int result = son->func2(arg0, arg1);
    jsval jsret = INT_TO_JSVAL(result);
    JS_SET_RVAL(cx, vp, jsret);
    
    return JS_TRUE;
}

static JSBool func3_wrapper(JSContext *cx, uint32_t argc, jsval *vp)
{
    Son::func3();
    
    return JS_TRUE;
}

static JSBool son_constructor(JSContext *cx, uint32_t  argc, jsval *vp)
{
    son = new Son();
    son_js = JS_NewObject(cx, son_class, son_prototype, NULL);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(son_js));
    
    return JS_TRUE;
}

static void register_son(JSContext *cx, JSObject *global)
{
    // register Son
    son_class = (JSClass*)calloc(1, sizeof(JSClass));
    son_class->name = "Son";
    son_class->addProperty = JS_PropertyStub;
    son_class->delProperty = JS_PropertyStub;
    son_class->getProperty = JS_PropertyStub;
    son_class->setProperty = JS_StrictPropertyStub;
    son_class->enumerate = JS_EnumerateStub;
    son_class->resolve = JS_ResolveStub;
    son_class->convert = JS_ConvertStub;
    son_class->finalize = son_finalize;
    son_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);
    
    static JSFunctionSpec son_funcs[] = {
        JS_FN("func2", func2_wrapper, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    static JSFunctionSpec st_funcs[] = {
        JS_FN("func3", func3_wrapper, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    son_prototype = JS_InitClass(cx, global,
                                    father_prototype,
                                    son_class,
                                    son_constructor,
                                    0,                     // number of args is 0
                                    NULL,                  // no property
                                    son_funcs,             // member function
                                    NULL,                  // no static property
                                    st_funcs               // no static functions
                                    );
}

static void do_register(JSContext *cx, JSObject *global)
{
    register_father(cx, global);
    register_son(cx, global);
}

/* Execute javascript file. */
int registerClass()
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
    
    do_register(cx, global);
    
    /* Your application code here. This may include JSAPI calls
     * to create your own custom JavaScript objects and to run scripts.
     *
     * The following example code creates a literal JavaScript script,
     * evaluates it, and prints the result to stdout.
     *
     * Errors are conventionally saved in a JSBool variable named ok.
     */
    std::string script = "var son = new Son(); \
                          son.func1(); \
                          var result = son.func2(1, 1); \
                          son.func2(result, 3); \
                          Son.func3();";
    jsval rval;
    JSBool ok;
    const char *filename = "noname";
    unsigned int lineno = 0;
    
    ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(),
                           filename, lineno, &rval);
    if (ok == JS_FALSE)
        return 1;
    
    /* End of your application code */
    
    /* Clean things up and shut down SpiderMonkey. */
    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    
    return 0;
}

