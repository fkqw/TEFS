

#ifndef _GVFS_PROCESS_H_INCLUDED_
#define _GVFS_PROCESS_H_INCLUDED_


#include <gvfs_setproctitle.h>


typedef pid_t       gvfs_pid_t;


#define GVFS_INVALID_PID           -1
#define GVFS_PROCESS_RESPAWN       -3
#define GVFS_MAX_PROCESSES          1024


typedef void (*gvfs_spawn_proc_pt) (gvfs_cycle_t *cycle, void *data);


typedef struct {
	gvfs_pid_t          pid;        /* ����ID */
	int                 status;     /* waitpid��ȡ�Ľ���״̬ */
	gvfs_socket_t       channel[2]; /* sockpairͨ�ŵľ�� */

	gvfs_spawn_proc_pt  proc;       /* �ӽ��̵�ѭ����ʽ,�ڸ����̴����ӽ���ʱ���� */
	void               *data;       /* ���ݸ�gvfs_spawn_proc_pt�ĵڶ������� */
	char               *name;       /* ��ʾ�Ľ������� */

	unsigned            respawn:1;  /* ��ʾ�ý����ǿ������´�����,������exit(2)ʱ,�ñ�־��Ϊ0 */
	unsigned            exited:1;   /* �����Ѿ��˳�,waitpid�ĵط��øñ�־λ */
} gvfs_process_t;


gvfs_pid_t gvfs_spawn_process(gvfs_cycle_t *cycle,
    gvfs_spawn_proc_pt proc, void *data, char *name, gvfs_int_t respawn);
gvfs_int_t gvfs_init_signals(void);


extern int             gvfs_argc;
extern char          **gvfs_argv;
extern char          **gvfs_os_argv;

extern gvfs_pid_t      gvfs_pid;
extern gvfs_socket_t   gvfs_channel;
extern gvfs_int_t      gvfs_process_slot;   // ��ǰ�����Ľ�����process�����е��±�
extern gvfs_int_t      gvfs_last_process;   // process���������������±�
extern gvfs_process_t  gvfs_processes[GVFS_MAX_PROCESSES];


#endif /* _GVFS_PROCESS_H_INCLUDED_ */
