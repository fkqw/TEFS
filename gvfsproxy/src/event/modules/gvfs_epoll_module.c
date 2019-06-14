

#include <gvfs_config.h>
#include <gvfs_core.h>
#include <gvfs_event.h>


static int                  ep = -1;
static struct epoll_event  *event_list;
static gvfs_uint_t          nevents;

gvfs_int_t gvfs_epoll_init(gvfs_cycle_t *cycle)
{
    if (ep == -1) {
        ep = epoll_create(cycle->connection_n / 2);

        if (ep == -1) {
            gvfs_log(LOG_ERROR, "epoll_create() failed, error: %s",
            					gvfs_strerror(errno));
            return GVFS_ERROR;
        }
    }

    if (nevents < g_gvfs_ecf->events) {
        if (event_list) {
            gvfs_free(event_list);
        }

        event_list = gvfs_alloc(sizeof(struct epoll_event) * g_gvfs_ecf->events);
        if (event_list == NULL) {
        	gvfs_log(LOG_ERROR, "gvfs_alloc(%lu) for event_list failed",
        						sizeof(struct epoll_event) * g_gvfs_ecf->events);
            return GVFS_ERROR;
        }
    }

    nevents = g_gvfs_ecf->events;

	gvfs_log(LOG_DEBUG, "epoll_create() fd: %d success", ep);
	
    return GVFS_OK;

}

void gvfs_epoll_done(gvfs_cycle_t *cycle)
{
	gvfs_err_t err;
	
	if (-1 == close(ep)) {
		err = errno;
		gvfs_log(LOG_ERROR, "epoll close(%d) failed error: %s",
							ep, gvfs_strerror(errno));
	}

	ep = -1;

	gvfs_free(event_list);

	event_list = NULL;
	nevents = 0;
}


/* 
 * ��������¼�
 * @event �¼����� GVFS_READ_EVENT/GVFS_WRITE_EVENT
 * @flags ������ʽ GVFS_LEVEL_EVENT/GVFS_CLEAR_EVENT
 */
gvfs_int_t
gvfs_epoll_add_event(gvfs_event_t *ev, gvfs_int_t event,
	gvfs_uint_t flags)
{
	int 				 op;
	gvfs_int_t           err;
	uint32_t			 events, prev;
	gvfs_event_t 		*e;
	gvfs_connection_t	*c;
	struct epoll_event	 ee;

	c = ev->data;

	events = (uint32_t) event;

	/* ��Ӷ��¼�,�ж��Ƿ�����ӹ�д�¼�,��ӹ�д�¼�activeΪture */
	if (event == GVFS_READ_EVENT) { 
		e = c->write;
		prev = EPOLLOUT;
	} else {
		e = c->read;
		prev = EPOLLIN;
	}

	/* ����active��־�ж��Ƿ�Ϊ��Ծ�¼�,�Ծ������޸Ļ�������¼� */
	if (e->active) { 
		op = EPOLL_CTL_MOD;
		events |= prev;

	} else {
		op = EPOLL_CTL_ADD;
	}

	/* flagsΪ������ʽ,��Ե��������ˮƽ���� */
	ee.events = events | (uint32_t) flags;
	ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

	gvfs_log(LOG_DEBUG, "epoll add event: fd:%d op:%d ev:%08X",
						c->fd, op, ee.events);

	if (-1 == epoll_ctl(ep, op, c->fd, &ee)) {
		err = errno;
		gvfs_log(LOG_ERROR, "epoll_ctl(\"%s\", %d) failed, error: %s",
							op == EPOLL_CTL_MOD ? "MOD" : "ADD",
							c->fd, gvfs_strerror(err));
							
		return GVFS_ERROR;
	}

	ev->active = 1;
	
	return GVFS_OK;
}


gvfs_int_t
gvfs_epoll_del_event(gvfs_event_t *ev, gvfs_int_t event,
	gvfs_uint_t flags)
{
	int 				 op;
	gvfs_err_t           err;
	uint32_t			 prev;
	gvfs_event_t 		*e;
	gvfs_connection_t	*c;
	struct epoll_event	 ee;

	if (flags & GVFS_CLOSE_EVENT) {
		ev->active = 0;
		return GVFS_OK;
	}

	c = ev->data;

	if (event == GVFS_READ_EVENT) {
		e = c->write;
		prev = EPOLLOUT;

	} else {
		e = c->read;
		prev = EPOLLIN;
	}

	if (e->active) {
		op = EPOLL_CTL_MOD;
		ee.events = prev | (uint32_t) flags;
		ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

	} else {
		op = EPOLL_CTL_DEL;
		ee.events = 0;
		ee.data.ptr = NULL;
	}

	gvfs_log(LOG_DEBUG, "epoll del event: fd:%d op:%d ev:%08X",
				   		c->fd, op, ee.events);

	if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
		err = errno;
		
		gvfs_log(LOG_ERROR, "epoll_ctl(\"%s\", %d) failed, error: %s",
							(op == EPOLL_CTL_MOD) ? "MOD" : "DEL",
							c->fd, gvfs_strerror(err));
							
		return GVFS_ERROR;
	}

	ev->active = 0;

	return GVFS_OK;
}


/*
 * �����������,��add_event�����������������ֱ��
 * ��������¼��Ŀɶ� ��д �����Ա�Ե�����ķ�ʽ���
 */
gvfs_int_t
gvfs_epoll_add_connection(gvfs_connection_t *c)
{
	gvfs_err_t          err;
    struct epoll_event  ee;

    ee.events = EPOLLIN|EPOLLOUT|EPOLLET;
    ee.data.ptr = (void *) ((uintptr_t) c | c->read->instance);

    gvfs_log(LOG_DEBUG, "epoll add connection: fd:%d ev:%08X", c->fd, ee.events);

    if (epoll_ctl(ep, EPOLL_CTL_ADD, c->fd, &ee) == -1) {
    	err = errno;
    	
        gvfs_log(LOG_ERROR, "epoll_ctl(EPOLL_CTL_ADD, %d) failed, error: %s",
        					c->fd, gvfs_strerror(err));
        					
        return GVFS_ERROR;
    }

    c->read->active = 1;
    c->write->active = 1;

    return GVFS_OK;
}

gvfs_int_t
gvfs_epoll_del_connection(gvfs_connection_t *c, gvfs_uint_t flags)
{
    int                 op;
    gvfs_err_t          err;
    struct epoll_event  ee;

    if (flags & GVFS_CLOSE_EVENT) {
        c->read->active = 0;
        c->write->active = 0;
        return GVFS_OK;
    }

    gvfs_log(LOG_DEBUG, "epoll del connection: fd:%d", c->fd);

    op = EPOLL_CTL_DEL;
    ee.events = 0;
    ee.data.ptr = NULL;

    if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
    	err = errno;
    	
        gvfs_log(LOG_ERROR, "epoll_ctl(EPOLL_CTL_DEL, %d) failed, error: %s",
        					c->fd, gvfs_strerror(err));
        					
        return GVFS_ERROR;
    }

    c->read->active = 0;
    c->write->active = 0;

    return GVFS_OK;
}


gvfs_int_t gvfs_epoll_process_events(gvfs_cycle_t *cycle,
	gvfs_uint_t timer, gvfs_uint_t flags)
{
	INT32                  err;
	UINT32                 level;
	INT32 			       events;
	uint32_t		       revents;
	gvfs_int_t		       instance, i;
	gvfs_event_t 	      *rev, *wev;
	gvfs_connection_t     *c;
	struct gvfs_list_head *events_list;

	events = epoll_wait(ep, event_list, (int) nevents, timer);

	err = (events == -1) ? errno : 0;

	if (gvfs_event_timer_alarm) {
		gvfs_time_update();
	}

	if (err) {
		if (err == EINTR) {
		
            if (gvfs_event_timer_alarm) { /* epoll_wait()�Ǳ�SIGALRM�ź��ж� */
                gvfs_event_timer_alarm = 0;
                return GVFS_OK;
            }

            level = LOG_INFO;
        } else {
        	level = LOG_WARN;
        }

		gvfs_log(level, "epoll_wait() failed, error:%s", gvfs_strerror(err));
							
		return GVFS_ERROR;
	}

	if (events == 0) {
		if (-1 != timer) {  /* �����˳�ʱʱ��,�����¼���Ϊ0�������� */
			return GVFS_OK;
		}

		/* ������������0��ʾ�Ѿ����� */
		gvfs_log(LOG_ERROR, "%s",
							"epoll_wait() returned no events without timeout");
							
		return GVFS_ERROR;
	}

	
	for (i = 0; i < events; i++) {
		c = event_list[i].data.ptr;

		instance = (uintptr_t) c & 1;
		c = (gvfs_connection_t *) ((uintptr_t) c & (uintptr_t) ~1);

		rev = c->read;

		if (c->fd == -1 || rev->instance != instance) {
			continue;
		}

		revents = event_list[i].events;

		if (revents & (EPOLLERR|EPOLLHUP)) {
			gvfs_log(LOG_ERROR, "epoll_wait() error on fd:%d ev:%04X",
								c->fd, revents);
		}

		if ((revents & (EPOLLERR|EPOLLHUP))
			 && (revents & (EPOLLIN|EPOLLOUT)) == 0)
		{
			revents |= EPOLLIN|EPOLLOUT;
		}
		
		if ((revents & EPOLLIN) && rev->active) {
		
			rev->ready = 1;
			
			if (flags & GVFS_POST_EVENTS) {
				events_list = rev->accept ?
							 &gvfs_posted_accept_events : &gvfs_posted_events;

				gvfs_locked_post_event(rev, events_list);
			} else {
				rev->handler(rev);
			}
		}
		
		wev = c->write;
		
		if ((revents & EPOLLOUT) && wev->active) {
			if (c->fd == -1 || wev->instance != instance) {
				continue;
			}
			
			wev->ready = 1;
			
            if (flags & GVFS_POST_EVENTS) {
                gvfs_locked_post_event(wev, &gvfs_posted_events);

            } else {
				wev->handler(wev);
			}
			
		}
	}

	return GVFS_OK;
}
