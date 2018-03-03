#include "declare.h"
#include "clusterlist.h"
#include "random.h"
#include "cube.h"

int main()
{
    shared_ptr<cube> C =  shared_ptr<cube>(new cube());
    creat_random_vector(1000,100000);
    C->initialize(filename);
    C->split();
}
