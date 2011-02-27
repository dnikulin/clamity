// Copyright 2010-2011 Dmitri Nikulin, Enzo Reyes.
//
// This file is part of clamity.
//
// clamity is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// clamity is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with clamity.  If not, see <http://www.gnu.org/licenses/>.

__kernel void testShiftCL(__global uint *memory, uint size) {
    memory[get_global_id(0)] <<= size;
}

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
