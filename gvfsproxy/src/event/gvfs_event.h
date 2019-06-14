

#ifndef _GVFS_EVENT_H_INCLUDED_
#define _GVFS_EVENT_H_INCLUDED_


#include <gvfs_config.h>
#include <gvfs_core.h>


struct gvfs_event_s {
	void                  *data;              /* ָ��gvfs_connection_t */
    unsigned               write:1;           /* ��־λ:Ϊ1ʱ��ʾ���Ӵ��ڿɷ�������״̬ */
    unsigned         	   accept:1;          /* ��־λ:Ϊ1ʱ��ʾ���¼����Խ����µ����� */
    unsigned               instance:1;        /* ��־λ:���������¼��Ƿ�Ϊ�����¼� */
    unsigned               active:1;          /* ��־λ:Ϊ1ʱ��ʾ�¼��ǻ�Ծ�� */
    unsigned               ready:1;           /* ��־λ:Ϊ1ʱ��ʾ�¼��Ѿ�׼������*/
    unsigned               eof:1;             /* ��־λ:Ϊ1ʱ��ʾ��ǰ���ֽ����Ѿ����� */
    unsigned               error:1;           /* ��־λ:Ϊ1ʱ��ʾ�ڴ���������������� */
    unsigned               timedout:1;        /* ��־λ:��ʾ����¼��Ѿ���ʱ */
    unsigned               deferred_accept:1; /* ��־λ:�ӳٽ������� */
    unsigned               channel:1;         /* ��־λ:��ʾ���¼���UNIXЭ�����׽��� */
    unsigned               timer_set:1;       /* ��־λ:��ʾ���¼��ڶ�ʱ���� */
    gvfs_event_handler_pt  handler;           /* �ֽ׶δ����߼��ĺ���ָ�� */
    gvfs_rbtree_node_t     timer;             /* ������ڵ�,���ڹ��ɺ������ʱ�� */
    struct gvfs_list_head  lh;                /* ���ӽڵ�,���ڹ����¼�������� */
};

typedef struct {
    gvfs_uint_t    connections;
    gvfs_uint_t    events;
} gvfs_event_conf_t;


extern gvfs_atomic_t         *gvfs_connection_counter;
extern sig_atomic_t           gvfs_event_timer_alarm;

extern gvfs_atomic_t         *gvfs_accept_mutex_ptr;
extern gvfs_shmtx_t           gvfs_accept_mutex;
extern gvfs_uint_t            gvfs_use_accept_mutex;
extern gvfs_uint_t            gvfs_accept_mutex_held;


extern gvfs_module_t           gvfs_event_core_module;

extern gvfs_event_conf_t     *g_gvfs_ecf;


#define GVFS_READ_EVENT     EPOLLIN     /* 00000001 */
#define GVFS_WRITE_EVENT    EPOLLOUT    /* 00000004 */
#define GVFS_LEVEL_EVENT    0           
#define GVFS_CLEAR_EVENT    EPOLLET     /* 80000000 */

#define GVFS_CLOSE_EVENT    1


#define GVFS_UPDATE_TIME    1
#define GVFS_POST_EVENTS    2


void gvfs_process_events_and_timers(gvfs_cycle_t *cycle);


gvfs_int_t gvfs_epoll_init(gvfs_cycle_t *cycle);
void gvfs_epoll_done(gvfs_cycle_t *cycle);


gvfs_int_t gvfs_epoll_add_event(gvfs_event_t *ev, gvfs_int_t event,
	gvfs_uint_t flags);
gvfs_int_t gvfs_epoll_del_event(gvfs_event_t *ev, gvfs_int_t event,
	gvfs_uint_t flags);

gvfs_int_t gvfs_epoll_add_connection(gvfs_connection_t *c);
gvfs_int_t gvfs_epoll_del_connection(gvfs_connection_t *c,
	gvfs_uint_t flags);

gvfs_int_t gvfs_epoll_process_events(gvfs_cycle_t *cycle,
	gvfs_uint_t timer, gvfs_uint_t flags);


void gvfs_event_accept(gvfs_event_t *ev);
gvfs_int_t gvfs_trylock_accept_mutex(gvfs_cycle_t *cycle);


#endif /* _GVFS_EVENT_H_INCLUDED_ */
