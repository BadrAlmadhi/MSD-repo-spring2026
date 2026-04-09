#ifndef __msdscript_pointer__
#define __msdscript_pointer__

#include <memory>

// controls eveything
#define USE_PLAIN_POINTERS 0
#if USE_PLAIN_POINTERS

// user macros shortcuts
# define NEW(T)    new T // NEW(Num) = new Num
# define PTR(T)    T*    // PTR(Num) = Num*
# define CAST(T)   dynamic_cast<T*> // dynamic_cast<Num*>
# define CLASS(T)  class T // update each class to CLASS
# define THIS      this 

#else

# define NEW(T)    std::make_shared<T>
# define PTR(T)    std::shared_ptr<T>
# define CAST(T)   std::dynamic_pointer_cast<T>
# define CLASS(T)  class T : public std::enable_shared_from_this<T>
# define THIS      shared_from_this()

#endif

#endif