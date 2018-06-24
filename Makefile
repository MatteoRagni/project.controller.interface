TARGET_EXEC := main.exe

SRCS := main.c libserialcomm.c libeserialcomm_interface.c
OBJS := libserialcomm.o libserialcomm_interface.o

CFLAGS := -g -I. -Wall
LDFLAGS := -lpthread -pthread -lwiringPi

default: $(TARGET_EXEC)

shared: $(OBJS)
	$(CC) $(OBJS) -o libserialcomm.so $(LDFLAGS) -shared -fPIC

$(TARGET_EXEC): main.o $(OBJS)
	$(CC) main.o $(OBJS) -o $@ $(LDFLAGS)

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(TARGET_EXE) $(OBJS) libserialcomm.so

-include $(DEPS)

MKDIR_P ?= mkdir -p
