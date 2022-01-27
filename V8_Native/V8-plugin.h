#pragma once

#ifndef _V8_COREUX_PLUGIN_
#define _V8_COREUX_PLUGIN_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libplatform/libplatform.h"
#include "v8.h"

#ifdef _In_
#undef _In_
#endif

#ifdef _Out_
#undef _Out_
#endif

#define _In_
#define _Out_

#ifndef _EXPORT_TO_MANAGEWORLD_
#define _EXPORT_TO_MANAGEWORLD_
#endif

#ifdef PrintLog
#define Log(msg) printf(msg);
#else
#define Log(msg)
#endif
#ifdef __linux
#define V8_API extern "C"
#elif _WIN32
#define V8_API extern "C" __declspec(dllexport)
#endif

#define PARAM_NOT_NULL(p) \
    if (p == nullptr) \
    { \
        return V8JsErrorType::NullArgument; \
    }

#define VALIDATE_INCOMING_RUNTIME_HANDLE(p) \
        { \
            if (p == JS_INVALID_RUNTIME_HANDLE) \
            { \
                return V8JsErrorType::NullArgument; \
            } \
        }

#define VALIDATE_INCOMING_V8JSVALUEREF(p) \
        { \
            if (p == JS_INVALID_REFERENCE) \
            { \
                return V8JsErrorType::NullArgument; \
            } \
        }

#define VALIDATE_INCOMING_EXCEPTIONREF(p) \
        { \
            if (p == JS_INVALID_EXCEPTION_REFERENCE) \
            { \
                return V8JsErrorType::NullArgument; \
            } \
        }

#define OneMBSizeInByte 1000000

typedef void * V8RuntimeHandler;

/// <summary>
///     An invalid runtime handle.
/// </summary>
const V8RuntimeHandler JS_INVALID_RUNTIME_HANDLE = 0;

typedef void * V8JsValueRef;

/// <summary>
///     An invalid V8JsValue reference.
/// </summary>
const V8JsValueRef JS_INVALID_REFERENCE = 0;

typedef void * V8JsException;
/// <summary>
///     An invalid V8JsValue reference.
/// </summary>
const V8JsException JS_INVALID_EXCEPTION_REFERENCE = 0;

_EXPORT_TO_MANAGEWORLD_ typedef enum _JsErrorType {
    JsNoError = 0,
    Exception = 1,
    NullArgument = 2,
    NullReference = 3,
    ScriptCompilationError = 4,
    ScriptExecutionException = 5,
    InvalidOperation = 6,
    InvalidRuntime = 7

}V8JsErrorType;


_EXPORT_TO_MANAGEWORLD_ typedef enum _V8ValueType {
    Null = 0,
    Undefined = 1,
    Boolean = 2,
    Integer = 3,
    Double = 4,
    String = 5,
    Object = 6,
    Array = 7,
    UnknownType = 8
} V8ValueType;

union Data
{
    bool bValue;
    long  lValue;
    double dValue;
    v8::String::Value* sValue;
};

/*
* V8Value will represent JavaScript Object for V8 engine. If Javascript value type is native value then
* GetType() will return the Type and Data will contain the native value;
* If Javascript value is an array or Object then GetType() will return the proper type and
* Array Index/ Object Property value can be accessed by  Get(int index) method.
* GetLength() will return the Array Size or object Property size
*/
class V8Value {
public:
    V8Value()
    {
        this->type = V8ValueType::UnknownType;
    }

    V8Value(v8::Isolate* isolate, v8::Local<v8::Context>& context, v8::Local<v8::Value>& propertyVal, v8::Local<v8::Value>& val)
        : V8Value(isolate, context, val)
    {
        this->propertyName = new v8::String::Value(isolate, propertyVal);
    }

    V8Value(v8::Isolate* isolate, v8::Local<v8::Context>& context, v8::Local<v8::Value>& val) {
        this->valuesLength = 0;

        if (val.IsEmpty() || val->IsNull()) {
            this->type = V8ValueType::Null;
        }
        else if (val->IsUndefined()) {
            this->type = V8ValueType::Undefined;
        }
        else if (val->IsBoolean()) {
            this->type = V8ValueType::Boolean;
            this->value.bValue = val->BooleanValue(isolate);
        }
        else if (val->IsInt32()) {
            this->type = V8ValueType::Integer;
            this->value.lValue = val->Int32Value(context).FromMaybe(defaultIntegerValue);
        }
        else if (val->IsUint32()) {
            this->type = V8ValueType::Integer;
            this->value.lValue = val->Uint32Value(context).FromMaybe(defaultIntegerValue);
        }
        else if (val->IsNumber()) {
            this->type = V8ValueType::Double;
            this->value.dValue = val->NumberValue(context).FromMaybe(defaultIntegerValue);
        }
        else if (val->IsString()) {
            this->type = V8ValueType::String;
            this->value.sValue = new v8::String::Value(isolate, val);
        }
        else if (val->IsArray()) {
            this->type = V8ValueType::Array;
            v8::HandleScope handle_scope(isolate);
            {
                v8::Local<v8::Array> t_array = val.As<v8::Array>();
                if (!t_array.IsEmpty()) {
                    this->valuesLength = t_array->Length();
                    if (this->valuesLength > 0)
                    {
                        this->values = new V8Value*[this->valuesLength];
                        for (int index = 0; index < this->valuesLength; index++) {
                            v8::Local<v8::Value> tempValue = t_array->Get(context, index).ToLocalChecked();
                            this->values[index] = new V8Value(isolate, context, tempValue);
                        }
                    }
                }
            }
        }
        else if (val->IsObject()) {
            this->type = V8ValueType::Object;
            v8::HandleScope handle_scope(isolate);
            {
                v8::Local<v8::Object> t_object = val.As<v8::Object>();
                if (!t_object.IsEmpty()) {
                    v8::Local<v8::Array> properties = t_object->GetPropertyNames(context).ToLocalChecked();
                    this->valuesLength = properties->Length();
                    if (this->valuesLength > 0)
                    {
                        this->values = new V8Value*[this->valuesLength];
                        for (int index = 0; index < this->valuesLength; index++) {
                            v8::Local<v8::Value> key = properties->Get(context, index).ToLocalChecked();
                            v8::Local<v8::Value> tempValue = t_object->Get(context, key).ToLocalChecked();
                            this->values[index] = new V8Value(isolate, context, key, tempValue);
                        }
                    }
                }
            }
        }
        else {
            this->type = V8ValueType::UnknownType;
        }
    };

    ~V8Value() {
        if (this->type == V8ValueType::String) {
            delete this->value.sValue;
        }
        else if (this->type == V8ValueType::Array || this->type == V8ValueType::Object)
        {
            for (int index = 0; index < this->valuesLength; index++) {
                delete this->values[index];
            }

            if(this->valuesLength > 0 && this->values != nullptr )
            {
                delete[] this->values;
            }
        }

        if (this->propertyName != nullptr) {
            delete this->propertyName;
        }

        Log(" -- destroying V8Value --\n");
    }

    bool AsInteger(long& result) const
    {
        if (this->type == V8ValueType::Integer) {
            result = value.lValue;
            return true;
        }

        return false;
    }

    bool AsDouble(double& result) const
    {
        if (this->type == V8ValueType::Double) {
            result = value.dValue;
            return true;
        }

        return false;
    }

    bool AsBool(bool& result) const
    {
        if (this->type == V8ValueType::Boolean) {
            result = value.bValue;
            return true;
        }

        return false;
    }

    bool AsString(const wchar_t*& result, int& length) const
    {
        if (this->type == V8ValueType::String) {
            result = reinterpret_cast<const wchar_t*>(**this->value.sValue);
            length = this->value.sValue->length();
            return true;
        }

        return false;
    }

    V8Value* Get(int index)
    {
        if ((this->type == V8ValueType::Array || this->type == V8ValueType::Object) && index < this->valuesLength) {
            return this->values[index];
        }

        return nullptr;
    }

    bool GetPropertyName(const wchar_t*& result, int& length) {
        if (this->propertyName == nullptr) return false;

        result = reinterpret_cast<const wchar_t*>(**this->propertyName);
        length = this->propertyName->length();
        return true;
    }

    int GetLength() {
        return this->valuesLength;
    }

    V8ValueType GetType() {
        return this->type;
    }

    V8JsValueRef ToHandle() { return static_cast<V8JsValueRef>(this); }

    static V8Value* FromHandle(V8JsValueRef v8JsValue)
    {
        V8Value *v8Value = static_cast<V8Value *>(v8JsValue);
        return v8Value;
    }

private:
    V8ValueType type;
    Data value;
    v8::String::Value* propertyName = nullptr;
    V8Value **values;
    int valuesLength;
    const int defaultIntegerValue = 0;
};

/*
* This will represent the V8 Exception. The Exception can be happened during compile the script or executing the script.
* Currently Exception class expose couple of property - 1) Message &  2) StackTrace
*/
class V8Exception
{
public:
    V8Exception(v8::Isolate* pIsolate, v8::Local<v8::Context>& context, const v8::TryCatch& exception)
    {
        v8::HandleScope handle_scope(pIsolate);
        {
            this->message = new v8::String::Value(pIsolate, exception.Exception());
            this->stackTrace = nullptr;

            v8::MaybeLocal<v8::Value> m_stackTrack = exception.StackTrace(context);
            if (!m_stackTrack.IsEmpty())
            {
                v8::Local<v8::Value> l_stacktrace;
                if (m_stackTrack.ToLocal(&l_stacktrace)) {
                    this->stackTrace = new v8::String::Value(pIsolate, l_stacktrace);
                }
            }
        }
    }

    ~V8Exception() {
        delete this->message;

        if (this->stackTrace != nullptr)
        {
            delete this->stackTrace;
        }

        Log(" __ destroying exception __\n");
    }

    bool GetMessage(const wchar_t*& i_message, int& length) {
        if (this->message == nullptr || this->message->length() == 0) return false;

        i_message = reinterpret_cast<const wchar_t*>(**this->message);
        length = this->message->length();
        return true;
    }

    bool GetStackTrace(const wchar_t*& i_stackTrace, int& length) {
        if (this->stackTrace == nullptr || this->stackTrace->length() == 0) return false;

        i_stackTrace = reinterpret_cast<const wchar_t*>(**this->stackTrace);
        length = this->stackTrace->length();
        return true;
    }

    V8JsException ToHandle() { return static_cast<V8JsException>(this); }

    static V8Exception* FromHandle(V8JsException v8JsException)
    {
        V8Exception *exception = static_cast<V8Exception *>(v8JsException);
        return exception;
    }

private:

    v8::String::Value* message;
    v8::String::Value* stackTrace;
};

#endif