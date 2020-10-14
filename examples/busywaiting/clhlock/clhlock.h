#ifndef CLHLOCK_H
#define CLHLOCK_H

#include "../busywaiting.h"

struct lock;
struct lock_thread;

/*@

predicate lock(struct lock *lock; list<int> level, predicate() inv);
predicate lock_thread(struct lock_thread *thread);
predicate locked(struct lock_thread *thread, struct lock *lock, list<int> level, predicate() inv, real frac, pair<void *, list<int> > ob);

@*/

struct lock *create_lock();
    //@ requires exists<list<int> >(?level) &*& exists<predicate()>(?inv) &*& inv();
    //@ ensures lock(result, level, inv);
    //@ terminates;

struct lock_thread *create_lock_thread();
    //@ requires true;
    //@ ensures lock_thread(result);
    //@ terminates;

void acquire(struct lock_thread *thread, struct lock *lock);
    //@ requires obs(?p, ?obs) &*& lock_thread(thread) &*& [?frac]lock(lock, ?level, ?inv) &*& forall(map(snd, obs), (all_sublevels_lt)(level)) == true;
    //@ ensures locked(thread, lock, level, inv, frac, ?ob) &*& inv() &*& obs(?p1, cons(ob, obs)) &*& is_ancestor_of(p, p1) == true &*& level_lt(level, level_of(ob)) == true;
    //@ terminates;

void release(struct lock_thread *thread);
    //@ requires locked(thread, ?lock, ?level, ?inv, ?frac, ?ob) &*& inv() &*& obs(?p, ?obs);
    //@ ensures obs(?p1, remove(ob, obs)) &*& lock_thread(thread) &*& [frac]lock(lock, level, inv) &*& is_ancestor_of(p, p1) == true;
    //@ terminates;

void dispose_lock_thread(struct lock_thread *thread);
    //@ requires lock_thread(thread);
    //@ ensures true;
    //@ terminates;

void dispose_lock(struct lock *lock);
    //@ requires lock(lock, ?level, ?inv);
    //@ ensures inv();
    //@ terminates;

#endif