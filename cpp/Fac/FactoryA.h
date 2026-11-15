#ifndef _FACTORYA_H
#define _FACTORYA_H


#include "Factory.h"
#include"ProductA.h"
class FactoryA
: public Factory 
{
public: 
    Product * createProduct() override;
};

#endif //_FACTORYA_H
