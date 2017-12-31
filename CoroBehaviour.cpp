#include "CoroBehaviour.h"

class CoroInternal : public Coroutine
{
private:
    CoroPull Pull;
    
public:
    CoroInternal(CoroPull& InPull) : Pull(std::move(InPull)) { }
    
protected:
    virtual void Tick(float DeltaTime) override { Pull(); }
    virtual bool IsDone() override { return !Pull; }
    virtual Coroutine* GetYieldReturn() override { return Pull.get(); }
};

CoroBehaviour::~CoroBehaviour()
{
    StopAllCoroutines();
}

void CoroBehaviour::PushYieldReturn(Coroutine* CoroutinePtr)
{
    Coroutine* YieldReturn = CoroutinePtr->GetYieldReturn();
    if (YieldReturn)
    {
        YieldReturn->Next = CoroutinePtr;
        CoroutinePtr->IsActive = false;
        Coroutines.erase(CoroutinePtr);
        if (YieldReturn->IsActive)
        {
            Coroutines.insert(YieldReturn);
        }
    }
}

Coroutine* CoroBehaviour::StartCoroutine(CoroEnumerator Enumerator)
{
    CoroPull Pull(Enumerator);
    if (Pull)
    {
        Coroutine* CoroutinePtr = new CoroInternal(Pull);
        Coroutines.insert(CoroutinePtr);
        PushYieldReturn(CoroutinePtr);
        return CoroutinePtr;
    }
    return nullptr;
}
    
void CoroBehaviour::StopCoroutine(Coroutine* CoroutinePtr)
{
    Coroutine* Caller = CoroutinePtr->Next;
    if (Caller)
    {
        Coroutines.insert(Caller);
        Caller->IsActive = true;
    }
    
    for (Coroutine* It = CoroutinePtr; It != nullptr; It = It->GetYieldReturn())
    {
        Coroutines.erase(It);
        delete It;
    }
}

void CoroBehaviour::StopAllCoroutines()
{
    for (Coroutine* CoroutinePtr : Coroutines)
    {
        while (CoroutinePtr != nullptr)
        {
            Coroutine* Caller = CoroutinePtr->Next;
            delete CoroutinePtr;
            CoroutinePtr = Caller;
        }
    }
    Coroutines.clear();
}

void CoroBehaviour::TickCoroutines(float DeltaTime)
{
    CoroSet Copies = Coroutines;
    for (Coroutine* It : Copies)
    {
        while (true)
        {
            It->Tick(DeltaTime);
            if (!It->IsDone())
            {
                PushYieldReturn(It);
                break;
            }
            
            Coroutine* Caller = It->Next;
            Coroutines.erase(It);
            delete It;

            if (Caller == nullptr)
            {
                break;
            }
            Coroutines.insert(Caller);
            It = Caller;
            It->IsActive = true;
        }
    }
}
