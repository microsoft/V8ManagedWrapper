// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Program.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Microsoft.Search.Frontend.V8
{
    using System;
    using System.IO;
    using System.Reflection;
    using JsRT.V8;


    public class Program
    {
        static int Main(string[] args)
        {
            Directory.SetCurrentDirectory(Path.GetDirectoryName(Assembly.GetAssembly(typeof(Program)).Location));

            try
            {
                var errorType = V8Native.InitializeV8(Environment.ProcessorCount, false);
                if (errorType == V8JsErrorType.JsNoError)
                {
                    Console.WriteLine("Loaded V8");

                    Tests.StringReturnAfterJavaScriptExecution();
                    Tests.NumberReturnAfterJavaScriptExecution();
                    Tests.TestsWebAppPageWithReact();

                    // generated perf result
                    Tests.RecordPerfResults(3);
                    Tests.RecordPerfResults(100);

                    errorType = V8Native.DisposeV8Platform();
                    if (errorType == V8JsErrorType.JsNoError)
                    {
                        Console.WriteLine("Disposed V8");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception. {ex.Message}. stacktrace {ex.StackTrace}");
                return -1;
            }

            Console.WriteLine("Ending Console App");
            return 0;
        }
    }
}
