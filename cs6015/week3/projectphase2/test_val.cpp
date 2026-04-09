#include "catch.h" 
#include "val.h"
#include "pointer.h"

TEST_CASE("NumVal basic") {
    PTR(Val) v1 = NEW(NumVal)(5);
    PTR(Val) v2 = NEW(NumVal)(5);
    PTR(Val) v3 = NEW(NumVal)(6);

    CHECK(v1->equals(v2));
    CHECK_FALSE(v1->equals(v3));

    CHECK_FALSE(v1->equals(nullptr));

    PTR(Val) sum = v1->add_to(NEW(NumVal)(7));
    CHECK(sum->equals(NEW(NumVal)(12)));

    PTR(Val) mult = v1->mult_with(NEW(NumVal)(5));
    CHECK(mult->equals(NEW(NumVal)(25)));

    CHECK(v1->to_string() == "5");
}

