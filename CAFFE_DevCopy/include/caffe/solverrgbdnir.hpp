#ifndef SOLVERRGBDNIR_HPP
#define SOLVERRGBDNIR_HPP

#include "../../include/caffe/solver.hpp"
#include "../../include/caffe/netrgbdnir.hpp"

namespace caffe {

template <typename Dtype>
class SolverRGBDNIR : public Solver<Dtype>
{
public:
    explicit SolverRGBDNIR(const SolverParameter& param);
    explicit SolverRGBDNIR(const string& param_file);
    void Init(const SolverParameter& param);
    void InitTrainNet();
    void InitTestNets();
    virtual void Solve(const char* resume_file = NULL);
    inline void Solve(const string resume_file) { Solve(resume_file.c_str()); }

protected:
    void Test(const int test_net_id = 0);
};

} //namespace caffe

#endif // SOLVERRGBDNIR_HPP
