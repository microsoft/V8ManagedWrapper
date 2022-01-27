// --------------------------------------------------------------------------------------------------------------------
// <copyright file="JavaScriptUsageException.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;

    public class JavaScriptUsageException : Exception
    {
        private const string DefaultErrorMessage = "V8 Runtime is throwing exception. Please check stack trace.";

        public JavaScriptUsageException(string message, string stackTrace, V8JsErrorType errorType)
        {
            this.Message = string.IsNullOrEmpty(message) ? DefaultErrorMessage : message;
            this.StackTrace = string.IsNullOrEmpty(stackTrace) ? string.Empty : stackTrace;
            this.ErrorType = errorType;
        }

        public override string StackTrace { get; }

        public override string Message { get; }

        public V8JsErrorType ErrorType { get; private set; }
    }
}
