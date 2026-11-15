#include "FactoryA.h"
/**
 * FactoryA implementation
 */

/**
 * @return Product *
 */
Product * FactoryA::createProduct()
{
    return new ProductA();
}
