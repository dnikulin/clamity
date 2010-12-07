__kernel void BasicADD ( __global  float * accumulator,
                          __global  float * varB,
                          __global  float * varC
                        ) {

    unsigned int id = get_global_id(0);
    accumulator[id] = varB[id] + varC[id];

}



__kernel void BasicMUL ( __global  float * accumulator,
                          __global  float * varB,
                          __global  float * varC
                        ) {

    unsigned int id = get_global_id(0);
    accumulator[id] = varB[id] * varC[id];

}
