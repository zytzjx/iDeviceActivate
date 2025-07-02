## For Apple Device Activate

This code flow is from [libimobileDevice](https://github.com/libimobiledevice/libideviceactivation) .  
Using Apple MobileDevice.dll and Corefoundation talk with device  
Now the project Only support X86, NOT SUPPORT X64.

### usage
```
Apple Device activate


iDeviceActivate.exe [OPTIONS] command


POSITIONALS:
  command TEXT:{activate,deactivate,activatestatus} REQUIRED
                              Operation to perform

OPTIONS:
  -h,     --help              Print this help message and exit
  -v,     --verbose [0]       Enable verbose output
  -l,     --label INT:POSITIVE
                              label for lot
  -u,     --udid TEXT         Use UDID instead of serial number
  -t,     --timeout INT:POSITIVE [10000]
                              Wait timeout for device in milliseconds
```


### TODO
SUPPORT WINDOWS X64