

#include <gvfs_config.h>
#include <gvfs_core.h>
#include <gvfs_ds_interface.h>

VOID gvfs_ds_file_download_completed(gvfs_http_request_t *r,
	gvfs_rsp_head_t *rsp_head)
{
	gvfs_event_t *rev;
	gvfs_buf_t	 *headers_out;
	CHAR		  request_body[128] = {0};
	size_t	      content_length, bsize, rsize;

	headers_out = &r->headers_out;
	
	if (200 == rsp_head->rtn) {
		content_length = r->body_size;
		rev = r->connection->read;

		/* ���ͻ��˵��¼��Ӷ�ʱ�����Ƴ�,��������û��������ͳ�ʱ�ͷ����� */
		if (rev->timer_set) {
			gvfs_event_del_timer(rev);
		}
		
		gvfs_http_response_headers(headers_out, HTTP_Ok, content_length);
	}
	else  {
		content_length = snprintf(request_body, 128,
								 "rtn=%u",
								 rsp_head->rtn);
							 
		gvfs_http_response_headers(headers_out, HTTP_Ok, content_length);

		bsize = headers_out->end - headers_out->last;
		rsize = snprintf((CHAR *) headers_out->last, bsize,
				 "%s",
				 request_body);
			 
		headers_out->last += rsize;
	}
}

