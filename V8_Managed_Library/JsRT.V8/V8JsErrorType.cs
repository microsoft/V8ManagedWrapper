// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8JsErrorType.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;

    [Flags]
    public enum V8JsErrorType
    {
        JsNoError = 0,
        Exception = 1,
        NullArgument = 2,
        NullReference = 3,
        ScriptCompilationError = 4,
        ScriptExecutionException = 5,
        InvalidOperation = 6,
        InvalidRuntime = 7
    }
}
