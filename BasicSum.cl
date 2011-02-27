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

__kernel void BasicFMAD ( __global  float * accumulator,
                          __global  float * varB,
                          __global  float * varC
                        ) {

    unsigned int id = get_global_id(0);
    accumulator[id] = accumulator[id] + (varB[id] * varC[id]);

}

__kernel void testALU ( __global  float * accumulator,
                        __global  float * varB,
                        __global  float * varC
                        ) {
   unsigned int counter = 0;
   unsigned int maxLoop = 65535;
   for(counter = 0; counter < maxLoop; counter++)
        BasicFMAD(accumulator,varB, varC);
}