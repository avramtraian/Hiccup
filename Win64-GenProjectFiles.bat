:: Copyright (c) 2022-2022 Avram Traian. All rights reserved.

@echo off
pushd "%~dp0"

call "Binaries/Win64/ThirdParty/Editor/premake/premake5.exe" --file="Solution.lua" vs2022

popd
pause