#include "gpib/ib.h"
#include "gpib/gpib_user.h"
#include "unistd.h"
#include <cstring>
#include "motor.h"

#include <iostream>

using namespace  std;

int Motor::InitGPIB()
{
    int boardIndex=0;
    int primaryAddress = 4;
    int secondaryAddress=0;
    int gpibTimeout = T3s;
    int EOImode = 1;
    int EOSmode = 0;
    userDescriptor = ibdev(boardIndex,
    primaryAddress,
    secondaryAddress,
    gpibTimeout,
    EOImode,
    EOSmode);
    if (userDescriptor<0) return -1;
    return 0;
}

int Motor::SendStringGPIB(char *x)
{
    ibwrt(userDescriptor,x,strlen(x));
    if (ibsta & ERR)
    {
        printf("Send string error. String is %s. ibsta=0x%x\n",x,ibsta);
        return -1;
    }
    return 0;
}


Motor::Motor(QObject *parent) : QObject(parent)
{

}


void Motor::perform_motion()
{
    int status;
    status = InitGPIB();
    if (status < 0)
    {
        printf("GPIB detection error\n");
        sleep(2);
        emit GPIB_err_connection();
        std::cout << "No connection" << std::endl; //sta roba non va
    } else{
        emit GPIBconnected();
        std::cout << "Connection!" << std::endl;
    }

}
