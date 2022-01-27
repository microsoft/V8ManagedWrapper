// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8JsException.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;
    using System.Runtime.InteropServices;

    public struct V8JsException
    {
        /* this will hold the native V8Exception object */
        private IntPtr handle;

        public string GetExceptionMessage()
        {
            IntPtr buffer;
            int length = 0;
            if (V8Native.GetExceptionMessage(this, out buffer, out length) == V8JsErrorType.JsNoError)
            {
                return Marshal.PtrToStringUni(buffer, length);
            }

            return string.Empty;
        }

        public string GetStackTrace()
        {
            IntPtr buffer;
            int length = 0;
            if (V8Native.GetStackTrace(this, out buffer, out length) == V8JsErrorType.JsNoError)
            {
                return Marshal.PtrToStringUni(buffer, length);
            }

            return string.Empty;
        }

        public void Dispose()
        {
            /* Don't need to dispose native object explicitly.
             Native exception object will be free when current runtime will be disposed. */
        }

        public JavaScriptUsageException GetException(V8JsErrorType errorType)
        {
            var message = this.GetExceptionMessage();
            var stacktrace = this.GetStackTrace();

            return new JavaScriptUsageException(message, stacktrace, errorType);
        }
    }
}
