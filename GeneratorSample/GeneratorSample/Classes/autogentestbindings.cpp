#include "autogentestbindings.hpp"
#include "cocos2d_specifics.hpp"
#include "simple_class.h"

template<class T>
static JSBool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	TypeTest<T> t;
	T* cobj = new T();
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	assert(p);
	JSObject *_tmp = JS_NewObject(cx, p->jsclass, p->proto, p->parentProto);
	js_proxy_t *pp;
	JS_NEW_PROXY(pp, cobj, _tmp);
	JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));

	return JS_TRUE;
}

static JSBool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	return JS_FALSE;
}


JSClass  *jsb_SimpleNativeClass_class;
JSObject *jsb_SimpleNativeClass_prototype;

JSBool js_autogentestbindings_SimpleNativeClass_getSomeField(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SimpleNativeClass* cobj = (SimpleNativeClass *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 0) {
		int ret = cobj->getSomeField();
		jsval jsret;
		jsret = int32_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}
JSBool js_autogentestbindings_SimpleNativeClass_log(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
	SimpleNativeClass* cobj = (SimpleNativeClass *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	if (argc == 1) {
		const char* arg0;
		std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
		cobj->log(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
	return JS_FALSE;
}
JSBool js_autogentestbindings_SimpleNativeClass_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		SimpleNativeClass* cobj = new SimpleNativeClass();
		TypeTest<SimpleNativeClass> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t *p;
		JS_NEW_PROXY(p, cobj, obj);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}




void js_autogentestbindings_SimpleNativeClass_finalize(JSFreeOp *fop, JSObject *obj) {
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JS_GET_NATIVE_PROXY(jsproxy, obj);
    if (jsproxy) {
        JS_GET_PROXY(nproxy, jsproxy->ptr);

        SimpleNativeClass *nobj = static_cast<SimpleNativeClass *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        JS_REMOVE_PROXY(nproxy, jsproxy);
    }
}

static JSBool js_autogentestbindings_SimpleNativeClass_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
	JSObject *obj = JS_THIS_OBJECT(cx, vp);
    SimpleNativeClass *nobj = new SimpleNativeClass();
    js_proxy_t* p;
    JS_NEW_PROXY(p, nobj, obj);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return JS_TRUE;
}

void js_register_autogentestbindings_SimpleNativeClass(JSContext *cx, JSObject *global) {
	jsb_SimpleNativeClass_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_SimpleNativeClass_class->name = "SimpleNativeClass";
	jsb_SimpleNativeClass_class->addProperty = JS_PropertyStub;
	jsb_SimpleNativeClass_class->delProperty = JS_PropertyStub;
	jsb_SimpleNativeClass_class->getProperty = JS_PropertyStub;
	jsb_SimpleNativeClass_class->setProperty = JS_StrictPropertyStub;
	jsb_SimpleNativeClass_class->enumerate = JS_EnumerateStub;
	jsb_SimpleNativeClass_class->resolve = JS_ResolveStub;
	jsb_SimpleNativeClass_class->convert = JS_ConvertStub;
	jsb_SimpleNativeClass_class->finalize = js_autogentestbindings_SimpleNativeClass_finalize;
	jsb_SimpleNativeClass_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("getSomeField", js_autogentestbindings_SimpleNativeClass_getSomeField, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("log", js_autogentestbindings_SimpleNativeClass_log, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_autogentestbindings_SimpleNativeClass_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
	};

	JSFunctionSpec *st_funcs = NULL;

	jsb_SimpleNativeClass_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_SimpleNativeClass_class,
		js_autogentestbindings_SimpleNativeClass_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "SimpleNativeClass", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<SimpleNativeClass> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_SimpleNativeClass_class;
		p->proto = jsb_SimpleNativeClass_prototype;
		p->parentProto = NULL;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_autogentestbindings(JSContext* cx, JSObject* obj) {

	js_register_autogentestbindings_SimpleNativeClass(cx, obj);
}

