CC=g++
SOURCE=easyAnalyser.cc
TARGET=analyse
INC= -Wall  -I$ROOTSYS/include -std=c++11

${TARGET} :
	${CC} -o ${TARGET} ${SOURCE}  ${INC} 
