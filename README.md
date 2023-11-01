# openmp_gpu_offloading

## Example codes for openmp gpu offloading


For clang clang++ compiler
CUDA library need to be installed
sudo apt install clang-tools clang 
sudo apt install libomp-dev

Compilation Command for clang-
clang++ omp1.cpp -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -v -Xopenmp-target -march=sm_86 -o a.out
1. -fopenmp-targets- for setting nvidia as target.
2. -Xopenmp-target- used for passing arguments/flags to the underlying nvptx tool present in cuda library.
    We are passing -march=sm_86 here.
3. -v for displaying all the steps in the compilation process.
4. please use -o a.out for clang as not using it may lead to random names for the compiled binary.



For gcc g++ omp comes prebuild.
sudo apt install gcc-offload-nvptx
sudo apt install gcc-12-offload-nvptx
gcc by default comes with only openmp 4.5 as its feature complete and stable.
openmp 4.5 offloading supports sm_30 sm_35 sm_53 sm_70 sm_75 sm_80 only. For newer sm we might have to move to openmp 5.0.
For using openmp 5.0 we may need to compile gcc from source.

g++ omp1.cpp -fopenmp -fcf-protection=none -foffload=nvptx-none -foffload=-misa=sm_80
-foffload=-misa=sm_80
-misa is machine instruction set architecture.
-fcf-protection=[full|branch|return|none|check]

    Enable code instrumentation of control-flow transfers to increase program security by checking that target addresses of control-flow transfer instructions (such as indirect function call, function return, indirect jump) are valid. This prevents diverting the flow of control to an unexpected target. This is intended to protect against such threats as Return-oriented Programming (ROP), and similarly call/jmp-oriented programming (COP/JOP).
    The value branch tells the compiler to implement checking of validity of control-flow transfer at the point of indirect branch instructions, i.e. call/jmp instructions. The value return implements checking of validity at the point of returning from a function. The value full is an alias for specifying both branch and return. The value none turns off instrumentation.
    The value check is used for the final link with link-time optimization (LTO). An error is issued if LTO object files are compiled with different -fcf-protection values. The value check is ignored at the compile time.
    The macro __CET__ is defined when -fcf-protection is used. The first bit of __CET__ is set to 1 for the value branch and the second bit of __CET__ is set to 1 for the return.
    You can also use the nocf_check attribute to identify which functions and calls should be skipped from instrumentation (see Declaring Attributes of Functions).
    Currently the x86 GNU/Linux target provides an implementation based on Intel Control-flow Enforcement Technology (CET) which works for i686 processor or newer.


For amd gpu (Not Tested)
gcc omp19.c -fopenmp -foffload=nvptx-none
gcc omp20.c -ffast-math -fopenacc -fopt-info-all-omp -fcf-protection=none -foffload=nvptx-none
gcc omp20.c -ffast-math -fopenacc -fopt-info-all-omp -fcf-protection=none -foffload=amdgcn-amdhsa

sudo amdgpu-install --usecase=hiplibsdk,rocm
sudo amdgpu-install --usecase=hiplibsdk,hip
sudo amdgpu-install --uninstall
sudo apt install gcc-12-offload-amdgcn
sudo apt install gcc-offload-amdgcn
gcc omp20.c -fopenmp -fcf-protection=none -foffload=amdgcn-amdhsa


1. omp1.cpp = checking if the gpu is running or not, basically a hello world code.
2. omp2.cpp = Dot product of 2 array in gpu. omp target clause demonstrated.
3. omp3.cpp = openmp teams construct explored. creating multiple teams with threads in multiples of 32 per team seen.
4. omp3_1.cpp = omp teams used in non offloaded cpu specific code.
4. omp4.cpp = distribute construct.
5. omp5.cpp = simd instruction
6. omp5_1.cpp = avg2 example
7. omp6.cpp = map caluse. data movement
8. omp7.cpp = mapping a vector to device array.
9. omp8.cpp =
10. omp9.cpp = nowait taskwait
11. omp10.cpp = if clause
12. omp11.cpp =  memory allocation in device using- 
    data map(from: deviceArray[0:N]) and 
    deviceArray = (int *)omp_target_alloc(N * sizeof(int), omp_get_initial_device());
