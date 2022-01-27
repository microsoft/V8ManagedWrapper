namespace Microsoft.Search.Frontend.V8
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;

    public static class Helper
    {
        public static readonly string[] Library = new string[]
        {
            "ServerSideRenderingDataManager.js",
            "ServerSideRendering.LayoutWriter.js",
            "ServerSideRendering.Instrumentation.js",
            "react.production16.1.1.min.js",
            "react-dom-server.browser.production16.1.1.min.js",
            "redux.min.js",
            "Region.js",
            "InstLink.js",
            "InstForm.js",
            "CommentBox.js",
            "CommentBoxServerRendering.js"
        };

        public static readonly string DataScript;

        public static readonly string RenderScript = "ServerSideRenderingCommentBoxEample.render();";
        public static readonly string RenderSaveBox = "ServerSideRenderingCommentBoxEample.renderMySave();";
        public static readonly string InstrumentationScript = "HostInstrumentation.getInstrumentationObject();ServerSideRenderingDataManager.clear();LayoutWriter.clear();";

        public static readonly Dictionary<string, string> JavascriptLibary = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

        static Helper()
        {
            foreach (var fileName in Library)
            {
                JavascriptLibary[fileName] = LoadFile(fileName);
            }

            DataScript = LoadFile("Data.txt");
        }

        public static string LoadFile(string filename)
        {
            var path = Path.Combine("Scripts", filename);
            if (File.Exists(path))
            {
                return File.ReadAllText(path);
            }

            throw new Exception("File doesn't exit " + path);
        }

        public static long ExecuteAction(Stopwatch stopwatch, Action action, string eventName = "")
        {
            stopwatch.Restart();
            action.Invoke();
            stopwatch.Stop();
            var time = stopwatch.ElapsedMilliseconds;
            // Console.WriteLine(eventName + " Execution time : " + time);
            return time;
        }
    }
}
