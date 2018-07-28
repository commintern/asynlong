// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// countprofun_C
arma::vec countprofun_C(const arma::vec& counttime, const arma::vec& externalTime);
RcppExport SEXP _asynlong_countprofun_C(SEXP counttimeSEXP, SEXP externalTimeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::vec& >::type counttime(counttimeSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type externalTime(externalTimeSEXP);
    rcpp_result_gen = Rcpp::wrap(countprofun_C(counttime, externalTime));
    return rcpp_result_gen;
END_RCPP
}
// Xgen_C
arma::cube Xgen_C(const arma::mat& covMat, const arma::vec& countprocess, const unsigned int& p);
RcppExport SEXP _asynlong_Xgen_C(SEXP covMatSEXP, SEXP countprocessSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type covMat(covMatSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type countprocess(countprocessSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(Xgen_C(covMat, countprocess, p));
    return rcpp_result_gen;
END_RCPP
}
// longest_c
Rcpp::List longest_c(const arma::rowvec& gamma, Rcpp::ListOf < NumericMatrix >& kerMat, Rcpp::ListOf < NumericVector >& meas_times, Rcpp::ListOf < NumericMatrix >& covariates, Rcpp::ListOf < NumericVector >& response, Rcpp::ListOf < NumericVector >& dlambda, const arma::vec& censor, const unsigned int& n, const unsigned int& p);
RcppExport SEXP _asynlong_longest_c(SEXP gammaSEXP, SEXP kerMatSEXP, SEXP meas_timesSEXP, SEXP covariatesSEXP, SEXP responseSEXP, SEXP dlambdaSEXP, SEXP censorSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::rowvec& >::type gamma(gammaSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf < NumericMatrix >& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf < NumericVector >& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf < NumericMatrix >& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf < NumericVector >& >::type response(responseSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf < NumericVector >& >::type dlambda(dlambdaSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type censor(censorSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(longest_c(gamma, kerMat, meas_times, covariates, response, dlambda, censor, n, p));
    return rcpp_result_gen;
END_RCPP
}
// ugamma1_C
arma::vec ugamma1_C(Rcpp::ListOf<NumericMatrix>& kerMat, Rcpp::ListOf<NumericMatrix>& covariates, const unsigned int& n, const int& p);
RcppExport SEXP _asynlong_ugamma1_C(SEXP kerMatSEXP, SEXP covariatesSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(ugamma1_C(kerMat, covariates, n, p));
    return rcpp_result_gen;
END_RCPP
}
// zbar_c
arma::field<arma::mat> zbar_c(const arma::rowvec& gamma, Rcpp::ListOf<NumericMatrix>& kerMat, Rcpp::ListOf<NumericVector>& meas_times, Rcpp::ListOf<NumericMatrix>& covariates, const arma::vec& censor, const unsigned int& n, const unsigned int& p);
RcppExport SEXP _asynlong_zbar_c(SEXP gammaSEXP, SEXP kerMatSEXP, SEXP meas_timesSEXP, SEXP covariatesSEXP, SEXP censorSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::rowvec& >::type gamma(gammaSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type censor(censorSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(zbar_c(gamma, kerMat, meas_times, covariates, censor, n, p));
    return rcpp_result_gen;
END_RCPP
}
// ugamma2_C
arma::vec ugamma2_C(const arma::rowvec& gamma, Rcpp::ListOf<NumericMatrix>& kerMat, Rcpp::ListOf<NumericVector>& meas_times, Rcpp::ListOf<NumericMatrix>& covariates, const arma::vec& censor, const unsigned int& n, const unsigned int& p);
RcppExport SEXP _asynlong_ugamma2_C(SEXP gammaSEXP, SEXP kerMatSEXP, SEXP meas_timesSEXP, SEXP covariatesSEXP, SEXP censorSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::rowvec& >::type gamma(gammaSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type censor(censorSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(ugamma2_C(gamma, kerMat, meas_times, covariates, censor, n, p));
    return rcpp_result_gen;
END_RCPP
}
// ugamma2_test_C
arma::vec ugamma2_test_C(const arma::rowvec& gamma, Rcpp::ListOf<NumericMatrix>& kerMat, Rcpp::ListOf<NumericVector>& meas_times, Rcpp::ListOf<NumericMatrix>& covariates, const arma::vec& censor, const unsigned int& n, const unsigned int& p);
RcppExport SEXP _asynlong_ugamma2_test_C(SEXP gammaSEXP, SEXP kerMatSEXP, SEXP meas_timesSEXP, SEXP covariatesSEXP, SEXP censorSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::rowvec& >::type gamma(gammaSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type censor(censorSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(ugamma2_test_C(gamma, kerMat, meas_times, covariates, censor, n, p));
    return rcpp_result_gen;
END_RCPP
}
// dlambda_C
Rcpp::List dlambda_C(const arma::rowvec& gamma, Rcpp::ListOf<NumericMatrix>& kerMat, Rcpp::ListOf<NumericVector>& meas_times, Rcpp::ListOf<NumericMatrix>& covariates, const arma::vec& censor, const unsigned int& n, const unsigned int& p);
RcppExport SEXP _asynlong_dlambda_C(SEXP gammaSEXP, SEXP kerMatSEXP, SEXP meas_timesSEXP, SEXP covariatesSEXP, SEXP censorSEXP, SEXP nSEXP, SEXP pSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::rowvec& >::type gamma(gammaSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type kerMat(kerMatSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericMatrix>& >::type covariates(covariatesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type censor(censorSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type n(nSEXP);
    Rcpp::traits::input_parameter< const unsigned int& >::type p(pSEXP);
    rcpp_result_gen = Rcpp::wrap(dlambda_C(gamma, kerMat, meas_times, covariates, censor, n, p));
    return rcpp_result_gen;
END_RCPP
}
// optimtest
arma::vec optimtest();
RcppExport SEXP _asynlong_optimtest() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(optimtest());
    return rcpp_result_gen;
END_RCPP
}
// outermin_C
NumericMatrix outermin_C(NumericVector x, NumericVector y);
RcppExport SEXP _asynlong_outermin_C(SEXP xSEXP, SEXP ySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type x(xSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type y(ySEXP);
    rcpp_result_gen = Rcpp::wrap(outermin_C(x, y));
    return rcpp_result_gen;
END_RCPP
}
// epanker_C
double epanker_C(double u, double h);
RcppExport SEXP _asynlong_epanker_C(SEXP uSEXP, SEXP hSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< double >::type u(uSEXP);
    Rcpp::traits::input_parameter< double >::type h(hSEXP);
    rcpp_result_gen = Rcpp::wrap(epanker_C(u, h));
    return rcpp_result_gen;
END_RCPP
}
// outerker_C
NumericMatrix outerker_C(NumericVector x, NumericVector y, double& h);
RcppExport SEXP _asynlong_outerker_C(SEXP xSEXP, SEXP ySEXP, SEXP hSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type x(xSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type y(ySEXP);
    Rcpp::traits::input_parameter< double& >::type h(hSEXP);
    rcpp_result_gen = Rcpp::wrap(outerker_C(x, y, h));
    return rcpp_result_gen;
END_RCPP
}
// kerMatgen_C
Rcpp::List kerMatgen_C(Rcpp::ListOf<NumericVector>& meas_times, Rcpp::ListOf<NumericVector>& obscov_times, double& h);
RcppExport SEXP _asynlong_kerMatgen_C(SEXP meas_timesSEXP, SEXP obscov_timesSEXP, SEXP hSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type meas_times(meas_timesSEXP);
    Rcpp::traits::input_parameter< Rcpp::ListOf<NumericVector>& >::type obscov_times(obscov_timesSEXP);
    Rcpp::traits::input_parameter< double& >::type h(hSEXP);
    rcpp_result_gen = Rcpp::wrap(kerMatgen_C(meas_times, obscov_times, h));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_asynlong_countprofun_C", (DL_FUNC) &_asynlong_countprofun_C, 2},
    {"_asynlong_Xgen_C", (DL_FUNC) &_asynlong_Xgen_C, 3},
    {"_asynlong_longest_c", (DL_FUNC) &_asynlong_longest_c, 9},
    {"_asynlong_ugamma1_C", (DL_FUNC) &_asynlong_ugamma1_C, 4},
    {"_asynlong_zbar_c", (DL_FUNC) &_asynlong_zbar_c, 7},
    {"_asynlong_ugamma2_C", (DL_FUNC) &_asynlong_ugamma2_C, 7},
    {"_asynlong_ugamma2_test_C", (DL_FUNC) &_asynlong_ugamma2_test_C, 7},
    {"_asynlong_dlambda_C", (DL_FUNC) &_asynlong_dlambda_C, 7},
    {"_asynlong_optimtest", (DL_FUNC) &_asynlong_optimtest, 0},
    {"_asynlong_outermin_C", (DL_FUNC) &_asynlong_outermin_C, 2},
    {"_asynlong_epanker_C", (DL_FUNC) &_asynlong_epanker_C, 2},
    {"_asynlong_outerker_C", (DL_FUNC) &_asynlong_outerker_C, 3},
    {"_asynlong_kerMatgen_C", (DL_FUNC) &_asynlong_kerMatgen_C, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_asynlong(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
