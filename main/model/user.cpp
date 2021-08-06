#include "user.h"

QX_REGISTER_CPP_QX_API(User)

namespace qx{

    template <> void register_class(QxClass<User> &t){

        t.id(&User::id, "id");
        t.data(&User::age, "age");
        t.data(&User::name, "name");
        t.data(&User::capacity, "capacity");
    }
}
