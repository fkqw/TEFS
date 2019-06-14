

#ifndef _GVFS_EVENT_CONNECT_H_
#define _GVFS_EVENT_CONNECT_H_


#include <gvfs_config.h>
#include <gvfs_core.h>


typedef struct gvfs_peer_connection_s gvfs_peer_connection_t;

struct gvfs_peer_connection_s {
	gvfs_connection_t *connection;    /* �Զ�����                 */
	
    struct sockaddr   *sockaddr;      /* �Զ��׽��ֵ�ַ            */
    socklen_t          socklen;       /* �Զ��׽��ֵ�ַ����        */ 
	gvfs_str_t         addr_text;     /* �Զ˵�ַip:port�ַ�����ʽ */
    
    VOID              *data;          /* ָ��gvfs_http_request_t   */       
	INT32              rcvbuf;        /* ���ջ������Ĵ�С          */
	INT32              sendbuf;
};

gvfs_int_t gvfs_event_connect_peer(gvfs_peer_connection_t *pc);


#endif
