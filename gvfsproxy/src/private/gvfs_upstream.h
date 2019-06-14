

#ifndef _GVFS_UPSTREAM_H_INCLUDE_
#define _GVFS_UPSTREAM_H_INCLUDE_


#include <gvfs_config.h>
#include <gvfs_core.h>

extern const CHAR *ns_ip_addr;
extern INT32       ns_port;

typedef struct gvfs_upstream_conf_s {
	CHAR	 *ip_addr;
	INT32     port;
} gvfs_upstream_conf_t;

struct gvfs_rsp_head_s
{
	VOID		  *data;
	VOID          *rsh;
    unsigned int   len;
    unsigned int   id;
    unsigned int   type;
    unsigned int   encrypt;
    unsigned int   rtn;
};

struct gvfs_upstream_s {
	gvfs_pool_t                       *pool; /* �̳���gvfs_connection_t�������·��� */     
	gvfs_buf_t                        *download_body;
	gvfs_buf_t                         buf;  /* ��buf����ʵ�ַ��Ͷ˷ְ����͵����   */
	gvfs_peer_connection_t             peer; /* �ͶԶ˵����ӽṹ                    */
	gvfs_rsp_head_t                    rsp_head;

	UINT32                             sid; 
	UINT16                             header_completed; /* ��־λ: ��Ϣͷ�Ѿ�������� */
	UINT16                             body_completed;   /* ��־λ: ��Ϣ���Ѿ�������� */
};


extern gvfs_upstream_conf_t *g_gvfs_ucf;

    
#endif

