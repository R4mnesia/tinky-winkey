# tinky-winkey

Within the mandatory part, you must use the following functions:

    ◦ OpenSCManager
    ◦ CreateService
    ◦ OpenService
    ◦ StartService
    ◦ ControlService
    ◦ CloseServiceHandle
    ◦ DuplicateTokenEx

## Mandatory part

### IV.1 Service
    • The program must be named svc.
    • The service must be named tinky.
    • The following options are available to handle the service:

        ◦ install
        ◦ start
        ◦ stop
        ◦ delete

    • Once the service starts running it must:

        ◦ Impersonate a SYSTEM token.
        ◦ Launch the Keylogger with it in the background.

    • Only one instance of the Keylogger can run.
    • On removal of the service, the Keylogger must be killed.

### IV.2 Keylogger
    
    • The program must be named winkey.
    • This must detect foreground processes.
    • This must save every keystroke related to the current foreground process.
    • Keyboard input must be handled in a low-level hook.
    • Timestamp, foreground process information and their related key stroke must be saved in a log file.
    • Keystroke input must be saved in a human readable format.
    • Keystroke input must be saved accordingly to the current locale identifier.

### Bonus part

You are free to add bonuses of your choice, but here are some interesting ideas:

    • Hide service and keylogger from their respective listing tools.
    • Be able to update the service during his runtime.
    • Clipboard, screen and/or microphone logging.
    • Applications, Users filtering.
    • Control text capture (capture password behind a password mask).
    • remote shell.

⚠️ This project is for educational purposes only.  
Do not use it for monitoring or collecting data without user consent