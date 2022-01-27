/// <reference path="..\React\react.d.ts" />
/// <reference path="..\Declarations\InstLink.d.ts" />
///<reference path="..\..\..\..\..\..\..\Serp\AppResources\Scripts\ServerSideRendering\ServerSideRendering.Instrumentation.d.ts" />
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
var __assign = (this && this.__assign) || Object.assign || function(t) {
    for (var s, i = 1, n = arguments.length; i < n; i++) {
        s = arguments[i];
        for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
            t[p] = s[p];
    }
    return t;
};
var InstForm = /** @class */ (function (_super) {
    __extends(InstForm, _super);
    function InstForm(props) {
        var _this = _super.call(this, props) || this;
        _this.state = _this.getState(_this.props);
        return _this;
    }
    InstForm.prototype.componentWillMount = function () {
        if (!this.state.isStartRegionLogging && !this.state.isCloseRegionLogging) {
            this.state.isStartRegionLogging = true;
            ServerSideLayoutInstrumentation.startForm(this.state.appNS, this.state.k, this.state.nodeName, this.state.parentNodeName);
            // If parent node is provide then close the region.
            // Child component under this region has to use ParentNodeName to be instrumented under this region.
            if (this.state.parentNodeName) {
                this.closeInstrumentationNode(this.state);
            }
        }
    };
    InstForm.prototype.closeInstrumentationNode = function (state) {
        if (state.isStartRegionLogging && !state.isCloseRegionLogging) {
            state.isCloseRegionLogging = true;
            ServerSideLayoutInstrumentation.closeForm();
        }
    };
    InstForm.prototype.componentDidMount = function () {
        this.closeInstrumentationNode(this.state);
    };
    InstForm.prototype.getItemFromProperty = function (props, propertyName, defaultValue) {
        if (props.hasOwnProperty(propertyName) && props[propertyName]) {
            return props[propertyName];
        }
        else
            return defaultValue;
    };
    InstForm.prototype.handleSubmit = function (event) {
        if (this.state.onSubmit) {
            this.state.onSubmit(event);
        }
    };
    InstForm.prototype.callGPing = function (event) {
        return si_ct && si_ct(this, 1);
    };
    InstForm.prototype.getState = function (props) {
        var instNodeName = "InstNodeName", nodeName = props.hasOwnProperty(instNodeName) && props[instNodeName] ? props[instNodeName] : null;
        var instParentNodeName = "InstParentNodeName", parentNodeName = props.hasOwnProperty(instParentNodeName) && props[instParentNodeName] ? props[instParentNodeName] : null;
        var onsubmitPropertyName = "onSubmit";
        var actionPropertyName = "action";
        var linkPropertyName = "Link";
        var suppressGPingPropertyName = "SuppressGPing";
        var renderableProperties = {};
        var appNS = "SERP";
        var k = "";
        var link = this.getItemFromProperty(props, linkPropertyName, null);
        if (link == null) {
            throw "Link parameter is required for InstForm";
        }
        var suppressGPing = this.getItemFromProperty(props, suppressGPingPropertyName, false);
        var url = link != null ? link.UrlWithFormCode : null;
        if (url != null) {
            renderableProperties["action"] = url;
        }
        var instrumentedItem = link;
        if (instrumentedItem != null) {
            instrumentedItem.CurrentUseCount++;
            appNS = instrumentedItem.UniqueId.Namespace;
            k = instrumentedItem.UniqueId.KValue.toString();
            if (instrumentedItem.CurrentUseCount > 0) {
                k += "." + instrumentedItem.CurrentUseCount.toString();
            }
        }
        var onSubmit = props.hasOwnProperty(onsubmitPropertyName) && props[onsubmitPropertyName] ? props[onsubmitPropertyName] : null;
        var hasGPing = !suppressGPing;
        if (hasGPing) {
            renderableProperties["data-h"] = "ID=" + appNS + "," + k;
        }
        for (var property in props) {
            if (props.hasOwnProperty(property)) {
                // Skip "action", "onsubmit", "Link" and "SuppressGPing" properties
                if (property === onsubmitPropertyName ||
                    property === actionPropertyName ||
                    property === linkPropertyName ||
                    property === instNodeName ||
                    property === instParentNodeName ||
                    property === suppressGPingPropertyName)
                    continue;
                // copy all other attributes as is
                renderableProperties[property] = props[property];
            }
        }
        return {
            renderableProperties: renderableProperties,
            isStartRegionLogging: false,
            isCloseRegionLogging: false,
            appNS: appNS,
            k: k,
            hasGPing: hasGPing,
            onSubmit: onSubmit,
            nodeName: nodeName,
            parentNodeName: parentNodeName
        };
    };
    InstForm.prototype.render = function () {
        // Add "onsubmit" attribute, which is rendered differently based on the following cases:
        // 1. GPing only
        // 2. Caller-provided handler only
        // 3. Both GPing and caller-provided handler
        var hasOnSubmit = this.state.hasGPing || this.state.onSubmit !== null;
        if (!hasOnSubmit) {
            return (React.createElement("form", __assign({}, this.state.renderableProperties), this.props.children));
        }
        else if (this.state.hasGPing && !this.state.onSubmit) {
            return (React.createElement("form", __assign({}, this.state.renderableProperties, { onSubmit: this.callGPing.bind(this) }), this.props.children));
        }
        else if (!this.state.hasGPing && this.state.onSubmit) {
            return (React.createElement("form", __assign({}, this.state.renderableProperties, { onSubmit: this.handleSubmit.bind(this) }), this.props.children));
        }
        else {
            return (React.createElement("form", __assign({}, this.state.renderableProperties, { onSubmit: this.callGPing.bind(this) && this.handleSubmit.bind(this) }), this.props.children));
        }
    };
    return InstForm;
}(React.Component));
