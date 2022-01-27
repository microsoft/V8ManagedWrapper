// A '.tsx' file enables JSX support in the TypeScript compiler, 
// for more information see the following page on the TypeScript wiki:
// https://github.com/Microsoft/TypeScript/wiki/JSX
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
/// <reference path="React\react.d.ts" />
/// <reference path="Declarations\InstLink.d.ts" />
var BingComment = /** @class */ (function (_super) {
    __extends(BingComment, _super);
    function BingComment() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    BingComment.prototype.render = function () {
        if (this.props !== null) {
            return (React.createElement("div", { className: "comment" },
                React.createElement("h3", { className: "commentAuthor" }, this.props.Author),
                React.createElement("div", { className: "commentText" }, this.props.Message)));
        }
    };
    return BingComment;
}(React.Component));
var CommentList = /** @class */ (function (_super) {
    __extends(CommentList, _super);
    function CommentList(props) {
        var _this = _super.call(this, props) || this;
        _this.state = { comments: _this.props.comments };
        return _this;
    }
    CommentList.prototype.render = function () {
        return (React.createElement("div", null,
            React.createElement("div", { className: "commentList" }, this.state.comments.map(function (comment) { return React.createElement(BingComment, { Author: comment.Author, Message: comment.Message, key: comment.Message }); }))));
    };
    return CommentList;
}(React.Component));
var CommentForm = /** @class */ (function (_super) {
    __extends(CommentForm, _super);
    function CommentForm() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    CommentForm.prototype.handleSubmit = function (e) {
        e && e.preventDefault();
        var ref = this.refs;
        var author = ref.author.value.trim();
        var text = ref.text.value.trim();
        console.log(author);
        console.log(text);
        this.props.addComment(author, text);
    };
    CommentForm.prototype.render = function () {
        var link = { UrlWithFormCode: null, UniqueId: { Namespace: 'SERP', KValue: 200, k: 200 }, CurrentUseCount: 0 };
        return (React.createElement(InstForm, { className: "commentForm", onSubmit: this.handleSubmit.bind(this), Link: link, InstParentNodeName: "region_root" },
            React.createElement("input", { type: "text", placeholder: "Your name", ref: "author" }),
            React.createElement("input", { type: "text", placeholder: "Say something...", ref: "text" }),
            React.createElement("input", { type: "submit", value: "Post" })));
    };
    return CommentForm;
}(React.Component));
var Footer = /** @class */ (function (_super) {
    __extends(Footer, _super);
    function Footer() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    Footer.prototype.render = function () {
        return (React.createElement("div", null));
    };
    return Footer;
}(React.Component));
var CommentBox = /** @class */ (function (_super) {
    __extends(CommentBox, _super);
    function CommentBox(props) {
        var _this = _super.call(this, props) || this;
        _this.state = { title: _this.props.Title, comments: _this.props.Comments, link: _this.props.Link };
        return _this;
    }
    CommentBox.prototype.updateCommentList = function (author, message) {
        this.setState({
            comments: this.state.comments.push({ Author: author, Message: message })
        });
    };
    CommentBox.prototype.render = function () {
        return (React.createElement(Region, { Type: "L.Box", DisplayRegion: "ServerSideRenderingReact", InstNodeName: "region_root" },
            React.createElement("div", { className: "commentBox" },
                React.createElement("h1", null, this.props.Title),
                React.createElement("h2", null, "Comments"),
                React.createElement(CommentList, { comments: this.state.comments }),
                React.createElement(CommentForm, { addComment: this.updateCommentList.bind(this) }),
                React.createElement(InstLink, { InstItem: this.state.link, "data-partnertag": "serversiderendering", Content: "Bing @", InstParentNodeName: "region_root" }),
                React.createElement(InstLink, { InstItem: this.state.link, "data-partnertag": "serversiderendering", Content: " Microsoft", InstParentNodeName: "region_root" }),
                React.createElement(Region, { Type: "L.Empty", DisplayRegion: "EmptyRegion", IsEmptyChild: true, InstParentNodeName: "region_root" }))));
    };
    return CommentBox;
}(React.Component));
var MySaveBox = /** @class */ (function (_super) {
    __extends(MySaveBox, _super);
    function MySaveBox(props) {
        return _super.call(this, props) || this;
    }
    MySaveBox.prototype.render = function () {
        var title = this.props.Title.indexOf("siam") <= 0 ? this.props.Title : this.props.Title + " _bing";
        var body = this.props.Body.length > 0 ? this.props.Body : " default body";
        return (React.createElement("div", { "class": "mysaveBox" },
            React.createElement("h5", null, title),
            React.createElement("p", null, body),
            React.createElement("img", { src: this.props.Url })));
    };
    return MySaveBox;
}(React.Component));
var unique = 0;
var MySave = /** @class */ (function (_super) {
    __extends(MySave, _super);
    function MySave(props) {
        return _super.call(this, props) || this;
    }
    MySave.prototype.render = function () {
        return (React.createElement(Region, { Type: "L.Box", DisplayRegion: "mysave", InstNodeName: "mysave" }, this.props.Data.map(function (mysaveData) { return React.createElement(MySaveBox, { Title: mysaveData.Title, Body: mysaveData.Body, Url: mysaveData.Url, key: unique++ }); })));
    };
    return MySave;
}(React.Component));
