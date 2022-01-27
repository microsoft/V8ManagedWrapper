

namespace Microsoft.Search.Frontend.V8
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Text;
    using JsRT.V8;

    public class Tests
    {
        public static void StringReturnAfterJavaScriptExecution()
        {
            using (var runtime = new V8Engine())
            {
                runtime.Execute("function A() {return 'Hello Bing'};");
                var output = runtime.Evaluate("A();");

                if (!String.Equals(output, "Hello Bing"))
                {
                    Console.WriteLine("[Failing] StringReturnAfterJavaScriptExecution. Output String : " + output);
                }
                else
                {
                    Console.WriteLine("[Passed] StringReturnAfterJavaScriptExecution");
                }
            }
        }

        public static void NumberReturnAfterJavaScriptExecution()
        {
            using (var runtime = new V8Engine())
            {
                var output = runtime.EvaluateAndGetNativeObject("function A() {return 3.5 + 10;}; A();");
                if (output.IsValid)
                {
                    if (output.ValueType() == V8ValueType.Double)
                    {
                        var value = output.ToDouble();
                        Console.WriteLine("[Passed] NumberReturnAfterJavaScriptExecution. Output {0}, Expected {1}", value, 13.5);
                    }
                }
                else
                {
                    Console.WriteLine("[Failing] NumberReturnAfterJavaScriptExecution. Output is not valid");
                }
            }
        }

        public static void TestsWebAppPageWithReact()
        {
            using (var runtime = new V8Engine())
            {
                foreach (var scriptName in Helper.Library)
                {
                    runtime.Execute(Helper.JavascriptLibary[scriptName]);
                }

                runtime.Execute(Helper.DataScript);
                var res = runtime.Evaluate(Helper.RenderScript);

                if (!string.IsNullOrEmpty(res))
                {
                    Console.WriteLine("[Passed] TestsWebAppPageWithReact.");
                }
                else
                {
                    Console.WriteLine("[Failed] TestsWebAppPageWithReact.");
                }
            }
        }

        public static void RecordPerfResults(int maxIterationLimit = 1)
        {

            var stopwatch = new Stopwatch();
            var logMessage = new StringBuilder(5058);
            logMessage.Append("Runtime Creation,").Append(string.Join(",", Helper.Library))
                .AppendLine(",DataScript,RenderScript, conversion");


            // each iteration simulate a new request
            for (var j = 0; j < maxIterationLimit; j++)
            {
                string result;
                stopwatch.Restart();
                using (var runtime = new V8Engine())
                {
                    stopwatch.Stop();
                    logMessage.Append(stopwatch.ElapsedMilliseconds).Append(",");

                    long time = 0;
                    foreach (var scriptName in Helper.Library)
                    {
                        time = Helper.ExecuteAction(
                            stopwatch,
                            () => { runtime.Execute(Helper.JavascriptLibary[scriptName]); },
                            scriptName);

                        logMessage.Append(time).Append(",");
                    }

                    time = Helper.ExecuteAction(stopwatch, () => { runtime.Execute(Helper.DataScript); }, "DataScript");
                    logMessage.Append(time).Append(",");

                    stopwatch.Restart();
                    var res = runtime.Evaluate(Helper.RenderScript);

                    var res1 = runtime.Evaluate(Helper.RenderSaveBox);
                    runtime.Execute("HostInstrumentation.getInstrumentationObject();");
                    runtime.Execute("ServerSideRenderingDataManager.clear();ServerSideRenderingDataManager.clear();");
                    result = res + res1;
                    stopwatch.Stop();
                    logMessage.AppendLine(stopwatch.ElapsedMilliseconds + "");
                }

                File.WriteAllText("v8_plainscript.html", result);
            }

            File.WriteAllText("v8_plainscript.csv", logMessage.ToString());
            Console.WriteLine("[Succeed] Generated Perf result. check v8_plainscript.csv file.");
        }

    }
}
