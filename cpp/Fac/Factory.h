#ifndef _FACTORY_H
#define _FACTORY_H

#include"Product.h"
// 使用前向声明
class Product;

class Factory 
{
public: 
    
virtual Product * createProduct() = 0;
    
};

#endif //_FACTORY_H
