

#ifndef _GVFS_ERRNO_H_INCLUDED_
#define _GVFS_ERRNO_H_INCLUDED_


#include <gvfs_config.h>
#include <gvfs_core.h>


typedef int                gvfs_err_t;

#define GVFS_SYS_NERR      135             /* Linuxƽ̨ͨ��sys_nerrֵ��ȡ */


u_char *gvfs_strerror(gvfs_err_t err);
gvfs_uint_t gvfs_strerror_init(void);


#endif /* _GVFS_ERRNO_H_INCLUDED_ */
