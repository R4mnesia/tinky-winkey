CC = cl
CFLAGS = /nologo /W3 /Wall /WX /I include
MANAGER = svc.exe
SERVICE = winkey.exe

SRCS_MANAGER = tinky/delete.c \
				tinky/remote_shell.c \
				tinky/install.c \
				tinky/start.c \
				tinky/stop.c \
				tinky/utils.c \
				tinky/svc.c \
				tinky/main.c

SRCS_WINKEY = winkey/winkey.c \
			  winkey/obfuscation_utils.c \
			  winkey/logs_utils.c \
			  winkey/logs.c

			  
all: $(MANAGER) $(SERVICE)

$(MANAGER): $(SRCS_MANAGER)
	$(CC) $(CFLAGS) $(SRCS_MANAGER) /Fe:$(MANAGER) /link Advapi32.lib

$(SERVICE): $(SRCS_WINKEY)
	$(CC) $(CFLAGS) /wd4668 $(SRCS_WINKEY) /Fe:$(SERVICE)  /link  Advapi32.lib user32.lib

manager: $(MANAGER)

service: $(SERVICE)

clean: 
	del *.obj

fclean: clean
	del $(MANAGER)
	del $(SERVICE)
