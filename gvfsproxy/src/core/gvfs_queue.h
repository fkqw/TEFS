
#ifndef _GVFS_QUEUE_H_INCLUDE_
#define _GVFS_QUEUE_H_INCLUDE_

#include <gvfs_config.h>
#include <gvfs_core.h>

typedef struct gvfs_queue_s  gvfs_queue_t;

struct gvfs_queue_s {
    gvfs_queue_t  *prev;
    gvfs_queue_t  *next;
};

/* ��ʼ������ */
#define gvfs_queue_init(q)                                                    \
    (q)->prev = q;                                                            \
    (q)->next = q

/* �ж϶����Ƿ�Ϊ�� */
#define gvfs_queue_empty(h)                                                   \
    (h == (h)->prev)

/* ��ͷ�ڵ�֮������½ڵ� */
#define gvfs_queue_insert_head(h, x)                                          \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define gvfs_queue_insert_after   gvfs_queue_insert_head

/* ��β�ڵ�֮������½ڵ� */
#define gvfs_queue_insert_tail(h, x)                                          \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define gvfs_queue_head(h)                                                    \
    (h)->next


#define gvfs_queue_last(h)                                                    \
    (h)->prev


#define gvfs_queue_sentinel(h)                                                \
    (h)


#define gvfs_queue_next(q)                                                    \
    (q)->next


#define gvfs_queue_prev(q)                                                    \
    (q)->prev

#define gvfs_queue_remove(x)                                                  \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

/* �ָ���� */
#define gvfs_queue_split(h, q, n)                                             \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

/* ���Ӷ��� */
#define gvfs_queue_add(h, n)                                                  \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define gvfs_queue_data(q, type, link)                                        \
    (type *) ((u_char *) q - offsetof(type, link))

/* ��ȡ�м�ڵ� */
gvfs_queue_t *gvfs_queue_middle(gvfs_queue_t *queue);
/* ������� */
void gvfs_queue_sort(gvfs_queue_t *queue,
    int (*cmp)(const gvfs_queue_t *, const gvfs_queue_t *));

#endif /* _GVFS_QUEUE_H_INCLUDE_ */
