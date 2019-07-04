//
// Created by tianjiajia on 6/21/19.
//

#ifndef THREADTEST_THREAD_H
#define THREADTEST_THREAD_H

#include <pthread.h>
#include <string.h>

typedef pthread_mutex_t MUTEX;
typedef pthread_cond_t COND;

class Thread {
public: /*methods*/
    Thread();
    ~Thread();

    virtual bool onLoop() = 0;
    int start();
    void join();
    void cancel();
    void cancel_point();

protected:
    void exit();

public:/*class*/

    /*Mutex*/
    class AutoMutex {
    public:
        AutoMutex(MUTEX* _mutex):mutex(_mutex){
            pthread_mutex_lock(mutex);
        }
        ~AutoMutex(){
            pthread_mutex_unlock(mutex);
        }

    private:
        MUTEX *mutex;
    };

private:

    pthread_t mTid = -1;
};

void* routine(void* _this){
    Thread* thread = static_cast<Thread*> (_this);
    while (thread->onLoop());
    return nullptr;
}

Thread::Thread() {

}

Thread::~Thread() {

}

int Thread::start() {
    int ret = pthread_create(&mTid, NULL, routine, (void*) this);
    if (ret) {
        printf("pthread_create failed: %s\n", strerror(ret));
    }

    return ret;
}

void Thread::join() {
    if (mTid != -1) {
        pthread_join(mTid, NULL);
    }
}

void Thread::cancel() {
    pthread_cancel(mTid);
}

void Thread::cancel_point() {
    pthread_testcancel();
}

void Thread::exit() {
    pthread_exit(NULL);
}


#endif //THREADTEST_THREAD_H
