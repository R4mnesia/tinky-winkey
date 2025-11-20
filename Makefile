CC = cl
CFLAGS = /nologo /W3 /Wall /WX /I include
MANAGER = svc.exe
SERVICE = winkey.exe

SRCS_MANAGER = tinky/delete.c \
				tinky/install.c \
				tinky/start.c \
				tinky/stop.c \
				tinky/svc.c


all: $(MANAGER) $(SERVICE)

$(MANAGER): $(SRCS_MANAGER)
	$(CC) $(CFLAGS) $(SRCS_MANAGER) /Fe:$(MANAGER)

#$(SERVICE): service.c
#	$(CC) $(CFLAGS) service.c /Fe:$(SERVICE)

manager: $(MANAGER)

service: $(SERVICE)

clean:
	del *.obj
	del $(MANAGER)
#	del $(SERVICE)
