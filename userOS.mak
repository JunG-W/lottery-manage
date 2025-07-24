CC:=gcc
CFLAGS:=-Iinclude
CFLAGS+=-c
TARGET:=./bin/userOS.exe
DEPEND:=./obj/genericlist.o ./obj/publicdata.o ./obj/datafile.o .\obj\lotterydata.o ./obj/userdata.o .\obj\display.o ./obj/userOS.o  

$(TARGET):$(DEPEND)
	$(CC) -o $@ $^

./obj/%.o:./src/%.c
	$(CC) -o $@ $(CFLAGS) $^

