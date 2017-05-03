Ultrasound Spectroscopy Recorder
================================

A Plus-based application to record ultrasound images with the Interson GP 3.5 probe or the Interson Array SP-L01 probe at different frequencies and powers. The images are displayed in B-mode and recorded in Rf mode. 

Requirements
============
Build each in "Release" configuration and in the following order:
# Qt5 (Yup, 5)
# VTK
## Note that Slicer's VTK uses an external zlib, so compile VTK from scratch instead of using Slicer's VTK.
## github.com:/Kitware/VTK
## Enable VTK_Group_Qt
## Specify use of QT_VERSION 5
## If using installation of Qt5 from trolltech, specify CMAKE_PREFIX_PATH = c:\src\qt5\5.8\msvc2013_64 (or equivalent.  You will need to add this variable to the cmake configuration (e.g., use AddEntry in cmake-gui).  Then it will automatically find your Qt5_DIR.
# ITK
## github.com:/Kitware/ITK
## To get the most up-to-date version, do not use the version in Slicer.
# IntersonSDK
## This is a proprietary DLL from Interson.  Contact them for a copy.
# IntersonArraySDK
## This is a proprietary DLL from Interson.  Contact them for a copy.
# IntersonSDKCxx (Must be INSTALLED - cannot use build dir)
## github.com:/KitwareMedical/IntersonSDKCxx
# IntersonArraySDKCxx (Must be INSTALLED - cannot use build dir)
## github.com:/KitwareMedical/IntersonSDKCxx
# PlusLib
## Enable IntersonSDKCxx and point to directory in which it is INSTALLED
## Enable IntersonArraySDKCxx and point to directory in which it is INSTALLED

You also need to pass in argument the config-file used for the Interson probe.

Usage:
UltrasoundSpectroscopyRecorder.exe --config-file=PlusDeviceSet_Server_IntersonArraySDKCxx_SP-L01.xml

DLL Hell:
If you want to run this from the command-line (versus from within visual studio), you are likely to end up with the executable starting and then immediately ending.   This is likely to be due to missing dlls.   You can use "dependency walker" to try to diagnose the dlls that are missing.

In general, you will need dlls for the following to be in your environment's PATH or copied into the executable's directory:
# QT5
# VTK
# ITK
# IntersonSDK (proprietary, contact Interson)
# IntersonArraySDK (proprietary, contact Interson)
# IntersonSDKCxx
# intersonArraySDKCxx
