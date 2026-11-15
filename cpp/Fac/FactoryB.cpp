#ifndef _FACTORYB_H
#define _FACTORYB_H

#include "Factory.h"
#include "ProductB.h"


class FactoryB
:public Factory 
{
public: 
    Product * createProduct() override;
};

#endif //_FACTORYB_H

