
OPTION(COMPILE_DISPLAY "Compile Display library." ON)
MARK_AS_ADVANCED(COMPILE_DISPLAY)

IF(USE_QT_LIB)
  IF(USE_ITK_LIB)
	OPTION(COMPILE_IO "Compile IO library." ON)
	SET(NeuroLib_COMPILE_IO ${COMPILE_IO})
	MARK_AS_ADVANCED(COMPILE_IO)
  ENDIF(USE_ITK_LIB)
ENDIF(USE_QT_LIB)

#-----------------------------------------------------------------------------
# Compile disclaimer library
SUBDIRS(Disclaimer)



