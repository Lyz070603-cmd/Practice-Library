#include "FactoryB.h"

/**
 * FactoryB implementation
 */


/**
 * @return Product *
 */
Product * FactoryB::createProduct()
{
    return new ProductB();
}
