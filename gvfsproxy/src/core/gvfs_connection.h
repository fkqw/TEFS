

#ifndef _GVFS_CONNECTION_H_INCLUDE_
#define _GVFS_CONNECTION_H_INCLUDE_

typedef struct gvfs_listening_s gvfs_listening_t;

/* �ýṹ��gvfs_create_listening�н��г�ʼ�� */
struct gvfs_listening_s
{
	gvfs_socket_t       fd;              /* ����������������          */

	struct sockaddr    *sockaddr;        /* �������������׽��ֵ�ַ     */
	socklen_t			socklen;         /* �������������׽��ֵ�ַ���� */
	gvfs_str_t          addr_text;       /* ����������IP���ַ�����ʽ   */

	int 				type;
	int 				backlog;         /* �������еĴ�С   */
	int 				rcvbuf;          /* ���ջ������Ĵ�С */
	int 				sndbuf;          /* ���ͻ������Ĵ�С */

	gvfs_connection_handler_pt	handler; /* ����accept֮��Ĵ�����: gvfs_http_init_connection */

	size_t              pool_size;       /* accpet�ɹ�֮��Ϊÿ�����ӷ�����ڴ�ش�С */

	gvfs_connection_t  *connection; 

    unsigned            listen:1;        /* ��־λ: �Ѿ����ù�listen���� */
};

struct gvfs_connection_s {
	gvfs_pool_t        *pool;        /* Ԥ��Ϊÿ�����ӷ�����ڴ��,accept�ɹ������ */ 
    void               *data;        /* ������;:δ����֮ǰָ���Լ�,��������,�����ָ��gvfs_http_request_t */
    gvfs_event_t       *read;
    gvfs_event_t       *write;

    gvfs_socket_t       fd;          /* accept���ص�������fd,��gvfs_get_connection�и�ֵ */

    gvfs_recv_pt        recv;        /* �հ�����ָ�� */
    gvfs_send_pt        send;        /* ��������ָ�� */
    off_t               sent;        /* �ѷ����ֽ��� */
    gvfs_listening_t   *listening;   /* �����Ӷ�Ӧ�ļ�����ַ�ṹ */
    
	gvfs_str_t          addr_text;   /* �ͻ��˵�ַ�ַ�����ʽ */
};

gvfs_listening_t *gvfs_create_listening(gvfs_cycle_t *cycle);
gvfs_int_t gvfs_open_listening_sockets(gvfs_cycle_t *cycle);
void gvfs_configure_listening_sockets(gvfs_cycle_t *cycle);
void gvfs_close_listening_sockets(gvfs_cycle_t *cycle);

gvfs_connection_t *gvfs_get_connection(gvfs_socket_t s);
void gvfs_free_connection(gvfs_connection_t *c);
void gvfs_close_connection(gvfs_connection_t *c);
gvfs_int_t gvfs_connection_error(gvfs_err_t err);


#endif
