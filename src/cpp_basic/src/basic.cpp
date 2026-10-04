#include <iostream>
#include <memory>
class Child;
class Parent
{
public:
    std::shared_ptr<Child> child;
    ~Parent() { std::cout << "Parent is released" << std::endl; }
};
class Child
{
public:
    // std::shared_ptr<Parent> parent;
    std::weak_ptr<Parent> parent;
    ~Child() { std::cout << "Child is released" << std::endl; }
};

int main()
{
    // 1. unique pointer
    std::cout << "-----------------------" << std::endl;
    std::cout << "unique_ptr" << std::endl;
    std::cout << "-----------------------" << std::endl;
    auto a1 = std::make_unique<int>(1);
    // can't copy - compile error
    // auto a2 = a1;
    auto a2 = std::move(a1);
    // segmentation fault due to access to nullptr for a1
    // std::cout << "a1: " << *a1 << std::endl;
    std::cout << "a2: " << *a2 << std::endl;

    // 2. shared pointer
    std::cout << "-----------------------" << std::endl;
    std::cout << "shared_ptr" << std::endl;
    std::cout << "-----------------------" << std::endl;
    auto b1 = std::make_shared<int>(2);
    auto b2 = b1;
    // use_count
    auto b3 = std::make_shared<int>(3);
    {
        std::shared_ptr<int> b4 = b3;
        std::cout << "b3: use count: " << b3.use_count() << std::endl;
        std::cout << "b4: use count: " << b4.use_count() << std::endl;
        std::cout << "b3: " << *b3 << std::endl;
        std::cout << "b4: " << *b4 << std::endl;
    }
    std::cout << "b3: use count: " << b3.use_count() << std::endl;
    // 3. weak pointer
    std::cout << "-----------------------" << std::endl;
    std::cout << "weak_ptr" << std::endl;
    std::cout << "-----------------------" << std::endl;
    {
        auto parent = std::make_shared<Parent>();
        auto child = std::make_shared<Child>();

        parent->child = child;
        child->parent = parent;

        std::cout << "parent use count: " << parent.use_count() << std::endl;
        std::cout << "child use count: " << child.use_count() << std::endl;
    }

    // 4. lambda, caputure
    std::cout << "-----------------------" << std::endl;
    std::cout << "lambda" << std::endl;
    std::cout << "-----------------------" << std::endl;
    auto add = [](float c1, float c2) -> float
    { return c1 + c2; };
    float r1 = add(1.0, 2.0);
    std::cout << "result1: " << r1 << std::endl;

    float r2 = [](float c1, float c2) -> float
    { return c1 + c2; }(1, 2);
    std::cout << "result2: " << r2 << std::endl;
    // capture
    float offset = 1.0;
    auto add2 = [offset](float c1, float c2) -> float
    { return c1 + c2 + offset; };
    float r3 = add2(1.0, 2.0);
    std::cout << "result3: " << r3 << std::endl;

    // capture - copy at definition time
    int d = 0;
    auto f1 = [d]
    { return d; };
    d = 1;
    int r4 = f1();
    std::cout << "result4: " << r4 << std::endl;

    // capture - reference
    int e = 0;
    auto f2 = [&e]
    { return e; };
    e = 1;
    int r5 = f2();
    std::cout << "result5: " << r5 << std::endl;

    return 0;
}