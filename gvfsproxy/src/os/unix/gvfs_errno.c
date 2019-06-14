/*
 * 1)����Linux�������ͨ��sys_errlist[errno]��ȡ��������,������澯
 * 2)GVFS_SYS_NERRͨ��sys_nerrֵ��ȡ
 * 3)Ϊ�β�ֱ��ʹ��strerror()��strerror_r()��ԭ���Ƿ��첽�źŰ�ȫ,strerror_r()
 *   �������̰߳�ȫ
 * 4)��ʵ�ֵ�����ֵ�������źŴ������а�ȫʹ��errnoֵ
 */


#include <gvfs_config.h>
#include <gvfs_core.h>


static gvfs_str_t  *gvfs_sys_errlist;
static gvfs_str_t   gvfs_unknown_error = gvfs_string("Unknown error");


u_char *
gvfs_strerror(gvfs_err_t err)
{
    gvfs_str_t  *msg;

    msg = ((gvfs_uint_t) err < GVFS_SYS_NERR) ? &gvfs_sys_errlist[err]:
                                                &gvfs_unknown_error;

    return msg->data;
}


gvfs_uint_t
gvfs_strerror_init(void)
{
	char       *msg;
	u_char     *p;
	size_t      len;
	gvfs_err_t  err;

	len = GVFS_SYS_NERR * sizeof(gvfs_str_t);

	gvfs_sys_errlist = malloc(len);
	if (gvfs_sys_errlist == NULL) {
		goto failed;
	}

    for (err = 0; err < GVFS_SYS_NERR; err++) {
        msg = strerror(err);
        len = gvfs_strlen(msg) + 1; /* ȷ�����������ַ�������\0��β�� */

        p = malloc(len);
        if (p == NULL) {
            goto failed;
        }

        gvfs_cpystrn((u_char *) p, (u_char *) msg, len);
        gvfs_sys_errlist[err].len = len;
        gvfs_sys_errlist[err].data = p;
    }

	return GVFS_OK;

failed:

	err = errno;
	gvfs_log(LOG_ERROR, "malloc(%lu) failed (%d: %s)", len, err, strerror(err));

	return GVFS_ERROR;
}
