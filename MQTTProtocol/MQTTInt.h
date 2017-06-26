#ifndef _MQTTER_INT_H
#define _MQTTER_INT_H

#include <iostream>
using namespace std;

namespace mqtter {

class MQTTInt
{
private:
    char _data[2];
public:
    MQTTInt(){_data[0] = 0, _data[1] = 0;}
    MQTTInt(const int& data){_data[0] = data & 0xf, _data[1] = data & 0xf0;}
public:
    int operator[](int i)
    {
        if( i > 15 || i < 0 )
        {
            cout << "索引超过最大值" <<endl;
            return (int)(_data[0] & 0x1);
        }
        return (int)(_data[i/8] & (0x1 << (i%8)));
    }
    friend ostream & operator<<(ostream &out, MQTTInt &rhs)
    {
        out << (int)rhs._data[1] << " " << (int)rhs._data[0];
        return out;
    }
};

}//namespace mqtter

#endif // MQTTINT_H
