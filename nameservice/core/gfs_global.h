#ifndef __GFS_GLOBAL_H__
#define __GFS_GLOBAL_H__
#include <gfs_config.h>
#include <gfs_mem.h>
#include <gfs_net.h>
#include <gfs_event.h>
#include <gfs_net_tcp.h>

typedef struct gfs_global_s
{
    gfs_char                bind_ip[32]; /* ������ip��ַ        */
    gfs_uint16              tport;       /* �����Ķ˿�,Ĭ��8082 */
    gfs_int16               timeout;
    gfs_int32               max_num;     /* ����֧�ֵ���������� */
    gfs_int32               client_num;  /* �����ӵ��û���       */
    gfs_int8                log_level;
    gfs_uint32              copies;      /* �ļ��챸�ݸ���       */
    gfs_uint32              block_size;  /* Ĭ���ļ����С       */

    gfs_int32               mysql_conn;        /* mysql ���ӳظ��� */
    gfs_char                mysql_host[32];    /* mysql ip */
    gfs_uint16              mysql_port;        /* mysql �Ķ˿� */
    gfs_char                mysql_db_name[32]; /* ���ݿ����� */
    gfs_char                mysql_user[32];    /* ���ݿ��û��� */
    gfs_char                mysql_pwd[32];     /* ���ݿ����� */

    gfs_mem_pool_t         *pool;
    gfs_net_loop_t         *nloop;
    gfs_event_loop_t       *eloop;
    gfs_net_tcpser_t       *tcpser;
}gfs_global_t;

gfs_int32   gfs_global_init();
gfs_void    gfs_global_destory();

gfs_int32   gfs_global_reload_cfg();

extern gfs_global_t *gfs_global;
extern gfs_int32     gfs_errno;
#endif
