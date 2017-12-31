#pragma once

#include <functional>

class Coroutine
{
    friend class CoroBehaviour;
    
private:
    Coroutine* Next;
    bool IsActive;

protected:
    Coroutine() : Next(nullptr), IsActive(true) { }
    virtual ~Coroutine() { }
    virtual void Tick(float DeltaTime) { }
    virtual bool IsDone() { return true; }
    virtual Coroutine* GetYieldReturn() { return nullptr; }
};

class WaitForSeconds : public Coroutine
{
private:
    float Seconds;
    
public:
    WaitForSeconds(float InSeconds) : Seconds(InSeconds) { }
    
protected:
    virtual void Tick(float DeltaTime) override { Seconds -= DeltaTime; }
    virtual bool IsDone() override { return Seconds <= 0.0f; }
};

class WaitWhile : public Coroutine
{
private:
    std::function<bool ()> Condition;
    
public:
    WaitWhile(std::function<bool ()> InCondition) : Condition(InCondition) { }
    
protected:
    virtual bool IsDone() override { return Condition(); }
};

class WaitUntil : public Coroutine
{
private:
    std::function<bool ()> Condition;
    
public:
    WaitUntil(std::function<bool ()> InCondition) : Condition(InCondition) { }
    
protected:
    virtual bool IsDone() override { return !Condition(); }
};

