#pragma once

#include "CoroDefinition.h"

#include <boost/coroutine2/all.hpp>
#include <set>

using CoroBoost = boost::coroutines2::coroutine<class Coroutine*>;
using CoroPull = CoroBoost::pull_type;
using CoroPush = CoroBoost::push_type;
using CoroSet = std::set<Coroutine*>;
using CoroEnumerator = std::function<void (CoroPush&)>;

class CoroBehaviour
{
public:
    virtual ~CoroBehaviour();
    
private:
    CoroSet Coroutines;
    
    void PushYieldReturn(Coroutine* CoroutinePtr);
    
public:
    Coroutine* StartCoroutine(CoroEnumerator Enumerator);
    void StopCoroutine(Coroutine* CoroutinePtr);
    void StopAllCoroutines();
    
protected:
    void TickCoroutines(float DeltaTime);
};
