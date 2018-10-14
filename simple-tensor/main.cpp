#include <iostream>
#include "Tensor.h"

int main() {
    Tensor<float> t1 = Tensor<float>({5, 5, 5}); // create new tensor with shape 5x5x5
    Tensor<float> t2 = t1(3, 4); // shape 1x5x5 , shares data with t1
    Tensor<float> t3 = t2; // shape 1x5x5 , doesn't share any data with t1 and t2 as copied

    Tensor<float> t4; // empty tensor
    t4 = t1(3); // shape 5x5 , shares data with t1 and t2 , move semantics here
    Tensor<float> t5 = t1(3)(4)(2, 5); // shape 3, shares data with t1 , t2 , t4
    t5[{1}] = 100.0; // t1 [{3 , 4, 3}] , t2 [{0 , 4, 3}] , t4 [{4 , 3}] are 100.0 as well , but t3 [{0 , 4, 3}] is 0.0
    t1[{3, 4, 4}] = -100.f; // t2 [{0 , 4, 4}] , t4 [{4 , 4}] , t5 [{2}] are -100.0 as well , but t3 [{0 , 4, 4}] is 0.0

    Tensor<float> t6 = t1.reshape({1, 1, 125});

    t1[{0, 0, 0}] = -999.f;
    t1[{0, 0, 3}] = -99.f;

    return 0;
}