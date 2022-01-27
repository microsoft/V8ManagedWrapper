// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8Native.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;
    using System.Runtime.InteropServices;

    public class V8Native
    {
        private const string DllName = "v8.dll";

        public static void ThrowIfError(V8JsErrorType type)
        {
            switch (type)
            {
                case V8JsErrorType.JsNoError:
                    return;
                case V8JsErrorType.InvalidRuntime:
                    throw new JavaScriptUsageException("Invalid V8 Runtime", string.Empty, type);
                case V8JsErrorType.InvalidOperation:
                    throw new JavaScriptUsageException("Invalid operation for current object", string.Empty, type);
                case V8JsErrorType.NullArgument:
                    throw new JavaScriptUsageException("Argument is Null", string.Empty, type);
                case V8JsErrorType.NullReference:
                    throw new JavaScriptUsageException("Reference is Null", string.Empty, type);
            }
        }

        [DllImport(DllName)]
        public static extern V8JsErrorType InitializeV8(int threadpoolSize, bool idleTaskSupport);

        [DllImport(DllName)]
        public static extern V8JsErrorType DisposeV8Platform();

        [DllImport(DllName)]
        public static extern V8JsErrorType CreateV8RunTime(int maxOldSpaceInMB, int maxSemiSpaceInMB, out V8Runtime runtime);

        [DllImport(DllName)]
        public static extern V8JsErrorType DisposeV8Runtime(V8Runtime runtime);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        public static extern V8JsErrorType RunScript(V8Runtime runtime, string script, bool evalute, out V8JsValue result);

        [DllImport(DllName)]
        public static extern V8JsErrorType Idle(V8Runtime handle);

        [DllImport(DllName)]
        public static extern V8JsErrorType CollectGarbage(V8Runtime handle);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetException(V8Runtime handle, out V8JsException v8JsException);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        public static extern V8JsErrorType GetExceptionMessage(V8JsException v8JsException, out IntPtr message, out int length);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        public static extern V8JsErrorType GetStackTrace(V8JsException v8JsException, out IntPtr message, out int length);

        [DllImport(DllName)]
        public static extern V8ValueType GetV8ValueType(V8JsValue value);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetV8ValueAsBool(V8JsValue value, out bool result);

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        public static extern V8JsErrorType GetV8ValueAsString(V8JsValue value, out IntPtr result, out int length);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetV8ValueAsInt(V8JsValue value, out long result);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetV8ValueAsDouble(V8JsValue value, out double result);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetLength(V8JsValue value, out int length);

        // need to change method name

        [DllImport(DllName, CharSet = CharSet.Unicode)]
        public static extern V8JsErrorType GetOwnPropertyName(V8JsValue value, out IntPtr name, out int length);

        [DllImport(DllName)]
        public static extern V8JsErrorType GetObjectByIndex(V8JsValue value, int index, out V8JsValue result);

        [DllImport(DllName)]
        public static extern V8JsErrorType DisposeV8Value(V8JsValue value);
    }
}
