CC:=gcc
CFLAGS:=-Iinclude
CFLAGS+=-c
TARGET:=./bin/officeOS
DEPEND:=./obj/genericlist.o ./obj/publicdata.o ./obj/datafile.o .\obj\lotterydata.o ./obj/userdata.o .\obj\display.o ./obj/officeOS.o

$(TARGET):$(DEPEND)
	$(CC) -o $@ $^

./obj/%.o:./src/%.c
	$(CC) -o $@ $(CFLAGS) $^


