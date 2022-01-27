#pragma once

#ifndef _V8_COREUX_RUNTIME_
#define _V8_COREUX_RUNTIME_

#include "V8Platform-plugin.h"
#include "V8-plugin.h"

/*
* V8RuntimeImp is wrapper of V8 Isolate and Context.
* This will allow to create V8 Isolate object for embedded Application and execute script.
* An Isolate can create multiple context but this wrapper will always create one context under
* one Isolate object. That means all code that execute under this isolate object can access previous
* executed variable or state.
*/

class V8RuntimeImp
{
public:
    V8RuntimeImp() : V8RuntimeImp(0, 0) {}
    V8RuntimeImp(size_t max_old_space, size_t max_semi_space);
    ~V8RuntimeImp();

    V8JsErrorType RunScript(const wchar_t *script, bool evaluate, V8Value*& result);

    V8Exception* GetLastException();

    void CollectGarbage();

    bool IdleNotification();

    V8RuntimeHandler ToHandle() { return static_cast<V8RuntimeHandler>(this); }

    static V8RuntimeImp * FromHandle(V8RuntimeHandler runtimeHandle)
    {
        V8RuntimeImp *runtime = static_cast<V8RuntimeImp *>(runtimeHandle);
        return runtime;
    }

private:
    void SetException(V8Exception* exception);

    v8::Isolate* isolate;
    v8::Persistent<v8::Context> globalContext;
    V8Exception *lastException = nullptr;
};

V8RuntimeImp::V8RuntimeImp(size_t max_old_space_in_MB, size_t max_semi_space_in_MB)
{
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = V8ArrayBufferAllocatorInstance;
    if (max_old_space_in_MB > 0)
    {
        create_params.constraints.set_max_old_generation_size_in_bytes(max_old_space_in_MB * OneMBSizeInByte);
    }

    if (max_semi_space_in_MB > 0)
    {
        create_params.constraints.set_max_young_generation_size_in_bytes(max_semi_space_in_MB * OneMBSizeInByte);
    }

    // An Isolate is an independant copy of the V8 runtime which includes its own heap.
    // Two different Isolates can run in parallel and can be seen as entierly different
    // sandboxed instances of a V8 runtime.
    this->isolate = v8::Isolate::New(create_params);
    {
        v8::Locker locker(this->isolate);
        v8::Isolate::Scope isolate_scope(this->isolate);
        {
            // Create a stack-allocated handle scope.
            v8::HandleScope handle_scope(this->isolate);
            {
                v8::Local<v8::Context> context = v8::Context::New(this->isolate);
                this->globalContext.Reset(this->isolate, context);
            }
        }
    }
}

V8RuntimeImp::~V8RuntimeImp()
{
    if (this->lastException != nullptr) {
        delete this->lastException;
    }

    this->globalContext.Reset();
    this->isolate->Dispose();
    this->isolate = nullptr;
    Log("destroying V8RuntimeImp \n");
}

V8JsErrorType V8RuntimeImp::RunScript(const wchar_t *script_st, bool evaluate, V8Value*& result)
{
    v8::Locker locker(this->isolate);
    v8::Isolate::Scope isolate_scope(this->isolate);
    {
        // Create a stack-allocated handle scope
        v8::HandleScope handle_scope(this->isolate);
        {
            v8::Local<v8::Context> context = v8::Local<v8::Context>::New(this->isolate, this->globalContext);
            // Enter the context for compiling and running the script.
            v8::Context::Scope context_scope(context);
            {
                v8::TryCatch try_catch(this->isolate);

                v8::Local<v8::String> source = v8::String::NewFromTwoByte(this->isolate, reinterpret_cast<const uint16_t*>(script_st), v8::NewStringType::kNormal, -1)
                    .ToLocalChecked();

                // Compile the source code.
                v8::Local<v8::Script> script;
                if (!v8::Script::Compile(context, source).ToLocal(&script))
                {
                    this->SetException(new V8Exception(this->isolate, context, try_catch));
                    return V8JsErrorType::ScriptCompilationError;
                }

                // Run the script to get the result.
                v8::MaybeLocal<v8::Value> maybeResult = script->Run(context);

                if (evaluate)
                {
                    if (maybeResult.IsEmpty()) {
                        this->SetException(new V8Exception(this->isolate, context, try_catch));
                        return V8JsErrorType::ScriptExecutionException;
                    }
                    else
                    {
                        v8::Local<v8::Value> tempResult = maybeResult.ToLocalChecked();
                        result = new V8Value(this->isolate, context, tempResult);
                    }
                }
            }
        }
    }

    return V8JsErrorType::JsNoError;
}

bool V8RuntimeImp::IdleNotification()
{
    return this->isolate->IdleNotificationDeadline(V8Platform::MonotonicallyIncreasingTime());
}

void V8RuntimeImp::CollectGarbage()
{
    this->isolate->LowMemoryNotification();
}

V8Exception* V8RuntimeImp::GetLastException()
{
    return this->lastException;
}

void V8RuntimeImp::SetException(V8Exception* exception)
{
    if (this->lastException != nullptr) {
        delete this->lastException;
    }

    this->lastException = exception;
}

#endif