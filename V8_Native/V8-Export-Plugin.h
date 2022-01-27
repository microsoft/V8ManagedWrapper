#pragma once

#ifndef _V8_COREUX_EXPORT_
#define _V8_COREUX_EXPORT_

#include "V8RuntimeImp-plugin.h"

/*
Initializes V8. This function needs to be called before the first Isolate.
This needs to call only once for a single app
*/

V8_API V8JsErrorType InitializeV8(_In_ int thread_pool_size, _In_ bool idleTaskSupport) {

    V8Platform::Initialize(thread_pool_size, idleTaskSupport);
    return V8JsErrorType::JsNoError;
}

/**
* Releases any resources used by v8 and stops any utility threads
* that may be running.  Note that disposing v8 is permanent, it
* cannot be reinitialized.
*
* It should generally not be necessary to dispose v8 before exiting
* a process, this should happen automatically. It is only necessary
* to use if the process needs the resources taken up by v8.
*/
V8_API V8JsErrorType DisposeV8Platform() {

    V8Platform::Dispose();
    return V8JsErrorType::JsNoError;
}

/**
This method will create a isolate, context object and wrapped by V8RuntimeImp class.
V8RuntimeImp class will allow to run script and dispose isolate object
*/
V8_API V8JsErrorType CreateV8RunTime(_In_ int max_old_space_in_MB, _In_ int max_semi_space_in_MB, _Out_ V8RuntimeHandler *runtimeHandler)
{
    PARAM_NOT_NULL(runtimeHandler);
    V8RuntimeImp* runtime = new V8RuntimeImp(max_old_space_in_MB, max_semi_space_in_MB);
    *runtimeHandler = runtime->ToHandle();
    return V8JsErrorType::JsNoError;
}

/**
 Disposing V8RuntimeImp will dipose isolate and context object related to this V8RuntimeHandler.
 During end of the request this method needs to call and release the memory that associate v8 isolate object.
*/
V8_API V8JsErrorType DisposeV8Runtime(_In_ V8RuntimeHandler runtimeHandler)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandler);
    V8RuntimeImp *runtime = V8RuntimeImp::FromHandle(runtimeHandler);
    delete runtime;
    return V8JsErrorType::JsNoError;
}

/**
Run javascript under current V8RuntimeHandler.
If *evalute* is "True" then it'll process the Script output object so managed code can access the output result by "result" reference.
*/
V8_API V8JsErrorType RunScript(_In_ V8RuntimeHandler runtimeHandler, _In_ const wchar_t *script, _In_ bool evalute, _Out_ V8JsValueRef *result) {

    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandler);
    if (script == nullptr)
    {
        *result = JS_INVALID_REFERENCE;
        return V8JsErrorType::NullArgument;
    }

    V8RuntimeImp *runtime = V8RuntimeImp::FromHandle(runtimeHandler);

    V8Value *value = nullptr;
    V8JsErrorType error = runtime->RunScript(script, evalute, value);
    if (error == V8JsErrorType::JsNoError) {
        *result = value->ToHandle();
    }
    else
    {
        *result = JS_INVALID_REFERENCE;
    }

    return error;
}

/**
* Optional notification that the embedder is idle.
* V8 uses the notification to perform garbage collection.
* This call can be used repeatedly if the embedder remains idle.
* Returns true if the embedder should stop calling IdleNotificationDeadline
* until real work has been done.  This indicates that V8 has done
* as much cleanup as it will be able to do.
*
* The deadline_in_seconds argument specifies the deadline V8 has to finish
* garbage collection work. deadline_in_seconds is compared with
* MonotonicallyIncreasingTime() and should be based on the same timebase as
* that function. There is no guarantee that the actual work will be done
* within the time limit.
*/
V8_API V8JsErrorType Idle(_In_ V8RuntimeHandler runtimeHandler) {

    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandler);
    V8RuntimeImp *runtime = V8RuntimeImp::FromHandle(runtimeHandler);

    if (runtime != nullptr)
    {
        runtime->IdleNotification();
        return V8JsErrorType::JsNoError;
    }
    else
    {
        return V8JsErrorType::InvalidRuntime;
    }
}

/**
* Optional notification that the system is running low on memory.
* V8 uses these notifications to attempt to free memory.
*/
V8_API V8JsErrorType CollectGarbage(_In_ V8RuntimeHandler runtimeHandler) {

    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandler);
    V8RuntimeImp *runtime = V8RuntimeImp::FromHandle(runtimeHandler);

    if (runtime != nullptr)
    {
        runtime->CollectGarbage();
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidRuntime;
}

/**
* It'll return the last exception that happened during executing the script. 
*/
V8_API V8JsErrorType GetException(_In_ V8RuntimeHandler runtimeHandler, _Out_ V8JsException *v8JsException) {
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandler);
    V8RuntimeImp *runtime = V8RuntimeImp::FromHandle(runtimeHandler);

    if (runtime != nullptr)
    {
        V8Exception *exception = runtime->GetLastException();
        *v8JsException = exception->ToHandle();
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidRuntime;
}

/*
* Return the Exception Message from V8JsException object
*/
V8_API V8JsErrorType GetExceptionMessage(_In_ V8JsException exception, _Out_ const wchar_t **message, _Out_ int& length) {
    VALIDATE_INCOMING_EXCEPTIONREF(exception);
    V8Exception *v8Exception = V8Exception::FromHandle(exception);

    if (v8Exception != nullptr && v8Exception->GetMessage(*message, length)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::NullReference;
}

/*
* Return the StackTrace from V8JsException object
*/
V8_API V8JsErrorType GetStackTrace(_In_ V8JsException exception, _Out_ const wchar_t **message, _Out_ int& length) {
    VALIDATE_INCOMING_EXCEPTIONREF(exception);
    V8Exception *v8Exception = V8Exception::FromHandle(exception);

    if (v8Exception != nullptr && v8Exception->GetStackTrace(*message, length)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::NullReference;
}

/*
* Get the value type of V8Value Object
* e.g: V8Value could be Null, Undefined, int, bool, string, arrary or object
*/
V8_API V8ValueType GetV8ValueType(_In_ V8JsValueRef v8JsValue)
{
    if (v8JsValue != JS_INVALID_REFERENCE)
    {
        V8Value *value = V8Value::FromHandle(v8JsValue);
        return value != nullptr ? value->GetType() : V8ValueType::UnknownType;
    }

    return V8ValueType::UnknownType;
}

/*
* Return the V8JsValueRef value as bool if the ValueType=V8ValueType::Boolean
*/
V8_API V8JsErrorType GetV8ValueAsBool(_In_ V8JsValueRef v8JsValue, _Out_ bool& output) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr && value->AsBool(output)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* Return the V8JsValueRef value as string if the ValueType=V8ValueType::String
*/
V8_API V8JsErrorType GetV8ValueAsString(_In_ V8JsValueRef v8JsValue, _Out_ const wchar_t **output, _Out_ int& length) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr && value->AsString(*output, length)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* Return the V8JsValueRef value as int if the ValueType=V8ValueType::Integer
*/
V8_API V8JsErrorType GetV8ValueAsInt(_In_ V8JsValueRef v8JsValue, _Out_ long& output) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr && value->AsInteger(output)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* Return the V8JsValueRef value as double if the ValueType=V8ValueType::Double
*/
V8_API V8JsErrorType GetV8ValueAsDouble(_In_ V8JsValueRef v8JsValue, _Out_ double& output) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr && value->AsDouble(output)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* Dispose the V8JsValueRef object and release all the memory
*/
V8_API V8JsErrorType DisposeV8Value(_In_ V8JsValueRef v8JsValue) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr) {
        delete value;
    }

    return V8JsErrorType::JsNoError;
}

/*
* if V8JsValueRef is an array then it will return the length
* else if V8JsValueRef is an object then it will return number of property it has
*/
V8_API V8JsErrorType GetLength(_In_ V8JsValueRef v8JsValue, _Out_ int& length) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr) {
        length = value->GetLength();
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* if V8JsValueRef is an object element then it will return the propery name
* e.g: {name: 'foo'} then it will return 'name';
*/
V8_API V8JsErrorType GetOwnPropertyName(_In_ V8JsValueRef v8JsValue, _Out_ const wchar_t **output, _Out_ int& length) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr && value->GetPropertyName(*output, length)) {
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}

/*
* if V8JsValueRef is an Array then it will return the element of that index
* else if V8JsValueRef is an object then it will return the property element (which contains [key as propertyname, value]) of that index 
*/
V8_API V8JsErrorType GetObjectByIndex(_In_ V8JsValueRef v8JsValue, _In_ int index, _Out_ V8JsValueRef *outPtr) {
    VALIDATE_INCOMING_V8JSVALUEREF(v8JsValue);
    V8Value *value = V8Value::FromHandle(v8JsValue);

    if (value != nullptr) {
        V8Value *object = value->Get(index);
        *outPtr = object->ToHandle();
        return V8JsErrorType::JsNoError;
    }

    return V8JsErrorType::InvalidOperation;
}


#endif