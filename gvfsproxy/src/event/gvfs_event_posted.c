

#include <gvfs_config.h>
#include <gvfs_core.h>
#include <gvfs_event.h>


struct gvfs_list_head gvfs_posted_accept_events; /* accept�¼��б�   */
struct gvfs_list_head gvfs_posted_events;        /* ��accept�¼��б� */


void
gvfs_event_process_posted(struct gvfs_list_head *posted)
{
	gvfs_event_t          *ev;
	struct gvfs_list_head *lh, *tmp;

	list_for_each_safe(lh, tmp, posted) {
		ev = list_entry(lh, gvfs_event_t, lh);

		if (NULL == ev) {
			continue;
		}
		
		gvfs_delete_posted_event(ev);
		INIT_LIST_HEAD(&ev->lh);
		
		ev->handler(ev);
	}
}


