// --------------------------------------------------------------------------------------------------------------------
// <copyright file="V8ValueType.cs" company="Microsoft">
//     Copyright (c) Microsoft. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace JsRT.V8
{
    using System;

    [Flags]
    public enum V8ValueType
    {
        Null = 0,
        Undefined = 1,
        Boolean = 2,
        Integer = 3,
        Double = 4,
        String = 5,
        Object = 6,
        Array = 7,
        UnknownType = 8
    }
}
