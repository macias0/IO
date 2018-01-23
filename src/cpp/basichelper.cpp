#include "basichelper.h"

BasicHelper::BasicHelper(QObject *parent) : QObject(parent)
{
    // nothing to do
}

void BasicHelper::tryMe(int x)
{
    std::cout << "Received " << x << std::endl;
}
