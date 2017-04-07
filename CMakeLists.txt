cmake_minimum_required( VERSION 3.5 )

project( UltrasoundSprectroscopyRecorder CXX )

find_package( PlusLib REQUIRED )
include( ${PlusLib_USE_FILE} )

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

qt5_wrap_ui( mytest_UI_H UltrasoundSpectroscopyRecorder.ui )
qt5_wrap_cpp( source_files_moc myWindow.hxx )

set( source_files
  main.cxx
  myWindow.cxx
  myWindow.hxx 
  vtkIntersonSDKCxxVideoSourceWindow.cxx
  vtkIntersonSDKCxxVideoSourceWindow.h
  )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( UltrasoundSprectroscopyRecorder 
  ${source_files}
  ${mytest_UI_H}
  ${source_files_moc}
  )

target_link_libraries( UltrasoundSprectroscopyRecorder 
  ${VTK_LIBRARIES}
  ${PlusLib_LIBRARIES}
  Qt5::Widgets
  )
