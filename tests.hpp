#include "jmpexceptions.hpp"

#include <iostream>

#define ERRORTYPE1 1
#define ERRORTYPE2 2
#define CRITICALERROR 3
#define UNCAUGHTERROR 4
#define MATHPROCESSINGERROR 5
#define RUNTIMEERROR 6

class Foo: public JeObject {
public:
    Foo(int i) : val(i) {
        std::cout << "Foo : Constructor (" << val << ")" << std::endl;
    }
    virtual ~Foo() {
        std::cout << "Foo : Destructor  (" << val << ")" << std::endl;
    }
    int val;
};

class Bar : public JeObject {
public:
    Bar(int i) : val(i) {
        std::cout << "Bar : Constructor (" << val << ")" << std::endl;
    }
    virtual ~Bar() {
        std::cout << "Bar : Destructor  (" << val << ")" << std::endl;
    }
    int val;
};


void test1() {
    std::cout << "[test] Destroying objects" << std::endl;
    Foo a(-10);
    TRY {
        Foo b(1);
        Bar c(2);
        THROW(ERRORTYPE1); 
    } CATCH(ERRORTYPE1) {
        std::cout << "caught : ERRORTYPE1\n";
    } FINAL
}

void test2() {
    std::cout << "[test] Destroying objects" << std::endl;
    Foo a(1);
    TRY{
        Bar c(2);
        THROW(ERRORTYPE1);
    } CATCH(ERRORTYPE1) {
        std::cout << "caught : ERRORTYPE1\n";
    } FINAL
}

void test3() {
    std::cout << "[test] Skipping mismatched exceptions" << std::endl;
    Foo a(1);
    TRY{
        Bar c(2);
        THROW(UNCAUGHTERROR);
    } CATCH(ERRORTYPE1) {
        std::cout << "caught : ERRORTYPE1\n";
    } CATCH(UNCAUGHTERROR) {
        std::cout << "caught : UNCAUGHTERROR\n";
    } FINAL
}

void test4() {
    std::cout << "[test] Skipping mismatched exceptions, level control" << std::endl;
    TRY {
        std::cout << "Try level = 1" << std::endl;
        TRY {
            std::cout << ". Try level = 2" << std::endl;
            TRY {
                std::cout << ". . Try level = 3" << std::endl;
                Foo x(1);
                THROW(ERRORTYPE1);
            } CATCH(MATHPROCESSINGERROR) {
                std::cout << ". . caught : MATHPROCESSINGERROR (bad - wrong catch)\n";
            } FINAL
        } CATCH(ERRORTYPE1) {
            std::cout << ". caught : ERRORTYPE1 (good)\n";
        } FINAL
    } CATCH(ERRORTYPE1) {
        std::cout << "caught : ERRORTYPE1 (bad - missed previous catch)\n";
    } FINAL
}

void funcB(int x) {
    std::cout << "funcB(" << x << ")" << std::endl;
    Foo b(x + 10);
    Bar c(x + 15);
    std::cout << "THROW(RUNTIMEERROR)" << std::endl;
    THROW(RUNTIMEERROR);
}

void funcA(int x) {
    std::cout << "funcA(" << x << ")" << std::endl;
    Foo object1(x);
    funcB(x);
    Bar object2(x + 20);
}

void funcC(int x) {
    std::cout << "funcC(" << x << ")" << std::endl;
    Foo object1(x);
    std::cout << "THROW(CRITICALERROR)" << std::endl;
    THROW(CRITICALERROR);
    std::cout << "THROW(UNCAUGHTERROR)" << std::endl;
    THROW(UNCAUGHTERROR);
    funcB(x + 100);
    Bar object2(x + 20);
}

void test5() {
    std::cout << "[test] Over functions" << std::endl;
    TRY {
        funcA(2);
    } CATCH(RUNTIMEERROR) {
        std::cout << "caught : RUNTIMEERROR" << std::endl;
    } FINAL
}

void test6() {
    std::cout << "[test] No errors thrown" << std::endl;
    TRY {
        Foo x(2);
    } CATCH(RUNTIMEERROR) {
        std::cout << "caught : RUNTIMEERROR (bad)" << std::endl;
    } FINAL
}

void test7() {
    std::cout << "[test] Multiple throws, unhandled exception" << std::endl;
    TRY {
        funcC(2);
    } CATCH(RUNTIMEERROR) {
        std::cout << "caught : RUNTIMEERROR" << std::endl;
    } FINAL
}

void print_ln() 
{
    std::cout << "============================================================" << std::endl;
}

void run_tests() {
    print_ln();
    test1();
    print_ln();
    test2();
    print_ln();
    test3();
    print_ln();
    test4();
    print_ln();
    test5();
    print_ln();
    test6();
    print_ln();
    test7();
}
