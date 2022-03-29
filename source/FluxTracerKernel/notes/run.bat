@SET PATH=C:\Qt\Qt5.11.0\5.11.0\mingw53_32\bin;%PATH%
@REM @SET PATH=C:\Qt\Qt5.11.0\5.11.0\msvc2017_64\bin;%PATH%

@REM help
@REM FluxTracerKernel -h

@REM tests
@REM FluxTracerKernel -t

@REM run
@REM -i=input.xml for input file
@REM -m=1 for dividing rays into m groups
@REM -n=0 for reading rays from the group n
FluxTracerKernel -i=C:\Neo\Programming\Qt\Projects\FluxTracerQ\source\FluxTracerKernel\notes\input.xml -m=1 -n=0

@PAUSE
