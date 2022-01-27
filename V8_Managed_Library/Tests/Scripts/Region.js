/// <reference path="..\React\react.d.ts" />
///<reference path="..\..\..\..\..\..\..\Serp\AppResources\Scripts\ServerSideRendering\ServerSideRendering.Instrumentation.d.ts" />
///<reference path="..\..\..\..\..\..\..\Serp\AppResources\Scripts\ServerSideRendering\ServerSideRendering.RegionInstrumentation.d.ts" />
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
var Region = /** @class */ (function (_super) {
    __extends(Region, _super);
    function Region(props) {
        var _this = _super.call(this, props) || this;
        _this.state = _this.getState(_this.props);
        return _this;
    }
    Region.prototype.componentWillMount = function () {
        if (!this.state.isStartRegionLogging && !this.state.isCloseRegionLogging) {
            this.state.isStartRegionLogging = true;
            ServerSideLayoutInstrumentation.startRegion(this.state.regionInstrumentation, this.state.nodeName, this.state.parentNodeName);
            // If node name or parent node name is provided then close the region.
            // Child component under this region has to use ParentNodeName to be instrumented under this region.
            if (this.state.nodeName || this.state.parentNodeName) {
                this.closeInstrumentationNode(this.state);
            }
        }
    };
    Region.prototype.closeInstrumentationNode = function (state) {
        if (state.isStartRegionLogging && !state.isCloseRegionLogging) {
            state.isCloseRegionLogging = true;
            ServerSideLayoutInstrumentation.closeRegion();
        }
    };
    Region.prototype.componentDidMount = function () {
        this.closeInstrumentationNode(this.state);
    };
    Region.prototype.addPropertyIfExist = function (props, property, instrumentation, instProperty) {
        if (props.hasOwnProperty(property) && props[property]) {
            instrumentation[instProperty] = props[property];
        }
    };
    Region.prototype.getState = function (props) {
        var instNodeName = "InstNodeName", nodeName = props.hasOwnProperty(instNodeName) && props[instNodeName] ? props[instNodeName] : null;
        var instParentNodeName = "InstParentNodeName", parentNodeName = props.hasOwnProperty(instParentNodeName) && props[instParentNodeName] ? props[instParentNodeName] : null;
        var regionInstrumentation = { type: "L.BOX", appNS: "SERP" };
        var instrumentedItem = this.props.hasOwnProperty("InstrumentedItem") && this.props["InstrumentedItem"] ? this.props["InstrumentedItem"] : null;
        var isLogged = this.props.hasOwnProperty("IsLogged") && this.props["IsLogged"] ? this.props["IsLogged"] : true;
        var isEmptyChild = this.props.hasOwnProperty("IsEmptyChild") && this.props["IsEmptyChild"] ? this.props["IsEmptyChild"] : false;
        this.addPropertyIfExist(props, "Type", regionInstrumentation, "type");
        this.addPropertyIfExist(props, "NS", regionInstrumentation, "appNS");
        this.addPropertyIfExist(props, "K", regionInstrumentation, "k");
        this.addPropertyIfExist(props, "DisplayRegion", regionInstrumentation, "displayRegion");
        this.addPropertyIfExist(props, "DynamicContentIndex", regionInstrumentation, "dynamicContentIndex");
        this.addPropertyIfExist(props, "InsertRef", regionInstrumentation, "insertRef");
        this.addPropertyIfExist(props, "InsertId", regionInstrumentation, "insertId");
        this.addPropertyIfExist(props, "N", regionInstrumentation, "n");
        this.addPropertyIfExist(props, "ST", regionInstrumentation, "stringTruncated");
        this.addPropertyIfExist(props, "Position", regionInstrumentation, "position");
        this.addPropertyIfExist(props, "Size", regionInstrumentation, "size");
        this.addPropertyIfExist(props, "EntityCat", regionInstrumentation, "entityCat");
        this.addPropertyIfExist(props, "EntityId", regionInstrumentation, "entityId");
        this.addPropertyIfExist(props, "EntityId", regionInstrumentation, "orientation");
        this.addPropertyIfExist(props, "ForcedTo", regionInstrumentation, "forcedTo");
        this.addPropertyIfExist(props, "Height", regionInstrumentation, "height");
        this.addPropertyIfExist(props, "TitleWidth", regionInstrumentation, "titleWidth");
        this.addPropertyIfExist(props, "HeightInRows", regionInstrumentation, "heightInRows");
        this.addPropertyIfExist(props, "SnapHeight", regionInstrumentation, "snapHeight");
        this.addPropertyIfExist(props, "SnapHeightInRows", regionInstrumentation, "snapHeightInRows");
        this.addPropertyIfExist(props, "SnippetWidthAfterTruncation", regionInstrumentation, "snippetWidthAfterTruncation");
        this.addPropertyIfExist(props, "SnippetWidth", regionInstrumentation, "snippetWidth");
        this.addPropertyIfExist(props, "TitleWidthAfterTruncation", regionInstrumentation, "titleWidthAfterTruncation");
        this.addPropertyIfExist(props, "IsTitleTruncated", regionInstrumentation, "isTitleTruncated");
        this.addPropertyIfExist(props, "IsLogged", regionInstrumentation, "isLogged");
        if (isLogged && instrumentedItem && instrumentedItem.UniqueId) {
            regionInstrumentation.appNS = instrumentedItem.UniqueId.Namespace;
            instrumentedItem.CurrentUseCount++;
            regionInstrumentation.k = instrumentedItem.UniqueId.KValue.toString();
            if (instrumentedItem.CurrentUseCount > 0) {
                regionInstrumentation.k += "." + instrumentedItem.CurrentUseCount.toString();
            }
        }
        return {
            regionInstrumentation: regionInstrumentation,
            isStartRegionLogging: false,
            isCloseRegionLogging: false,
            isLogged: isLogged,
            isEmptyChild: isEmptyChild,
            nodeName: nodeName,
            parentNodeName: parentNodeName
        };
    };
    Region.prototype.render = function () {
        return !this.state.isEmptyChild ? this.props.children : null;
    };
    return Region;
}(React.Component));
