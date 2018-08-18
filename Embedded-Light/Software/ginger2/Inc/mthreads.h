/*
 * mthreads.h
 *
 *  Created on: Sep 25, 2017
 *      Author: lisa
 */

#ifndef MTHREADS_H_
#define MTHREADS_H_

#ifndef USE_MINI_THREADS
//#if 0
#include <pthread.h>
#define minthread pthread_t
#define minthread_create(A,B,C,D) pthread_create((A),(B),(C),(D))
#define minthread_join(A,B) pthread_join((A),(B))
#define minthread_atcreate(...)
#define minthread_atexit(...)
#define minthread_quantum(...)

#define minmutex pthread_mutex_t
#define minmutex_init(A,B) pthread_mutex_init((A),(B))
#define minmutex_destroy(A) pthread_mutex_destroy(A)
#define minmutex_lock(A) pthread_mutex_lock(A)
#define minmutex_unlock(A) pthread_mutex_unlock(A)

#else

#ifndef EINVAL
#define EINVAL -2
#endif
#ifndef EAGAIN
#define EAGAIN -1
#endif

#ifdef __x86_64__
#define MINTHREAD_NORETURN ((void*)0xffffffffffffffff)
#else
#define MINTHREAD_NORETURN ((void*)0xffffffff)
#endif

#ifdef __cplusplus

struct _minthread {
	void* (*funcall)(void*);
	void* userdata;
	void* retval;
	void* self;
	unsigned rate;
	bool finished;
};

typedef int _minmutex;

void _minthread_at_create(void (*callme)(void*));
void _minthread_at_exit(void (*callme)(void*));
int _minthread_create(_minthread* thread, const void* attr, void* (*start_routine)(void*), void* arg);
int _minthread_join(_minthread &thread, void** retval);
void _minthread_quantum(_minthread &thread);

#define minthread _minthread
#define minthread_create(A,B,C,D) _minthread_create((A),(B),(C),(D))
#define minthread_join(A,B) _minthread_join((A),(B))
#define minthread_atcreate(A) _minthread_at_create(A)
#define minthread_atexit(A) _minthread_at_exit(A)
#define minthread_quantum(A) _minthread_quantum(A)

#define minmutex _minmutex
#define minmutex_init(A,B)
#define minmutex_destroy(A)
#define minmutex_lock(A)
#define minmutex_unlock(A)

#endif
#endif

#endif /* MTHREADS_H_ */
