#ifndef VNODE_H
#define VNODE_H

#include <stddef.h>
#include <stdint.h>

struct vattr {
    uint32_t file_size;
};

typedef struct vattr vattr_t;

struct vnodeops;

struct vnode {
    struct vnodeops *v_op;
    uint32_t v_data;
};
typedef struct vnode vnode_t;

struct vnodeops {
    int (*vn_open)(vnode_t *node);
    int (*vn_lookup)(vnode_t *dir, char const *name, vnode_t *res);
    int (*vn_read)(vnode_t *node, void *buf, size_t count);
    int (*vn_write)(vnode_t *node, char const *buf, size_t count);
    int (*vn_getattr)(vnode_t *node, vattr_t *attr);
};
typedef struct vnodeops vnodeops_t;

void vnode_copy(vnode_t *from, vnode_t *to);

#endif