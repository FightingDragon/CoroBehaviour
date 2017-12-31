#include <iostream>
#include <string>

#include "CoroBehaviour.h"

class ExampleBehaviour : public CoroBehaviour
{
public:
    Enumerator TaskA()
    {
        return [=](CoroutinePush& YieldReturn)
        {
            std::cout << "TaskA 1\n";
            YieldReturn(nullptr);
            
            std::cout << "TaskA 2\n";
        };
    }

    Enumerator TaskB()
    {
        return [=](CoroutinePush& YieldReturn)
        {
            std::cout << "TaskB 1\n";
            YieldReturn(new WaitForSeconds(1.0f));
            
            std::cout << "TaskB 2\n";
        };
    }
    
    Enumerator TaskC(std::string Param1, float Param2)
    {
        return [=](CoroutinePush& YieldReturn)
        {
            std::cout << "TaskC 1 " << Param1 << "\n";
            YieldReturn(StartCoroutine(TaskA()));
            
            std::cout << "TaskC 2 " << Param1 << "\n";
            YieldReturn(StartCoroutine(TaskB()));
        };
    }

    Enumerator TaskD(std::string Param1, float Param2)
    {
        return [=](CoroutinePush& YieldReturn)
        {
            std::cout << "TaskD 1 " << Param1 << "\n";
            YieldReturn(new WaitForSeconds(Param2));

            std::cout << "TaskD 2 " << Param1 << "\n";
            YieldReturn(StartCoroutine(TaskB()));

            std::cout << "TaskD 3 " << Param1 << "\n";
            YieldReturn(StartCoroutine(TaskC(Param1, Param2)));
        };
    }
    
    void Test()
    {
        for (int i = 0; i < 100; i++)
        {
            TickCoroutines(0.3f);
        }
    }
};

int main()
{
    ExampleBehaviour foo;
    foo.StartCoroutine(foo.TaskA());
    foo.StartCoroutine(foo.TaskB());
    foo.StartCoroutine(foo.TaskC("first", 1.0f));
    foo.StartCoroutine(foo.TaskD("second", 2.0f));
    foo.Test();
    return 0;
}

