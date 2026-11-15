#include "Factory.h"
#include "Product.h"
#include "FactoryA.h"
#include "FactoryB.h"
#include <memory>
using namespace std;
int main(int argc, char * argv[]){
    cout << "hello,world" << endl;
    //生产产品A
    unique_ptr<Factory> factoryA(new FactoryA());
    unique_ptr<Product> productA(factoryA->createProduct());
    productA->show();

    //生产产品B
    unique_ptr<Factory> factoryB(new FactoryB());
    unique_ptr<Product> productB(factoryB->createProduct());
    productB->show();
    return 0;
}
