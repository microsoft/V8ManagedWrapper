// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8Engine.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;
    using System.Globalization;

    public class V8Engine :IDisposable
    {
        private bool disposed;
        private readonly V8Runtime runtime;
        private object executionContext;
        internal bool hasException;

        public V8Engine()
        {
            var v8MaxOldSpaceInMB = 0;
            var v8MaxSemiSpaceInMB = 0;

            this.runtime = V8Runtime.CreateRuntime(v8MaxOldSpaceInMB, v8MaxSemiSpaceInMB);

            this.disposed = false;
            this.executionContext = new object();
            this.hasException = false;
        }

        ~V8Engine()
        {
            this.Dispose(false);
        }


        public void Execute(string script)
        {
            if (string.IsNullOrEmpty(script))
            {
                throw new ArgumentNullException(nameof(script));
            }

            lock (this.executionContext)
            {
                try
                {
                    this.runtime.RunScript(script, false);
                }
                catch (Exception ex)
                {
                    hasException = true;
                    throw ex;
                }
            }
        }

        public string Evaluate(string script)
        {
            if (string.IsNullOrEmpty(script))
            {
                throw new ArgumentNullException(nameof(script));
            }

            var output = string.Empty;

            lock (this.executionContext)
            {
                try
                {
                    var result = this.runtime.RunScript(script, true);
                    if (result.ValueType() == V8ValueType.String)
                    {
                        output = result.ToString();
                    }

                    result.Dispose();
                }
                catch (Exception ex)
                {
                    hasException = true;
                    throw ex;
                }
            }

            return output;
        }

        public V8JsValue EvaluateAndGetNativeObject(string script)
        {
            if (string.IsNullOrEmpty(script))
            {
                throw new ArgumentNullException(nameof(script));
            }

            lock (this.executionContext)
            {
                try
                {
                    var result = this.runtime.RunScript(script, true);
                    return result;
                }
                catch (Exception ex)
                {
                    hasException = true;
                    throw ex;
                }
            }
        }


        public virtual void Dispose()
        {
            this.Dispose(true);

            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!disposed && disposing)
            {
                this.runtime.Dispose();
                this.disposed = true;
            }
        }
    }
}
