@echo *************************************************
@echo copy files to .\lib (*.dll,*.pbd)
@echo *************************************************
@pause
@copy ext\wxWidgets\lib\vc_dll\*.dll 		lib
@copy ext\wxWidgets\lib\vc_dll\*.pdb 		lib
@copy ext\lua\src\*.dll 			lib
@copy ext\physfs\debug\*.dll 			lib
@copy ext\physfs\debug\*.pdb 			lib
@copy ext\devil\lib\*.dll 			lib
@copy ext\glew\bin\*.dll 			lib
@pause