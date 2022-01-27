#pragma once

#ifndef _V8_COREUX_PLATFORM_
#define _V8_COREUX_PLATFORM_

#include "V8-plugin.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//-----------------------------------------------------------------------------
const auto V8ArrayBufferAllocatorInstance = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//-----------------------------------------------------------------------------


/*
* V8Platform is a small wrapper class that maintain V8 Initialization and shut down.
* It has be called only once per Application. Embedder needs to make sure to call initialize Method only one time.
* If the apps shut down and restart than it needs to be call again.
*/

class V8Platform {

public:
    static void Initialize(int thread_pool_size, bool idleTaskSupport)
    {
        if (!isInitizied) {
            // Initialize V8
            enginePlatform = new V8Platform();
            enginePlatform->v8NativePlatform = v8::platform::NewDefaultPlatform(
                                               thread_pool_size,
                                               idleTaskSupport
                                               ? v8::platform::IdleTaskSupport::kEnabled
                                               : v8::platform::IdleTaskSupport::kDisabled);

            v8::V8::InitializePlatform(enginePlatform->v8NativePlatform.get());
            isInitizied = v8::V8::Initialize();

       /**
       * On Win64, by default V8 does not emit unwinding data for jitted code,
       * which means the OS cannot walk the stack frames and the system Structured
       * Exception Handling (SEH) cannot unwind through V8-generated code:
       */
#if defined(V8_OS_WIN)
            v8::V8::SetUnhandledExceptionCallback(CatchUnhandledExceptionCallback);
#endif
        }
    }

    static void Dispose()
    {
        if (isInitizied) {
            delete enginePlatform;
            enginePlatform = nullptr;
            isInitizied = false;
        }
    }

    static double MonotonicallyIncreasingTime() {
        return isInitizied ? enginePlatform->v8NativePlatform->MonotonicallyIncreasingTime() : 0.025;
    }
#if defined(V8_OS_WIN)
    /*
    * Consume exception for V8  unwinding data for jitted code
    */
    static int CatchUnhandledExceptionCallback (_EXCEPTION_POINTERS* exception_pointers)
    {
        // 0 = ExceptionContinueExecution
        return 0;
    }
#endif

private:
    V8Platform() { isInitizied = false; }
    ~V8Platform() {
        if (isInitizied) {
            v8::V8::Dispose();
            v8::V8::ShutdownPlatform();

            v8::Platform *tempPlatform = this->v8NativePlatform.get();
            this->v8NativePlatform.release();
            delete tempPlatform;
            isInitizied = false;

            Log("ShutdownPlatform V8 \n");
        }
    }

    std::unique_ptr<v8::Platform> v8NativePlatform = nullptr;

    static bool isInitizied;
    static V8Platform *enginePlatform;
};

V8Platform* V8Platform::enginePlatform;
bool V8Platform::isInitizied;

#endif