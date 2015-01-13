#PROJECT(AutoSeg)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)

FIND_PACKAGE(FLTK REQUIRED)
FIND_PACKAGE(ITK REQUIRED)
IF (ITK_FOUND)
   INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
   MESSAGE(FATAL_ERROR, "ITK not found. Please set ITK_DIR.")
ENDIF (ITK_FOUND)

FIND_PACKAGE(BatchMake REQUIRED)
INCLUDE(${BatchMake_USE_FILE})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/About
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/AddData
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/AddAuxData
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/AutoSeg
  ${CMAKE_CURRENT_SOURCE_DIR}/Gui/TextDisplay
  ${FLTK_INCLUDE_PATH}
)

SET(CXX_FILES 
  main.cxx
  AutoSegComputation.cxx
  Gui/About/AboutGUIControls.cxx
  Gui/AddData/AddDataGUIControls.cxx
  Gui/AddAuxData/AddAuxDataGUIControls.cxx
  Gui/AutoSeg/AutoSegGUIControls.cxx
  Gui/AutoSeg/ResizeBrowser.cxx
  Gui/TextDisplay/TextDisplayGUIControls.cxx  
)

SET(H_FILES
  AutoSegComputation.h
  Gui/About/AboutGUIControls.h
  Gui/AddData/AddDataGUIControls.h
  Gui/AddAuxData/AddAuxDataGUIControls.h
  Gui/AutoSeg/AutoSegGUIControls.h
  Gui/AutoSeg/ResizeBrowser.h
  Gui/TextDisplay/TextDisplayGUIControls.h  
)

SET(GUI_FILES
  Gui/About/AboutGUI.fl
  Gui/AddData/AddDataGUI.fl
  Gui/AddAuxData/AddAuxDataGUI.fl
  Gui/AutoSeg/AutoSegGUI.fl
  Gui/TextDisplay/TextDisplayGUI.fl  
)

LINK_DIRECTORIES( ${FLTK_LIBRARY_DIRS})
FLTK_WRAP_UI(AutoSeg ${GUI_FILES})

ADD_EXECUTABLE(AutoSeg ${CXX_FILES} ${H_FILES} ${AutoSeg_FLTK_UI_SRCS})

TARGET_LINK_LIBRARIES(AutoSeg
  ${ITK_LIBRARIES}
  ${FLTK_LIBRARIES}
  BatchMakeLib
)

install(TARGETS
  AutoSeg
  BUNDLE DESTINATION  bin
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)

