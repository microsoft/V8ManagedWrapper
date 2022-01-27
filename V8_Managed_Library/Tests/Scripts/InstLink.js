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
var InstLink = /** @class */ (function (_super) {
    __extends(InstLink, _super);
    function InstLink(props) {
        var _this = _super.call(this, props) || this;
        _this.state = _this.getNewState(_this.props);
        return _this;
    }
    InstLink.prototype.componentWillMount = function () {
        var instItem = this.state.instItem;
        if (instItem && !this.state.isInstrumented) {
            this.state.isInstrumented.isStartLink = true;
            instItem.CurrentUseCount++;
            var k = instItem.UniqueId.K + "";
            if (instItem.CurrentUseCount && instItem.CurrentUseCount > 0) {
                k = k + "." + instItem.CurrentUseCount;
            }
            ServerSideLayoutInstrumentation.startLink(instItem.UniqueId.Namespace, k, this.state.nodeName, this.state.parentNodeName);
            //InstLink can't have children so close the instrumentation node.
            ServerSideLayoutInstrumentation.closeLink();
        }
    };
    InstLink.prototype.componentWillReceiveProps = function (nextProps) {
        this.setState(this.getNewState(nextProps));
    };
    InstLink.prototype.getNewState = function (props) {
        var instNodeName = "InstNodeName", nodeName = props.hasOwnProperty(instNodeName) && props[instNodeName] ? props[instNodeName] : null;
        var instParentNodeName = "InstParentNodeName", parentNodeName = props.hasOwnProperty(instParentNodeName) && props[instParentNodeName] ? props[instParentNodeName] : null;
        // InstLink property
        var instItemPropertyName = "InstItem", instItem = props[instItemPropertyName];
        // TestHooks
        var testHooksEnablePropName = "TestHooksEnabled", testHooksEnabled = props[testHooksEnablePropName] === true, partnerTestHookPropertyName = "PartnerTestHook", dataPartnerTagAttributeName = "data-partnertag";
        var contentPropertyName = "Content";
        // Get all properties passed as attributes.
        // Then copy all of them except serialized objects as Link:ILink and Content:ReactComponent
        var allProperties = {};
        for (var property in props) {
            if (props.hasOwnProperty(property)) {
                // Skip "Link" and "Content" properties
                if (property === instItemPropertyName ||
                    property === contentPropertyName ||
                    property === instParentNodeName ||
                    property === instNodeName ||
                    property === testHooksEnablePropName)
                    continue;
                // Translate PartnerTestHooks to data-partnertag if testhooks are enabled
                if (property === partnerTestHookPropertyName) {
                    if (testHooksEnabled) {
                        allProperties[dataPartnerTagAttributeName] = props[property];
                    }
                    // If TestHooksEnabled is not set to true explicitly - just skip this attribute
                    continue;
                }
                // copy all other attributes as is
                allProperties[property] = props[property];
            }
        }
        // Prevent links from being draggable.
        allProperties["draggable"] = false;
        if (instItem) {
            // If href attribute is passed as a property - replace the one defined in ILink
            var hrefAttributeName = "href";
            if (!allProperties[hrefAttributeName]) {
                allProperties[hrefAttributeName] = instItem.UrlWithFormCode;
            }
            // Generate KValue + namespace to enable standard GPingClick instrumentation on the element. Add 1 to current use count
            // because it should be 1-based
            allProperties["data-h"] = "ID=" + instItem.UniqueId.Namespace + "," + instItem.UniqueId.K + "." + (instItem.CurrentUseCount + 1);
        }
        return {
            content: this.props[contentPropertyName] ? this.props[contentPropertyName] : this.props.children,
            properties: allProperties,
            isInstrumented: false,
            instItem: instItem,
            nodeName: nodeName,
            parentNodeName: parentNodeName
        };
    };
    InstLink.prototype.render = function () {
        return (React.createElement("a", __assign({}, this.state.properties),
            " ",
            this.state.content,
            " "));
    };
    return InstLink;
}(React.Component));
