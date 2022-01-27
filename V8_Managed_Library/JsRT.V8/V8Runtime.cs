// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8Runtime.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;

    public struct V8Runtime
    {
        /// <summary>
        /// This will hold the Native runtime Object
        /// </summary>
        private IntPtr handle;

        public static V8Runtime CreateRuntime(int maxOldSpaceInMB, int maxSemiSpaceInMB)
        {
            V8Runtime runtime;
            V8Native.ThrowIfError(V8Native.CreateV8RunTime(maxOldSpaceInMB, maxSemiSpaceInMB, out runtime));
            return runtime;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="script">javascript as a plain string</param>
        /// <param name="evalute">if true then convert javascript output to V8Native object</param>
        /// <returns></returns>
        public V8JsValue RunScript(string script, bool evalute)
        {
            V8JsValue result;
            var errorCode = V8Native.RunScript(this, script, evalute, out result);
            if (errorCode == V8JsErrorType.JsNoError)
            {
                return result;
            }
            else if (errorCode == V8JsErrorType.ScriptCompilationError || errorCode == V8JsErrorType.ScriptExecutionException)
            {
                V8JsException exception;
                V8Native.ThrowIfError(V8Native.GetException(this, out exception));
                throw exception.GetException(errorCode);
            }
            else
            {
                V8Native.ThrowIfError(errorCode);
            }

            return V8JsValue.Invalid;
        }

        /// <summary>
        /// This will tell V8Runtime to go Idle. 
        /// </summary>
        public void Idle()
        {
            V8Native.ThrowIfError(V8Native.Idle(this));
        }

        /// <summary>
        /// Performs a full garbage collection.
        /// </summary>
        public void CollectGarbage()
        {
            V8Native.ThrowIfError(V8Native.CollectGarbage(this));
        }

        /// <summary>
        /// Dispose native V8RuntimeHandler object.
        /// During freeing V8RuntimeHandler, it will also free current context and exception object
        /// </summary>
        public void Dispose()
        {
            V8Native.ThrowIfError(V8Native.DisposeV8Runtime(this));
        }
    }
}
