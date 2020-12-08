#ifndef __common_threads_h__
#define __common_threads_h__

#include <pthread.h>
#include <assert.h>
#include <sched.h>


#define Pthread_create(thread, attr, start_routine, arg) assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define Pthread_join(thread, value_ptr)                  assert(pthread_join(thread, value_ptr) == 0);

#define Pthread_mutex_init(m, v)                         assert(pthread_mutex_init(m, v) == 0);
#define Pthread_mutex_lock(m)                            assert(pthread_mutex_lock(m) == 0);
#define Pthread_mutex_unlock(m)                          assert(pthread_mutex_unlock(m) == 0);


#endif // __common_threads_h__
