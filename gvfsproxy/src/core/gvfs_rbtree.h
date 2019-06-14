
#ifndef _GVFS_RBTREE_H_INCLUDED_
#define _GVFS_RBTREE_H_INCLUDED_


#include <gvfs_config.h>
#include <gvfs_core.h>


typedef gvfs_uint_t  gvfs_rbtree_key_t;
typedef gvfs_int_t   gvfs_rbtree_key_int_t;


typedef struct gvfs_rbtree_node_s  gvfs_rbtree_node_t;

struct gvfs_rbtree_node_s {
    gvfs_rbtree_key_t       key;       /* �ؼ��ֽڵ� */
    gvfs_rbtree_node_t     *left;      /* ���ӽڵ�ָ�� */
    gvfs_rbtree_node_t     *right;     /* �Һ��ӽڵ�ָ�� */
    gvfs_rbtree_node_t     *parent;    /* ˫�׽ڵ�ָ�� */
    u_char                  color;     /* �ڵ���ɫ */
};


typedef struct gvfs_rbtree_s  gvfs_rbtree_t;

typedef void (*gvfs_rbtree_insert_pt) (gvfs_rbtree_node_t *root,
    gvfs_rbtree_node_t *node, gvfs_rbtree_node_t *sentinel);

struct gvfs_rbtree_s {
    gvfs_rbtree_node_t     *root;      /* ָ����ڵ�,���ڵ�Ҳ��һ�����ݽڵ� */
    gvfs_rbtree_node_t     *sentinel;  /* ָ��NIL�ڱ��ڵ� */
    gvfs_rbtree_insert_pt   insert;    /* ��ӽڵ�ʱ���������������滻�ڵ� */
};


#define gvfs_rbtree_init(tree, s, i)                                           \
    gvfs_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->insert = i


void gvfs_rbtree_insert(gvfs_rbtree_t *tree, gvfs_rbtree_node_t *node);
void gvfs_rbtree_delete(gvfs_rbtree_t *tree, gvfs_rbtree_node_t *node);

/* Ԥ�ṩ��2�����뷽ʽ,Ҳ���Զ�����뷽�� */
void gvfs_rbtree_insert_value(gvfs_rbtree_node_t *root, gvfs_rbtree_node_t *node,
    gvfs_rbtree_node_t *sentinel);
void gvfs_rbtree_insert_timer_value(gvfs_rbtree_node_t *root,
    gvfs_rbtree_node_t *node, gvfs_rbtree_node_t *sentinel);


#define gvfs_rbt_red(node)               ((node)->color = 1)
#define gvfs_rbt_black(node)             ((node)->color = 0)
#define gvfs_rbt_is_red(node)            ((node)->color)
#define gvfs_rbt_is_black(node)          (!gvfs_rbt_is_red(node))
#define gvfs_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define gvfs_rbtree_sentinel_init(node)  gvfs_rbt_black(node)


static inline gvfs_rbtree_node_t *
gvfs_rbtree_min(gvfs_rbtree_node_t *node, gvfs_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}


#endif /* _GVFS_RBTREE_H_INCLUDED_ */
