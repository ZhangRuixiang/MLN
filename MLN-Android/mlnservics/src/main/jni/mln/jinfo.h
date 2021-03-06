/**
  * Created by MomoLuaNative.
  * Copyright (c) 2019, Momo Group. All rights reserved.
  *
  * This source code is licensed under the MIT.
  * For the full copyright and license information,please view the LICENSE file in the root directory of this source tree.
  */
//
// Created by Xiong.Fangyu 2019/03/13.
//

#ifndef J_INFO_H
#define J_INFO_H

#include "global_define.h"
#include "juserdata.h"
#include "map.h"

#define USE_NDK_NEWSTRING_VERSION 23
#define MAX_EXCEPTION_MSG 300
#define EXCEPTION_STACK_LEN 20

#define JAVA_CONSTRUCTOR "<init>"
#define JAVA_VALUE_OF "valueOf"

#define findTypeClass(env, type) (*env)->FindClass(env, JAVA_PATH "" type)
#define findConstructor(env, type, sig) (*env)->GetMethodID(env, type, JAVA_CONSTRUCTOR, "(" sig ")V")

#define GetArrLen(env, arr) (int)((*env)->GetArrayLength(env, arr))

jstring newJString(JNIEnv *, const char *);

void initJavaInfo(JNIEnv *);

void copyUDToGNV(JNIEnv *env, lua_State *L, UDjavaobject ud, int idx, jobject jobj);

jobject newLuaNumber(JNIEnv *, jdouble);

jobject newLuaString(JNIEnv *, const char *);

jobject newLuaTable(JNIEnv *, lua_State *, int);

jobject newLuaFunction(JNIEnv *, lua_State *, int);

jobject newLuaUserdata(JNIEnv *, lua_State *, int, UDjavaobject);

#if defined(JAVA_CACHE_UD)
jlong getUserdataId(JNIEnv *env, jobject ud);
#endif

jobject getUserdata(JNIEnv *env, lua_State *L, UDjavaobject ud);

jobject newLuaThread(JNIEnv *env, lua_State *L, int idx);

/**
 * 把java的userdata对象push到栈上
 */
void pushUserdataFromJUD(JNIEnv *env, lua_State *L, jobject obj);

/**
 * 将栈中idx位置的数据转为java中的LuaValue
 * idx可为正数或负数
 */
jobject toJavaValue(JNIEnv *env, lua_State *L, int idx);

/**
 * 调用函数时使用，将栈中所有的数据读成LuaValue，并保存为对象
 */
jobjectArray newLuaValueArrayFromStack(JNIEnv *env, lua_State *L, int count, int stackoffset);

/**
 * 将LuaValue对象obj转成lua数据类型，并push到栈顶
 */
void pushJavaValue(JNIEnv *env, lua_State *L, jobject obj);

/**
 * 函数返回时使用，将java方法结果转成lua数据，push到栈中，并返回参数个数
 */
int pushJavaArray(JNIEnv *env, lua_State *L, jobjectArray arr);

/**
 * 抛出调用异常
 */
void throwInvokeError(JNIEnv *env, const char *errmsg);

/**
 * 超出运行时异常
 */
void throwRuntimeError(JNIEnv *env, const char *msg);

/**
 * lua调用gc时调用
 */
void callbackLuaGC(JNIEnv *env, lua_State *L);


#define OTHER_ERROR     -1
#define STATE_DESTROYED -2
#define STATE_NONE_LOOP -3
#define POST_SUCCESS     0

typedef int (*callback_method)(lua_State *L, void *ud);

/**
 * 在虚拟机线程回调method方法
 * @return OTHER_ERROR STATE_DESTROYED STATE_NONE_LOOP POST_SUCCESS
 */
int postCallback(JNIEnv *env, lua_State *L, callback_method method, void *arg);

/**
 * 获取java环境
 * 若需要自行调用detach,返回1,否则返回0
 */
int getEnv(JNIEnv **out);

/**
 * detach
 */
void detachEnv();

/**
 * 获取异常信息
 * @return 0 成功
 */
int getThrowableMsg(JNIEnv *, jthrowable, char *, size_t);

/**
 * 若java有异常，捕获，并在栈上增加异常信息
 * @return 1: 有异常，0: 无异常
 */
int catchJavaException(JNIEnv *, lua_State *, const char *);

/**
 * 根据名称获取jclass对象
 * 若有缓存，取缓存，若无，通过反射获取，并缓存
 */
jclass getClassByName(JNIEnv *, const char *);

/**
 * 获取构造函数
 * 若有缓存，取缓存，若无，通过反射获取，并缓存
 */
jmethodID getConstructor(JNIEnv *env, jclass clz);

/**
 * 根据名称获取方法对象
 * 若有缓存，取缓存，若无，通过反射获取，并缓存
 */
jmethodID getMethodByName(JNIEnv *env, jclass clz, const char *name);

/**
 * 根据名称获取静态方法对象
 * 若有缓存，取缓存，若无，通过反射获取，并缓存
 */
jmethodID getStaticMethodByName(JNIEnv *env, jclass clz, const char *name);

/**
 * 遍历clz中所有的方法
 */
void traverseAllMethods(jclass clz, map_look_fun fun, void *ud);

#define METHOD_INDEX    0
#define METHOD_NEWINDEX 1
#define METHOD_TOSTRING 2
#define METHOD_EQAULS   3
#define METHOD_GC       4

static const char *special_methods[] = {
        "__index", "__newindex", "toString", "__onLuaEq", "__onLuaGc", NULL
};

/**
 * 获取特殊函数
 */
jmethodID getSpecialMethod(JNIEnv *env, jclass clz, int type);

/**
 * 获取静态index函数
 */
jmethodID getIndexStaticMethod(JNIEnv *env, jclass clz);

#endif //J_INFO_H