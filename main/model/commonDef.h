#ifndef COMMONDEF_H
#define COMMONDEF_H

const int MAXCHECK=400;
const int MAXGRAY=400;
const int MAXDB=40;

struct Point
{
    float x;
    float y;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(x);
        archive & BOOST_SERIALIZATION_NVP(y);
    }

};

#endif // COMMONDEF_H

