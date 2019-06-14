

#ifndef _GVFS_SHMEM_H_INCLUDED_
#define _GVFS_SHMEM_H_INCLUDED_


#include <gvfs_config.h>
#include <gvfs_core.h>


typedef struct {
	u_char      *addr;  /* ָ�����ڴ����ʼ��ַ */
	size_t       size;  /* �����ڴ�Ĵ�С */
	gvfs_str_t   name;  /* �����ڴ������ */
} gvfs_shm_t;


gvfs_int_t gvfs_shm_alloc(gvfs_shm_t *shm);
void gvfs_shm_free(gvfs_shm_t *shm);


#endif /* _GVFS_SHMEM_H_INCLUDED_ */
