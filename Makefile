CC=g++
SOURCE=easyAnalyser.cc
TARGET=analyse
INC= -Wall  -I$ROOTSYS/include 

${TARGET} :
	${CC} -o ${TARGET} ${SOURCE}  ${INC} 