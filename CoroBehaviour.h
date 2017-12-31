#pragma once

#include "CoroDefinition.h"

#include <boost/coroutine2/all.hpp>
#include <set>

using CoroutinePull = boost::coroutines2::coroutine<class Coroutine*>::pull_type;
using CoroutinePush = boost::coroutines2::coroutine<class Coroutine*>::push_type;
using CoroutineSet = std::set<Coroutine*>;
using Enumerator = std::function<void (CoroutinePush& YieldReturn)>;

class CoroBehaviour
{
public:
    virtual ~CoroBehaviour();
    
private:
    CoroutineSet Coroutines;
    
    void PushYieldReturn(Coroutine* CoroutinePtr);
    
public:
    Coroutine* StartCoroutine(Enumerator&& Enumerator);
    void StopCoroutine(Coroutine* CoroutinePtr);
    void StopAllCoroutines();
    
protected:
    void TickCoroutines(float DeltaTime);
};
