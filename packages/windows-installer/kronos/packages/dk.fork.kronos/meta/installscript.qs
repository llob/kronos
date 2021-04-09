function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/kronos.exe", "@StartMenuDir@/kronos.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/kronos.ico",
            "iconId=2", "description=Kronos");
    }
}