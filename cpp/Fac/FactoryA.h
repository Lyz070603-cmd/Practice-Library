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
```

FactoryA.h

```c++
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
