#include <csetjmp>          // setjmp, longjmp
#include <exception>
#include <iostream>
       
static int _je_n_except = 0; 
static int _je_n_catch = 0;

class JeObject {
public:
    JeObject();
    virtual ~JeObject();

    JeObject* prev;         // previously called element
};

class _JeStackElement {
public:
    _JeStackElement();
    ~_JeStackElement();

    _JeStackElement* prev;  // previous stack element
    JeObject* obj;          // pointer for this object

    std::jmp_buf buf;       // buffer for setjmp/longjmp
};

static _JeStackElement* _je_stack_root = nullptr;

void je_unstack(int code);

// ---------------------------- MACROSES -------------------------------

#define TRY                                     \
    _JeStackElement next_element;               \
    int exc = (int) setjmp(next_element.buf);   \
    _je_n_except = 1;                           \
    _je_n_catch = 0;                            \
    if (exc == 0) {


#define CATCH(err)                              \
    } else if (exc == err) {                    \
        _je_n_catch++;                          \


#define THROW(_exc)                             \
    if (_je_n_except == 0) {                    \
        std::cout << "Another exception is in process / currently out of TRY block. Continuing...\n"; \
    } else {                                    \
        _je_n_except = 0;                       \
        je_unstack(_exc);                       \
    }


#define FINAL                                   \
    } if (_je_n_catch == 0) {                   \
        THROW(exc);                             \
    }

