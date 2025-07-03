



xcopy /y/s .\Engine\Public\*.*			.\EngineSDK\Inc\
xcopy /y .\Engine\Bin\*.lib				.\EngineSDK\Lib\
xcopy /y .\Engine\ThirdPartyLib\*.lib	.\EngineSDK\Lib\
xcopy /y .\Engine\Bin\*.dll				.\Client\Bin\
xcopy /y .\Engine\Bin\ShaderFiles\*.*	.\Client\Bin\ShaderFiles\