#ifndef SVC_H
#define SVC_H

#include <stdio.h>
#include <string.h>

#define INSTALL "install"
#define START   "start"
#define STOP    "stop"
#define DELETE  "delete"

void    install_svc();
void    start_svc();
void    stop_svc();
void    delete_svc();

#endif