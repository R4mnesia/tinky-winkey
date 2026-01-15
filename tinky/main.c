#include "svc.h"

int usage_error(void)
{
    fprintf(stderr, "Usage: .\\svc [options]\n\nThe options are:\n    install     Install service\n    start       Start service\n    stop        Stop service\n    delete      Delete service\n");
    return (-1);
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (strcmp(argv[1], INSTALL) == 0)
            install_svc(argv[0]);
        else if (strcmp(argv[1], START) == 0)
            start_svc();
        else if (strcmp(argv[1], STOP) == 0)
            stop_svc();
        else if (strcmp(argv[1], DEL) == 0)
            delete_svc();
        else
            return (usage_error());
        return (0);
    }

    // service mode
    SERVICE_TABLE_ENTRY table[] = {
        { SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    // this function connect executable to the Service Control Manager SCM
    StartServiceCtrlDispatcher(table);

    return (0);
}