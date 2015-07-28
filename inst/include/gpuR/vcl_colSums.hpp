
// eigen headers for handling the R input data
#include <RcppEigen.h>

#include "gpuR/eigen_helpers.hpp"

// Use OpenCL with ViennaCL
#define VIENNACL_WITH_OPENCL 1

// Use ViennaCL algorithms on Eigen objects
#define VIENNACL_WITH_EIGEN 1

// ViennaCL headers
#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/sum.hpp"

using namespace Rcpp;

template <typename T>
inline
void cpp_vienna_colsum(
    MapMat<T> &Am, 
    MapVec<T> &colSums)
{    
    //use only GPUs:
    long id = 0;
    viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
    
    int M = Am.cols();
    int K = Am.rows();
    int V = colSums.size();
    
    viennacl::matrix<T> vcl_A(K,M);
    viennacl::vector<T> vcl_colSums(V);
//    viennacl::vector<T> ones = viennacl::scalar_vector<T>(K, 1);
    
    viennacl::copy(Am, vcl_A); 
    
//    vcl_colSums = viennacl::linalg::prod(trans(vcl_A), ones);
    
    vcl_colSums = viennacl::linalg::column_sum(vcl_A);
    
    viennacl::copy(vcl_colSums, colSums);
}
