CC:=gcc
CFLAGS:=-Iinclude
CFLAGS+=-c
TARGET:=./bin/lotteryOS
DEPEND:=./obj/genericlist.o ./obj/publicdata.o ./obj/datafile.o .\obj\lotterydata.o ./obj/userdata.o .\obj\display.o ./obj/lotteryOS.o

$(TARGET):$(DEPEND)
	$(CC) -o $@ $^

./obj/%.o:./src/%.c
	$(CC) -o $@ $(CFLAGS) $^


