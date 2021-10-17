SETLOCAL

set BinaryName=${LEMONS_PLUGIN_NAME}
set OrigDir=${LEMONS_PLUGIN_ORIG_DIR}

set SourcePath=%OrigDir%\%BinaryName%.xml

set AAXPlugin=%CommonProgramW6432%\Avid\Audio\Plug-Ins\%BinaryName%.aaxplugin
set DestDir=%AAXPlugin%\Contents\Resources

copy %SourcePath% %DestDir%

ENDLOCAL