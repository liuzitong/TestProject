#ifndef RX_H
#define RX_H
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

struct Rx
{
    float rx1_r;
    float rx2_r;
    float rx3_r;
    float rx1_l;
    float rx2_l;
    float rx3_l;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(rx1_r);
        archive & BOOST_SERIALIZATION_NVP(rx2_r);
        archive & BOOST_SERIALIZATION_NVP(rx3_r);
        archive & BOOST_SERIALIZATION_NVP(rx1_l);
        archive & BOOST_SERIALIZATION_NVP(rx2_l);
        archive & BOOST_SERIALIZATION_NVP(rx3_l);
    }
};

#endif // RX_H
