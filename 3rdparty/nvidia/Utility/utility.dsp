# Microsoft Developer Studio Project File - Name="utility" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=utility - Win32 Debug64MT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "utility.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "utility.mak" CFG="utility - Win32 Debug64MT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "utility - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 DebugMT" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 ReleaseMT" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 Debug64" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 Release64" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 Release64MT" (based on "Win32 (x86) Static Library")
!MESSAGE "utility - Win32 Debug64MT" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "utility"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "utility - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D FLOAT_SIZE=32 /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Libraries\Release\utility.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "KILL_WARNINGS" /D FLOAT_SIZE=32 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Libraries\Debug\utility.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 DebugMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "utility___Win32_DebugMT"
# PROP BASE Intermediate_Dir "utility___Win32_DebugMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\OBJ\utility\DebugMT"
# PROP Intermediate_Dir "..\OBJ\utility\DebugMT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /I "..\gformats\tiff\libtiff" /I "..\gformats\jpeg" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Libraries\Debug\utilityMT.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 ReleaseMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "utility___Win32_ReleaseMT"
# PROP BASE Intermediate_Dir "utility___Win32_ReleaseMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\OBJ\utility\ReleaseMT"
# PROP Intermediate_Dir "..\OBJ\utility\ReleaseMT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".\\" /I "..\gformats\tiff\libtiff" /I "..\gformats\jpeg" /I "..\gformats\gbm" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libraries\Release\utility.lib"
# ADD LIB32 /nologo /out:"..\..\Libraries\Release\utilityMT.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 Debug64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "utility___Win32_Debug64"
# PROP BASE Intermediate_Dir "utility___Win32_Debug64"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "utility___Win32_Debug64"
# PROP Intermediate_Dir "utility___Win32_Debug64"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\gformats\include" /I "..\include" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "KILL_WARNINGS" /D FLOAT_SIZE=32 /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "KILL_WARNINGS" /D FLOAT_SIZE=64 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Libraries\Debug\utility.lib"
# ADD LIB32 /nologo /out:"..\..\Libraries\Debug\utility64.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 Release64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "utility___Win32_Release64"
# PROP BASE Intermediate_Dir "utility___Win32_Release64"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "utility___Win32_Release64"
# PROP Intermediate_Dir "utility___Win32_Release64"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D FLOAT_SIZE=64 /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D FLOAT_SIZE=64 /D "FAST_MATH" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Libraries\Release\utility.lib"
# ADD LIB32 /nologo /out:"..\..\Libraries\Release\utility64.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 Release64MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "utility___Win32_Release64MT"
# PROP BASE Intermediate_Dir "utility___Win32_Release64MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "utility___Win32_Release64MT"
# PROP Intermediate_Dir "utility___Win32_Release64MT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /D FLOAT_SIZE=64 /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Libraries\Release\utilityMT.lib"
# ADD LIB32 /nologo /out:"..\..\Libraries\Release\utility64MT.lib"

!ELSEIF  "$(CFG)" == "utility - Win32 Debug64MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "utility___Win32_Debug64MT"
# PROP BASE Intermediate_Dir "utility___Win32_Debug64MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "utility___Win32_Debug64MT"
# PROP Intermediate_Dir "utility___Win32_Debug64MT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\gformats\include" /I "..\include" /I "..\..\include" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "MULTI_THREAD" /D "KILL_WARNINGS" /D FLOAT_SIZE=64 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Libraries\Debug\utilityMT.lib"
# ADD LIB32 /nologo /out:"..\..\Libraries\Debug\utility64MT.lib"

!ENDIF 

# Begin Target

# Name "utility - Win32 Release"
# Name "utility - Win32 Debug"
# Name "utility - Win32 DebugMT"
# Name "utility - Win32 ReleaseMT"
# Name "utility - Win32 Debug64"
# Name "utility - Win32 Release64"
# Name "utility - Win32 Release64MT"
# Name "utility - Win32 Debug64MT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ulBig.cpp
# End Source File
# Begin Source File

SOURCE=.\ulBsp.cpp
# End Source File
# Begin Source File

SOURCE=.\ulDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\ulGeom.cpp
# End Source File
# Begin Source File

SOURCE=.\ulMat3.cpp
# End Source File
# Begin Source File

SOURCE=.\ulMat4.cpp
# End Source File
# Begin Source File

SOURCE=.\ulMatQ.cpp
# End Source File
# Begin Source File

SOURCE=.\ulMatR.cpp
# End Source File
# Begin Source File

SOURCE=.\ulMatStack.cpp
# End Source File
# Begin Source File

SOURCE=.\ulNormal.cpp
# End Source File
# Begin Source File

SOURCE=.\ulOrientation.cpp
# End Source File
# Begin Source File

SOURCE=.\ulPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\ulQuat.cpp
# End Source File
# Begin Source File

SOURCE=.\ulRescale.cpp
# End Source File
# Begin Source File

SOURCE=.\ulSinCos.Cpp
# End Source File
# Begin Source File

SOURCE=.\ulSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\ulSvd.cpp
# End Source File
# Begin Source File

SOURCE=.\ulWindows.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BinSort.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Getopt.h
# End Source File
# Begin Source File

SOURCE=..\..\include\NV_Common.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\UL.h
# End Source File
# Begin Source File

SOURCE=.\ulBBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ulBig.h
# End Source File
# Begin Source File

SOURCE=.\ulBsp.h
# End Source File
# Begin Source File

SOURCE=.\ulCommon.h
# End Source File
# Begin Source File

SOURCE=.\ulMat3.h
# End Source File
# Begin Source File

SOURCE=.\ulMat4.h
# End Source File
# Begin Source File

SOURCE=.\ulMatQ.h
# End Source File
# Begin Source File

SOURCE=.\ulMatR.h
# End Source File
# Begin Source File

SOURCE=.\ulMatrix.h
# End Source File
# Begin Source File

SOURCE=.\ulMatrix1.h
# End Source File
# Begin Source File

SOURCE=.\ulMatrixVec.h
# End Source File
# Begin Source File

SOURCE=.\ulOrientation.h
# End Source File
# Begin Source File

SOURCE=.\ulQuat.h
# End Source File
# Begin Source File

SOURCE=.\ulVec2.h
# End Source File
# Begin Source File

SOURCE=.\ulVec3.h
# End Source File
# Begin Source File

SOURCE=.\ulVec4.h
# End Source File
# Begin Source File

SOURCE=.\ulVector.h
# End Source File
# Begin Source File

SOURCE=.\ulVector1.h
# End Source File
# End Group
# End Target
# End Project
