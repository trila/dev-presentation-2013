#ifndef __autogentestbindings_h__
#define __autogentestbindings_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_SimpleNativeClass_class;
extern JSObject *jsb_SimpleNativeClass_prototype;

JSBool js_autogentestbindings_SimpleNativeClass_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_autogentestbindings_SimpleNativeClass_finalize(JSContext *cx, JSObject *obj);
void js_register_autogentestbindings_SimpleNativeClass(JSContext *cx, JSObject *global);
void register_all_autogentestbindings(JSContext* cx, JSObject* obj);
JSBool js_autogentestbindings_SimpleNativeClass_getSomeField(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_autogentestbindings_SimpleNativeClass_log(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_autogentestbindings_SimpleNativeClass_SimpleNativeClass(JSContext *cx, uint32_t argc, jsval *vp);
#endif

