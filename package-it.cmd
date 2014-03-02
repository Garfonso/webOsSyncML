@rem if run in the windows script folder, change to parent
@REM @if not exist plugin ( pushd .. ) else pushd .

set STAGING_DIR=STAGING\info.mobo.webossyncml
set MOJO_DIR="mojo"
set JAVA_HOME="C:\Program Files\Java\jre6"

rmdir /s/q %STAGING_DIR%
del *.ipk
mkdir %STAGING_DIR%
xcopy /e/y %MOJO_DIR% %STAGING_DIR%
cs-make all
copy webOsSyncML_plugin %STAGING_DIR%
echo filemode.755=webOsSyncML_plugin > %STAGING_DIR%\package.properties
palm-package %STAGING_DIR%

pause

@REM popd