#ifndef POINT_H
#define POINT_H


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

