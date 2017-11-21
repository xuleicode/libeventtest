libeventThread : libeventTest.o WorkThread.o dataParser.o Log.o ReadConfig.o
	g++ -g libeventTest.o WorkThread.o dataParser.o Log.o ReadConfig.o -o libeventThread -I/usr/include/mysql  -L/usr/lib/mysql -ljson -lmysqlclient -levent
libeventTest.o : libeventTest.cpp stdafx.h dataParser.h WorkThread.h Log.h 
	g++ -g -c libeventTest.cpp stdafx.h dataParser.h WorkThread.h Log.h -levent
WorkThread.o : WorkThread.cpp WorkThread.h dataParser.h stdafx.h Log.h ReadConfig.h
	g++ -g -c WorkThread.cpp WorkThread.h dataParser.h stdafx.h Log.h ReadConfig.h -I/usr/include/mysql  -L/usr/lib/mysql -ljson -lmysqlclient
dataParser.o : dataParser.cpp dataParser.h stdafx.h  Log.h
	g++ -g -c dataParser.cpp dataParser.h stdafx.h  Log.h -ljson
Log.o : Log.cpp stdafx.h  Log.h
	g++ -g -c Log.cpp stdafx.h  Log.h
ReadConfig.o :ReadConfig.cpp stdafx.h ReadConfig.h
	g++ -g -c ReadConfig.cpp stdafx.h ReadConfig.h
clean:
	rm libeventThread libeventTest.o workThread.o dataParser.o Log.o ReadConfig.o