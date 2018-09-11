
#*******************************************************************#
#makefile for navierstokes1.cpp
#******************variables*******************************#
GXX=g++
GXXFLAGS=-Wall -g
XXX=-pthread -m64 -I/usr/local/include/root
YYY=-L/usr/local/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic



#*******************targerts****************************#
navierstokes1: navierstokes1.o
	$(GXX) $(GXXFLAGS) -o navierstokes1 navierstokes1.cpp $(XXX) $(YYY)
navierstokes1.o: navierstokes1.cpp
	$(GXX) $(GXXFLAGS) -c  navierstokes1.cpp $(XXX) $(YYY)


clean:
	rm -f Flujo.gif
	rm -f navierstokes1
	rm -f navierstokes1.o
