# Microsoft Developer Studio Project File - Name="FaceRecAccessSys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FaceRecAccessSys - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FaceRecAccessSys.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FaceRecAccessSys.mak" CFG="FaceRecAccessSys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FaceRecAccessSys - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FaceRecAccessSys - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FaceRecAccessSys - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "FaceRecAccessSys - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gd /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /stack:0x989680 /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FaceRecAccessSys - Win32 Release"
# Name "FaceRecAccessSys - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DLLMethodForCSharp.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceAccessSys.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceRecAccessSys.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceRecAccessSys.def
# End Source File
# Begin Source File

SOURCE=.\FaceRecAccessSys.odl
# End Source File
# Begin Source File

SOURCE=.\FaceRecAccessSys.rc
# End Source File
# Begin Source File

SOURCE=.\FASCameraManage.cpp
# End Source File
# Begin Source File

SOURCE=.\FASFaceManage.cpp
# End Source File
# Begin Source File

SOURCE=.\FASLoginOrLogout.cpp
# End Source File
# Begin Source File

SOURCE=.\FASPolicyManage.cpp
# End Source File
# Begin Source File

SOURCE=.\FASRecognition.cpp
# End Source File
# Begin Source File

SOURCE=.\FASStatusQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_reader.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_value.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_writer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolsForExecute.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolsForFaceDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolsForFaceManage.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolsForPolicy.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolsForRecognition.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\json\autolink.h
# End Source File
# Begin Source File

SOURCE=.\json\config.h
# End Source File
# Begin Source File

SOURCE=.\DLLMethodForCSharp.h
# End Source File
# Begin Source File

SOURCE=.\FaceAccessSys.h
# End Source File
# Begin Source File

SOURCE=.\FaceRecAccessSys.h
# End Source File
# Begin Source File

SOURCE=.\FASCameraManage.h
# End Source File
# Begin Source File

SOURCE=.\FASFaceManage.h
# End Source File
# Begin Source File

SOURCE=.\FASLoginOrLogout.h
# End Source File
# Begin Source File

SOURCE=.\FASPolicyManage.h
# End Source File
# Begin Source File

SOURCE=.\FASRecognition.h
# End Source File
# Begin Source File

SOURCE=.\FASStatusQuery.h
# End Source File
# Begin Source File

SOURCE=.\json\features.h
# End Source File
# Begin Source File

SOURCE=.\json\forwards.h
# End Source File
# Begin Source File

SOURCE=.\json\json.h
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_batchallocator.h
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_internalarray.inl
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_internalmap.inl
# End Source File
# Begin Source File

SOURCE=.\lib_json\json_valueiterator.inl
# End Source File
# Begin Source File

SOURCE=.\json\reader.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\ToolsForExecute.h
# End Source File
# Begin Source File

SOURCE=.\ToolsForFaceDB.h
# End Source File
# Begin Source File

SOURCE=.\ToolsForFaceManage.h
# End Source File
# Begin Source File

SOURCE=.\ToolsForPolicy.h
# End Source File
# Begin Source File

SOURCE=.\ToolsForRecognition.h
# End Source File
# Begin Source File

SOURCE=.\json\value.h
# End Source File
# Begin Source File

SOURCE=.\json\writer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FaceRecAccessSys.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\Debug\msado15.tlh
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
