

#include <gvfs_config.h>
#include <gvfs_core.h>


extern char **environ;

static char *gvfs_os_argv_last;

gvfs_int_t gvfs_init_setproctitle(void)
{
    u_char       *p;
    size_t        size;
    gvfs_uint_t   i;

    size = 0;

    for (i = 0; environ[i]; i++) {
        size += gvfs_strlen(environ[i]) + 1;
    }

    p = gvfs_alloc(size);
    if (p == NULL) {
        return GVFS_ERROR;
    }

    gvfs_os_argv_last = gvfs_os_argv[0];

	/* ���������в���,ָ�����һ�������в�����β */
    for (i = 0; gvfs_os_argv[i]; i++) {
        if (gvfs_os_argv_last == gvfs_os_argv[i]) {
            gvfs_os_argv_last = gvfs_os_argv[i] + gvfs_strlen(gvfs_os_argv[i]) + 1;
        }
    }

	/* �������������·���Ķ��ڴ���,Ȼ����ȫ�ֵĻ���ָ��������ָ��,����������ƻ� */
    for (i = 0; environ[i]; i++) {
        if (gvfs_os_argv_last == environ[i]) {
            size = gvfs_strlen(environ[i]) + 1;
            gvfs_os_argv_last = environ[i] + size;

			/* �ڲ�ʵ���Ѿ��ڽ�β���˽����� */
            gvfs_cpystrn(p, (u_char *) environ[i], size);
            environ[i] = (char *) p;
            p += size;
        }
    }

    gvfs_os_argv_last--; /* ����+1,ָ��'\0' */

    return GVFS_OK;
}


void
gvfs_setproctitle(char *title)
{
    u_char     *p;
    gvfs_os_argv[1] = NULL;

    p = gvfs_cpystrn((u_char *) gvfs_os_argv[0], (u_char *) "gvfs_proxy: ",
                    gvfs_os_argv_last - gvfs_os_argv[0]);

    p = gvfs_cpystrn(p, (u_char *) title, gvfs_os_argv_last - (char *) p);

    if (gvfs_os_argv_last - (char *) p) {
        gvfs_memset(p, GVFS_SETPROCTITLE_PAD, gvfs_os_argv_last - (char *) p);
    }

    gvfs_log(LOG_INFO, "setproctitle: \"%s\"", gvfs_os_argv[0]);
}
