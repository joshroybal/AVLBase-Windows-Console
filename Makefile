CC = cl.exe
CFLAGS = /EHsc /c /O2
LDFLAGS =

#%.obj: %.cpp
#	$(CC) $(CFLAGS) $< -o $@
		  
objects = obj\main.obj obj\avl.obj obj\bst.obj obj\fileio.obj \
obj\winscreenio.obj

bin\avlbase.exe: $(objects)
	cl /Fe$@ $(objects)

obj\main.obj: src\main.cpp src\avl.hpp src\fileio.hpp src\winscreenio.hpp
	cl $(CFLAGS) src\main.cpp /Fo$@

obj\bst.obj: src\bst.cpp src\bst.hpp
	cl $(CFLAGS) src\bst.cpp /Fo$@
	
obj\avl.obj: src\avl.cpp src\avl.hpp src\bst.hpp
	cl $(CFLAGS) src\avl.cpp /Fo$@

obj\fileio.obj: src\fileio.cpp src\fileio.hpp src\winscreenio.hpp
	cl $(CFLAGS) src\fileio.cpp /Fo$@

obj\winscreenio.obj: src\winscreenio.cpp src\winscreenio.hpp src\fileio.hpp
	cl $(CFLAGS) src\winscreenio.cpp /Fo$@

.PHONY : clean	
clean:
	del obj\*.obj bin\*.exe