// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8JsValue.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;
    using System.Runtime.InteropServices;

    public struct V8JsValue
    {
        private IntPtr reference;

        public V8JsValue(IntPtr reference)
        {
            this.reference = reference;
        }

        /// <summary>
        ///     Gets an invalid value.
        /// </summary>
        public static V8JsValue Invalid
        {
            get { return new V8JsValue(IntPtr.Zero); }
        }

        /// <summary>
        ///     Gets a value indicating whether the value is valid.
        /// </summary>
        public bool IsValid
        {
            get { return reference != IntPtr.Zero; }
        }

        /// <summary>
        /// return the current V8JsValue object's Type (e.g: int, double, string etc)
        /// </summary>
        /// <returns></returns>
        public V8ValueType ValueType()
        {
            if (!this.IsValid)
            {
                return V8ValueType.UnknownType;
            }

            V8ValueType type = V8Native.GetV8ValueType(this);
            return type;
        }

        /// <summary>
        /// convert this V8JsValue object to bool if the type is bool
        /// </summary>
        public bool ToBool()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [ToBool]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            bool result;
            V8Native.ThrowIfError(V8Native.GetV8ValueAsBool(this, out result));
            return result;
        }

        /// <summary>
        /// convert this V8JsValue object to long if the type is a number (int or uint)
        /// </summary>
        public long ToInt()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [ToInt]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            long result;
            V8Native.ThrowIfError(V8Native.GetV8ValueAsInt(this, out result));
            return result;
        }

        /// <summary>
        /// convert this V8JsValue object to double if the type is double
        /// </summary>
        public double ToDouble()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [ToDouble]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            double result;
            V8Native.ThrowIfError(V8Native.GetV8ValueAsDouble(this, out result));
            return result;
        }

        /// <summary>
        /// convert this V8JsValue object to string if the type is string
        /// </summary>
        public new string ToString()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [ToString]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            IntPtr buffer;
            int length;
            V8Native.ThrowIfError(V8Native.GetV8ValueAsString(this, out buffer, out length));
            return Marshal.PtrToStringUni(buffer);
        }

        /// <summary>
        /// Get the length of current Javascript(V8JsValue) object
        /// </summary>
        public int GetPropertyLength()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [GetPropertyLength]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            int length;
            V8Native.ThrowIfError(V8Native.GetLength(this, out length));
            return length;
        }

        /// <summary>
        /// Get the length of current Javascript(V8JsValue) Array
        /// </summary>
        public int GetArrayLength()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [GetArrayLength]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            int length;
            V8Native.ThrowIfError(V8Native.GetLength(this, out length));
            return length;
        }

        /// <summary>
        /// if current object has any property name then it will return the name.
        /// e.g {'name' : value}. this method will return 'name'.
        /// </summary>
        public string GetPropertyName()
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [GetPropertyName]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            IntPtr buffer;
            int length;
            V8Native.ThrowIfError(V8Native.GetOwnPropertyName(this, out buffer, out length));
            return Marshal.PtrToStringUni(buffer, length);
        }

        /// <summary>
        /// if current object has any property name then it will return the name.
        /// e.g {'name' : value}. this method will return 'name'.
        /// </summary>
        public V8JsValue GetProperty(int index)
        {
            if (!this.IsValid)
            {
                throw new JavaScriptUsageException("Current V8JsValue doesn't have valid value to do this Operation [GetProperty]", string.Empty, V8JsErrorType.InvalidOperation);
            }

            V8JsValue prop;
            V8Native.ThrowIfError(V8Native.GetObjectByIndex(this, index, out prop));
            return prop;
        }

        public void Dispose()
        {
            if (this.IsValid)
            {
                V8Native.ThrowIfError(V8Native.DisposeV8Value(this));
                this.reference = IntPtr.Zero;
            }
        }
    }
}
