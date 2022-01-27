///<reference path="..\..\..\..\..\..\Serp\AppResources\Scripts\ServerSideRendering\ServerSideRenderingDataManager.d.ts" />
///<reference path="..\..\..\..\..\..\Serp\AppResources\Scripts\ServerSideRendering\ServerSideRendering.Instrumentation.d.ts" />
///<reference path="React\React-server-dom16.d.ts" />
// Gettign the data that we register using ServerSideRendering:Data view
var ServerSideRenderingCommentBoxEample;
(function (ServerSideRenderingCommentBoxEample) {
    function render() {
        var commentBoxData = ServerSideRenderingDataManager.get("CommentBoxData");
        var link = ServerSideRenderingDataManager.get("link");
        ServerSideLayoutInstrumentation.setPageInfo("ServerSideReact", 101);
        // ReactDOMServer.renderToString executes on serverside and return the html output as string.
        return ReactDOMServer.renderToString(React.createElement(CommentBox, { Title: commentBoxData.Title, Comments: commentBoxData.Comments, Link: link }));
    }
    ServerSideRenderingCommentBoxEample.render = render;
    function renderMySave() {
        var data = ServerSideRenderingDataManager.get("mysaveData");
        return ReactDOMServer.renderToString(React.createElement(MySave, { Data: data.Data }));
    }
    ServerSideRenderingCommentBoxEample.renderMySave = renderMySave;
    function renderFibonacci() {
        getFibonnaci(1000);
        return "Fibonnaci Done";
    }
    ServerSideRenderingCommentBoxEample.renderFibonacci = renderFibonacci;
    function getFibonnaci(num) {
        if (num <= 1)
            return 1;
        return getFibonnaci(num - 1) + getFibonnaci(num - 2);
    }
})(ServerSideRenderingCommentBoxEample || (ServerSideRenderingCommentBoxEample = {}));
//# sourceMappingURL=CommentBoxServerRendering.js.map