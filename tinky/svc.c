#include "svc.h"

int usage_error(void)
{
    fprintf(stderr, "Usage: .\\svc [options]\n\nThe options are:\n    install     Install service\n    start       Start service\n    stop        Stop service\n    delete      Delete service\n");
    return (-1);
}

int main(int argc, char **argv)
{
    (void)argv;
    if (argc != 2)
        return (usage_error());

    if (strcmp(argv[1], INSTALL) == 0)
        install_svc();
    else if (strcmp(argv[1], START) == 0)
        start_svc();
    else if (strcmp(argv[1], STOP) == 0)
        stop_svc();
    else if (strcmp(argv[1], DELETE) == 0)
        delete_svc();
    else
        return (usage_error());
    return (0);
}