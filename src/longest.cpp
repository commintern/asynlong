#include <RcppArmadillo.h>
//[[Rcpp::depends(RcppArmadillo)]]

#include <iostream>


using namespace arma;
using namespace Rcpp;
using namespace std;



/* ===========================================
 * Estimation for longitudinal processes with additive modelgpopo
 * ===========================================
 */

// [[Rcpp::export]]
arma::vec countprofun_C(const arma::vec& counttime, const arma::vec& externalTime){
  arma::uword i,j;
  arma::uword Nc = counttime.size();
  arma::uword Nt = externalTime.size();
  arma::vec res(Nt);
  j = 0;
  i = 0;
  while((i<Nt)&&(j<Nc)){
    if(j>=Nc-1){
      if(counttime(j)<externalTime(i)){
        res(i) = Nc;
      } else {
        res(i) = Nc-1;
      }
      i++;
    } else {
      if(counttime(j)<externalTime(i)){
        if(counttime(j+1)>externalTime(i)){
          res(i) = j+1;
          ++i;
        } else {
          ++j;
        }
      } else {
        res(i) = j;
        ++i;
      }
    }

  }
  return res;
}

// [[Rcpp::export]]
arma::cube Xgen_C(const arma::mat& covMat, const arma::vec& countprocess, const unsigned int& p){
  arma::uword i;
  arma::uword nrow = countprocess.size();
  arma::uword ncol = covMat.n_cols;
  arma::cube res(nrow,ncol,p+1);
  // for(i=0;i<nrow;i++){
  //   res.subcube(i,0,0,i,ncol-1,p-1) = covMat;
  // }
  for(i=0;i<p;i++){
    res.slice(i).each_row() = covMat.row(i);
  }
  res.slice(p).each_col() = countprocess;

  return res;
}



// [[Rcpp::export]]
Rcpp::List longest_c(const arma::rowvec & gamma,
                     Rcpp::ListOf < NumericMatrix > & kerMat,
                     Rcpp::ListOf < NumericVector > & meas_times,
                     Rcpp::ListOf < NumericMatrix > & covariates,
                     Rcpp::ListOf < NumericVector > & response,
                     Rcpp::ListOf < NumericVector > & dlambda,
                     const arma::vec & censor,
                     const unsigned int & n,
                     const unsigned int & p) {
  unsigned int i, l = 0;
  arma::mat temp_kermat;

  arma::rowvec expgammaZ;
  arma::field < arma::rowvec > expgamZ_list(n);
  arma::field < arma::mat > Xbar_list(n);
  arma::field < arma::mat > KerexpgamZ_list(n * n);
  arma::field < arma::cube > Xmat_list(n * n), KerXexpgamZ_list(n * n),  XXtbar_list(n);
  arma::vec temp_meas_time_i, temp_meas_time_l, temp_countprocess_l;
  arma::vec censorind, temp_response;

  unsigned int j, k, Jn, Kn;
  // Rcpp::Rcout <<  "start" <<endl;
  // Calculate Xbar and XXtbar
  // arma::cube Xmat_temp;
  arma::mat temp_cov, temp_KerexpgamZ;
  arma::rowvec tempexpgamZ;
  // arma::mat  XXtbar_num_temp=mat(p+1,p+1,arma::fill::zeros);
  arma::mat temp_kermat_ii;
  // arma::vec  Xbar_num_temp=vec(p+1,arma::fill::zeros);
  arma::vec den_temp;
  //Rcpp::Rcout <<  "00" <<endl;
  arma::vec temp_X = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum_part1 = vec(p + 1, arma::fill::zeros);
  arma::vec temp_thetanum_part1 = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum_part2 = vec(p + 1, arma::fill::zeros);
  arma::mat thetaden = mat(p + 1, p + 1, arma::fill::zeros);
  arma::vec temp_kermat_rowsum;
  // Rcpp::Rcout <<  "11" <<endl;
  // Calculate \hat{theta}
  for (i = 0; i < n; i++) {
    temp_meas_time_i = vec(meas_times[i].begin(), meas_times[i].size(), false);
    temp_response = vec(response[i].begin(), response[i].size(), false);
    Jn = temp_meas_time_i.size();
    Xbar_list(i) = mat(Jn, p + 1, arma::fill::zeros);
    XXtbar_list(i) = cube(p + 1, p + 1, Jn, arma::fill::zeros);
    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    den_temp = vec(Jn, arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    for (l = 0; l < n; l++) {
      temp_meas_time_l =
        vec(meas_times[l].begin(), meas_times[l].size(), false);
      temp_cov = mat(covariates[l].begin(), covariates[l].nrow(),
                     covariates[l].ncol(), false);
      temp_kermat = mat(kerMat[i * n + l].begin(), kerMat[i * n + l].nrow(),
                        kerMat[i * n + l].ncol(), false);
      temp_KerexpgamZ = temp_kermat.each_row() % exp(gamma * temp_cov);
      ///////////////////////////////////// TODO
      censorind = conv_to < arma::vec > ::from(censor[l] > temp_meas_time_i);
      temp_KerexpgamZ.each_col() %= censorind;
      temp_countprocess_l = countprofun_C(temp_meas_time_l, temp_meas_time_i);

      den_temp += sum(temp_KerexpgamZ, 1);

      Kn = temp_cov.n_cols;
      // Rcpp::Rcout <<  "33" <<endl;

      for (j = 0; j < Jn; j++) {
        temp_thetanum_part1.zeros();
        for (k = 0; k < Kn; k++) {
          temp_X.head(p) = temp_cov.col(k);
          temp_X(p) = temp_countprocess_l(j);
          Xbar_list(i).row(j) += temp_KerexpgamZ(j, k) * temp_X.t();
          XXtbar_list(i).slice(j) +=
            temp_KerexpgamZ(j, k) * temp_X * temp_X.t();
          // Calculate the numerator of theta equation part1;
          if (i == l) {
            temp_thetanum_part1 += temp_kermat(j, k) * temp_X;
            // thetanum_part1+= temp_kermat(j,k)*temp_X*temp_response(j);
          }
        }
        if (i == l) {
          thetanum_part1 += temp_thetanum_part1 * temp_response(j);
        }
      }
    }

    // Rcpp::Rcout <<  "44" <<endl;
    Xbar_list(i).each_col() /= den_temp;
    for (j = 0; j < Jn; j++) {
      XXtbar_list(i).slice(j) /= den_temp(j);
    }

    // Rcpp::Rcout <<  "55" <<endl;
    temp_kermat_ii = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                         kerMat[i * n + i].ncol(), false);
    temp_kermat_rowsum = sum(temp_kermat_ii, 1);
    thetanum_part2 +=
      sum(Xbar_list(i).each_col() % (temp_response % temp_kermat_rowsum), 0)
                      .t();
      for (j = 0; j < Jn; j++) {
        thetaden += (XXtbar_list(i).slice(j) -
          Xbar_list(i).row(j).t() * Xbar_list(i).row(j)) *
          temp_kermat_rowsum(j);
      }
      // thetaden += sum(Xbar_list(i).each_col() % response(i) %
      // sum(temp_kermat_ii,1),0).t();
  }

  arma::vec thetanum = thetanum_part1 - thetanum_part2;
  arma::vec thetaest = inv_sympd(thetaden) * thetanum;
  arma::field < arma::vec > gmu0est(n);
  for (i = 0; i < n; i++) {
    temp_response = vec(response[i].begin(), response[i].size(), false);
    gmu0est(i) = temp_response - Xbar_list(i) * thetaest;
  }
  return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
                            Rcpp::Named("gmu0est") = gmu0est,
                            Rcpp::Named("Xbar") = Xbar_list,
                            Rcpp::Named("XXtbar") = XXtbar_list);

  //   return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
  //                             Rcpp::Named("Xbar_list") = Xbar_list,
  //                             Rcpp::Named("temp0") = thetaden,
  //                             Rcpp::Named("temp1") = thetanum  );
}

// ============================ Purturbation ==================================================

// [[Rcpp::export]]
Rcpp::List longest_pur_c(const arma::rowvec & gamma,
                     Rcpp::ListOf < NumericMatrix > & kerMat,
                     Rcpp::ListOf < NumericVector > & meas_times,
                     Rcpp::ListOf < NumericMatrix > & covariates,
                     Rcpp::ListOf < NumericVector > & response,
                     Rcpp::ListOf < NumericVector > & dlambda,
                     const arma::vec & censor,
                     const unsigned int & n,
                     const unsigned int & p, const arma::vec& pur_weights) {
  unsigned int i, l = 0;
  arma::mat temp_kermat;

  arma::rowvec expgammaZ;
  arma::field < arma::rowvec > expgamZ_list(n);
  arma::field < arma::mat > Xbar_list(n);
  arma::field < arma::mat > KerexpgamZ_list(n * n);
  arma::field < arma::cube > Xmat_list(n * n), KerXexpgamZ_list(n * n),  XXtbar_list(n);
  arma::vec temp_meas_time_i, temp_meas_time_l, temp_countprocess_l;
  arma::vec censorind, temp_response;

  unsigned int j, k, Jn, Kn;
  // Rcpp::Rcout <<  "start" <<endl;
  // Calculate Xbar and XXtbar
  // arma::cube Xmat_temp;
  arma::mat temp_cov, temp_KerexpgamZ;
  arma::rowvec tempexpgamZ;
  // arma::mat  XXtbar_num_temp=mat(p+1,p+1,arma::fill::zeros);
  arma::mat temp_kermat_ii;
  // arma::vec  Xbar_num_temp=vec(p+1,arma::fill::zeros);
  arma::vec den_temp;
  //Rcpp::Rcout <<  "00" <<endl;
  arma::vec temp_X = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum_part1 = vec(p + 1, arma::fill::zeros);
  arma::vec temp_thetanum_part1 = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum_part2 = vec(p + 1, arma::fill::zeros);
  arma::mat thetaden = mat(p + 1, p + 1, arma::fill::zeros);
  arma::vec temp_kermat_rowsum;
  // Rcpp::Rcout <<  "11" <<endl;
  // Calculate \hat{theta}
  for (i = 0; i < n; i++) {
    temp_meas_time_i = vec(meas_times[i].begin(), meas_times[i].size(), false);
    temp_response = vec(response[i].begin(), response[i].size(), false);
    Jn = temp_meas_time_i.size();
    Xbar_list(i) = mat(Jn, p + 1, arma::fill::zeros);
    XXtbar_list(i) = cube(p + 1, p + 1, Jn, arma::fill::zeros);
    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    den_temp = vec(Jn, arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    for (l = 0; l < n; l++) {
      temp_meas_time_l =
        vec(meas_times[l].begin(), meas_times[l].size(), false);
      temp_cov = mat(covariates[l].begin(), covariates[l].nrow(),
                     covariates[l].ncol(), false);
      temp_kermat = mat(kerMat[i * n + l].begin(), kerMat[i * n + l].nrow(),
                        kerMat[i * n + l].ncol(), false);
      temp_KerexpgamZ = temp_kermat.each_row() % exp(gamma * temp_cov);
      ///////////////////////////////////// TODO
      censorind = conv_to < arma::vec > ::from(censor[l] > temp_meas_time_i);
      temp_KerexpgamZ.each_col() %= censorind;
      temp_countprocess_l = countprofun_C(temp_meas_time_l, temp_meas_time_i);

      den_temp += sum(temp_KerexpgamZ, 1);

      Kn = temp_cov.n_cols;
      // Rcpp::Rcout <<  "33" <<endl;

      for (j = 0; j < Jn; j++) {
        temp_thetanum_part1.zeros();
        for (k = 0; k < Kn; k++) {
          temp_X.head(p) = temp_cov.col(k);
          temp_X(p) = temp_countprocess_l(j);
          Xbar_list(i).row(j) += temp_KerexpgamZ(j, k) * temp_X.t();
          XXtbar_list(i).slice(j) +=
            temp_KerexpgamZ(j, k) * temp_X * temp_X.t();
          // Calculate the numerator of theta equation part1;
          if (i == l) {
            temp_thetanum_part1 += temp_kermat(j, k) * temp_X;
            // thetanum_part1+= temp_kermat(j,k)*temp_X*temp_response(j);
          }
        }
        if (i == l) {
          thetanum_part1 += pur_weights(i)*temp_thetanum_part1 * temp_response(j);
        }
      }
    }

    // Rcpp::Rcout <<  "44" <<endl;
    Xbar_list(i).each_col() /= den_temp;
    for (j = 0; j < Jn; j++) {
      XXtbar_list(i).slice(j) /= den_temp(j);
    }

    // Rcpp::Rcout <<  "55" <<endl;
    temp_kermat_ii = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                         kerMat[i * n + i].ncol(), false);
    temp_kermat_rowsum = sum(temp_kermat_ii, 1);
    thetanum_part2 +=
      pur_weights(i)*sum(Xbar_list(i).each_col() % (temp_response % temp_kermat_rowsum), 0)
                      .t();
      for (j = 0; j < Jn; j++) {
        thetaden += pur_weights(i)*(XXtbar_list(i).slice(j) -
          Xbar_list(i).row(j).t() * Xbar_list(i).row(j)) *
          temp_kermat_rowsum(j);
      }
      // thetaden += sum(Xbar_list(i).each_col() % response(i) %
      // sum(temp_kermat_ii,1),0).t();
  }

  arma::vec thetanum = thetanum_part1 - thetanum_part2;
  arma::vec thetaest = inv_sympd(thetaden) * thetanum;
  arma::field < arma::vec > gmu0est(n);
  for (i = 0; i < n; i++) {
    temp_response = vec(response[i].begin(), response[i].size(), false);
    gmu0est(i) = temp_response - Xbar_list(i) * thetaest;
  }
  return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest);

  //   return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
  //                             Rcpp::Named("Xbar_list") = Xbar_list,
  //                             Rcpp::Named("temp0") = thetaden,
  //                             Rcpp::Named("temp1") = thetanum  );
}


// [[Rcpp::export]]
Rcpp::List longest_Cao_c(Rcpp::ListOf < NumericMatrix > & kerMat,
                         Rcpp::ListOf < NumericVector > & meas_times,
                         Rcpp::ListOf < NumericMatrix > & covariates,
                         Rcpp::ListOf < NumericVector > & response,
                         const arma::vec & censor,
                         const unsigned int & n,
                         const unsigned int & p) {
  unsigned int i = 0;
  arma::mat temp_kermat;

  arma::rowvec expgammaZ;
  arma::field < arma::rowvec > expgamZ_list(n);
  arma::field < arma::mat > Xbar_list(n);
  arma::field < arma::mat > KerexpgamZ_list(n * n);
  arma::field < arma::cube > Xmat_list(n * n), KerXexpgamZ_list(n * n),  XXtbar_list(n);
  arma::vec temp_meas_time_i, temp_meas_time_l, temp_countprocess_l;
  arma::vec censorind, temp_response;

  unsigned int j, k, Jn, Kn;
  // Rcpp::Rcout <<  "start" <<endl;
  // Calculate Xbar and XXtbar
  // arma::cube Xmat_temp;
  arma::mat temp_cov;

  //Rcpp::Rcout <<  "00" <<endl;
  arma::vec temp_X = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum = vec(p + 1, arma::fill::zeros);
  arma::mat thetaden = mat(p + 1, p + 1, arma::fill::zeros);

  arma::mat Bmat = mat(p + 1, p + 1, arma::fill::zeros);
  arma::vec tempB = vec(p + 1, arma::fill::zeros);

  double sumK, sumKY;
  arma::vec sumKX = vec(p + 1, arma::fill::zeros);
  arma::vec sumKXY = vec(p + 1, arma::fill::zeros);
  arma::mat sumKXXt = mat(p + 1, p + 1, arma::fill::zeros);
  // Rcpp::Rcout <<  "11" <<endl;
  // Calculate \hat{theta}
  sumK = 0;
  sumKY = 0;
  // Get \hat{theta}
  //Rcpp::Rcout <<  "01" <<endl;
  for (i = 0; i < n; i++) {
    //Rcpp::Rcout <<  "02" <<endl;
    //Rcpp::Rcout <<  i  <<endl;
    temp_response = vec(response[i].begin(), response[i].size(), false);
    //Rcpp::Rcout <<  "temp_response" << temp_response << endl;

    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    temp_cov = mat(covariates[i].begin(), covariates[i].nrow(),
                   covariates[i].ncol(), false);
    temp_kermat = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                      kerMat[i * n + i].ncol(), false);
    sumK += accu(temp_kermat);
    //Rcpp::Rcout <<  temp_kermat <<endl;
    Jn = temp_kermat.n_rows;

    Kn = temp_cov.n_cols;

      for (j = 0; j < Jn; j++) {
        for (k = 0; k < Kn; k++) {
          //Rcpp::Rcout <<  "inner" <<endl;
          //Rcpp::Rcout <<  i << " "<< j << " " << k  <<endl;
          temp_X.head(p) = temp_cov.col(k);
          temp_X(p) = j;

          sumKX += temp_kermat(j, k) * temp_X;

          sumKXXt += temp_kermat(j, k) * temp_X * temp_X.t();

          sumKXY += temp_kermat(j, k) * temp_X * temp_response(j);
          sumKY += temp_kermat(j, k) * temp_response(j);
        }
      }
    }

  //Rcpp::Rcout <<  "sumKX" << sumKX << endl;
  //Rcpp::Rcout <<  "sumKXXt" << sumKXXt << endl;
  ////Rcpp::Rcout <<  "sumKXY" << sumKXY << endl;
  //Rcpp::Rcout <<  "sumKY" << sumKY << endl;

  thetanum = sumKXY-sumKX*sumKY / sumK;
  thetaden = sumKXXt-sumKX * sumKX.t() / sumK;
  //Rcpp::Rcout <<  "thetaden" << thetaden << endl;
  arma::vec thetaest = inv_sympd(thetaden) * thetanum;

  // Get Bmat
  //Rcpp::Rcout <<  "03" <<endl;
  for (i = 0; i < n; i++) {
    temp_response = vec(response[i].begin(), response[i].size(), false);

    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    temp_cov = mat(covariates[i].begin(), covariates[i].nrow(),
                   covariates[i].ncol(), false);
    temp_kermat = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                      kerMat[i * n + i].ncol(), false);
    sumK += accu(temp_kermat);
    Jn = temp_cov.n_rows;
    Kn = temp_cov.n_cols;

    tempB = vec(p + 1, arma::fill::zeros);

    for (j = 0; j < Jn; j++) {
      for (k = 0; k < Kn; k++) {
        temp_X.head(p) = temp_cov.col(k);
        temp_X(p) = j;
        tempB = tempB + temp_kermat(j, k) * temp_X * (temp_response(j) - sumKY / sumK - as_scalar(thetaest.t() * (temp_X - sumKX / sumK) ));


      }
    }
    Bmat += tempB * tempB.t();
  }


  return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
                            Rcpp::Named("Vartheta") = inv_sympd(thetaden) * Bmat * inv_sympd(thetaden));
}


// [[Rcpp::export]]
Rcpp::List longest_Cao_full_c(Rcpp::ListOf < NumericMatrix > & kerMat,
                         Rcpp::ListOf < NumericVector > & meas_times,
                         Rcpp::ListOf < NumericMatrix > & covariates,
                         Rcpp::ListOf < NumericVector > & response,
                         const arma::vec & censor,
                         const unsigned int & n,
                         const unsigned int & p) {
  unsigned int i = 0;
  arma::mat temp_kermat;

  double b0;

  arma::rowvec expgammaZ;
  arma::field < arma::rowvec > expgamZ_list(n);
  //arma::field < arma::mat > Xbar_list(n);
  //arma::field < arma::mat > KerexpgamZ_list(n * n);
  //arma::field < arma::cube > Xmat_list(n * n), KerXexpgamZ_list(n * n),  XXtbar_list(n);
  arma::vec temp_meas_time_i, temp_meas_time_l, temp_countprocess_l;
  arma::vec censorind, temp_response;

  unsigned int j, k, Jn, Kn;
  // Rcpp::Rcout <<  "start" <<endl;
  // Calculate Xbar and XXtbar
  // arma::cube Xmat_temp;
  arma::mat temp_cov;

  //Rcpp::Rcout <<  "00" <<endl;
  arma::vec temp_X = vec(p + 1, arma::fill::zeros);
  arma::vec thetanum = vec(p + 1, arma::fill::zeros);
  arma::mat thetaden = mat(p + 1, p + 1, arma::fill::zeros);

  arma::mat Bmat = mat(p + 1, p + 1, arma::fill::zeros);
  arma::vec tempB = vec(p + 1, arma::fill::zeros);
  arma::vec tempC = vec(p + 1, arma::fill::zeros);

  double sumK, sumKY;
  arma::vec sumKX = vec(p + 1, arma::fill::zeros);
  arma::vec sumKXY = vec(p + 1, arma::fill::zeros);
  arma::mat sumKXXt = mat(p + 1, p + 1, arma::fill::zeros);
  // Rcpp::Rcout <<  "11" <<endl;
  // Calculate \hat{theta}
  sumK = 0;
  sumKY = 0;
  // Get \hat{theta}
  //Rcpp::Rcout <<  "01" <<endl;
  for (i = 0; i < n; i++) {
    //Rcpp::Rcout <<  "02" <<endl;
    //Rcpp::Rcout <<  i  <<endl;
    temp_response = vec(response[i].begin(), response[i].size(), false);
    //Rcpp::Rcout <<  "temp_response" << temp_response << endl;

    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    temp_cov = mat(covariates[i].begin(), covariates[i].nrow(),
                   covariates[i].ncol(), false);
    temp_kermat = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                      kerMat[i * n + i].ncol(), false);
    //sumK += accu(temp_kermat);
    //Rcpp::Rcout <<  temp_kermat <<endl;
    Jn = temp_kermat.n_rows;

    Kn = temp_cov.n_cols;
    temp_X(0) = 1;

    for (j = 0; j < Jn; j++) {
      for (k = 0; k < Kn; k++) {
        //Rcpp::Rcout <<  "inner" <<endl;
        //Rcpp::Rcout <<  i << " "<< j << " " << k  <<endl;

        temp_X.tail(p) = temp_cov.col(k);
        //temp_X(p+1) = j;

        //sumKX += temp_kermat(j, k) * temp_X;

        sumKXXt += temp_kermat(j, k) * temp_X * temp_X.t();
        sumKXY += temp_kermat(j, k) * temp_X * temp_response(j);
        //sumKY += temp_kermat(j, k) * temp_response(j);
      }
    }
  }

  //Rcpp::Rcout <<  "sumKX" << sumKX << endl;
  //Rcpp::Rcout <<  "sumKXXt" << sumKXXt << endl;
  ////Rcpp::Rcout <<  "sumKXY" << sumKXY << endl;
  //Rcpp::Rcout <<  "sumKY" << sumKY << endl;

  thetanum = sumKXY;
  thetaden = sumKXXt;
  //Rcpp::Rcout <<  "thetaden" << thetaden << endl;
  arma::vec thetaest = inv_sympd(thetaden) * thetanum;


  // Get Bmat
  //Rcpp::Rcout <<  "03" <<endl;
  for (i = 0; i < n; i++) {
    temp_response = vec(response[i].begin(), response[i].size(), false);

    // temp_thetanum_part1 = vec(p+1,arma::fill::zeros);
    // Rcpp::Rcout <<  "22" <<endl;
    temp_cov = mat(covariates[i].begin(), covariates[i].nrow(),
                   covariates[i].ncol(), false);
    temp_kermat = mat(kerMat[i * n + i].begin(), kerMat[i * n + i].nrow(),
                      kerMat[i * n + i].ncol(), false);
    //sumK += accu(temp_kermat);
    Jn = temp_kermat.n_rows;
    Kn = temp_kermat.n_cols;

    tempB = vec(p + 1, arma::fill::zeros);
    temp_X(0) = 1;

    for (j = 0; j < Jn; j++) {
      for (k = 0; k < Kn; k++) {
        temp_X.tail(p) = temp_cov.col(k);
        //temp_X(p+1) = j;
        tempB = tempB + temp_kermat(j, k) * temp_X *
          (temp_response(j) - as_scalar(thetaest.t() * temp_X ));

      }
    }
    Bmat += tempB * tempB.t();

  }
  Rcpp::Rcout << "Bmat" << endl;
  Bmat.raw_print();
  Rcpp::Rcout << "thetaden" << endl;
  thetaden.raw_print();
  Rcpp::Rcout << "thetanum" << endl;
  thetanum.raw_print();
  //Rcpp::Rcout << tempC << endl;


  return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
                            Rcpp::Named("Vartheta") = inv_sympd(thetaden) * Bmat * inv_sympd(thetaden));
}

//==============================================================================================



// Rcpp::List longest_bak_c(const arma::rowvec& gamma,
//                          Rcpp::ListOf<NumericMatrix>& kerMat,
//                          Rcpp::ListOf<NumericVector>& meas_times,
//                          Rcpp::ListOf<NumericMatrix>& covariates,
//                          Rcpp::ListOf<NumericVector>& response,
//                          Rcpp::ListOf<NumericVector>& dlambda,
//                          const arma::vec& censor, const unsigned int& n,
//                          const unsigned int& p) {
//   unsigned int i, l=0;
//   arma::mat temp_kermat,temp,temp_test;
//   arma::mat temp1, temp0, s0sumres, s1sumres;
//   arma::cube temp_cube;
//   arma::mat tempres;
//   double temp_dvalue;
//   arma::dmat temp_cov_i;
//   arma::rowvec expgammaZ;
//   arma::field<arma::rowvec> expgamZ_list(n);
//   arma::field<arma::mat> Xbar_list(n);
//   arma::field<arma::mat> KerexpgamZ_list(n*n);
//   arma::field<arma::cube> Xmat_list(n*n),KerXexpgamZ_list(n*n), XXtbar_list(n);
//   arma::vec temp_meas_time_i,temp_meas_time_l,temp_countprocess_l,temp_vec1,temp_vec2;
//   arma::vec censorind;
//
//   unsigned int j,k,Jn,Kn;
//   //Rcpp::Rcout <<  "start" <<endl;
//   //List res(2);
//   // i is the iterator for R_i
//   for (i = 0; i < n; i++) {
//     temp_meas_time_i = vec(meas_times[i].begin(),meas_times[i].size(),false);
//     temp_cov_i = mat(covariates[i].begin(),covariates[i].nrow(),covariates[i].ncol(),false);
//     // Obtain exp(gamma * Z_i(R_{ik}))
//     // Store them in expgamZ_list
//
//     expgamZ_list(i) = conv_to<arma::rowvec>::from(exp(gamma * temp_cov_i));
//
//     // l is the iterator for T_l
//     for (l = 0; l < n; l++) {
//       // K(T_{lu}-R_{ik})I(C_i>=T_lu)*exp(gamma * Z_i(R_{ik}))
//       // Store them in KerexpgamZ_list
//
//       temp_kermat = mat(kerMat[l * n + i].begin(),kerMat[l * n + i].nrow(),kerMat[l * n + i].ncol(),false);
//       temp_meas_time_l = vec(meas_times[l].begin(),meas_times[l].size(),false);
//       KerexpgamZ_list(l*n + i) = temp_kermat.each_row() % expgamZ_list(i);
//       censorind = conv_to<arma::vec>::from(censor[i] > temp_meas_time_l);
//       KerexpgamZ_list(l*n + i).each_col() %= censorind;
//
//
//       // Genrate N_i(T_{lu})
//       temp_countprocess_l = countprofun_C(temp_meas_time_i, temp_meas_time_l);
//
//
//       // Get X_i(R_(ik),T_(lu))
//       Xmat_list(l*n + i)  = Xgen_C(temp_cov_i,temp_countprocess_l,p);
//
//       // Calculate
//       //K\left(T_{lu}-R_{ik}\right)I\left(C_{i}\ge t\right)X_{i}\left(R_{ik},T_{lu}\right)
//       //  *\exp\left(\gamma^{\prime}Z_{i}\left(R_{ik}\right)\right)
//       KerXexpgamZ_list(l*n+i) = Xmat_list(l*n + i).each_slice() % KerexpgamZ_list(l*n + i);
//     }
//   }
//
//   //Rcpp::Rcout << expgamZ_list <<endl;
//   //Rcpp::Rcout << KerexpgamZ_list <<endl;
//   //Rcpp::Rcout <<  "2step" <<endl;
//
//   // Calculate \bar{X}
//   for(i=0;i<n;i++){
//     temp1 = arma::zeros<arma::mat>(KerexpgamZ_list(i*n+0).n_rows, p+1);
//     temp0 = arma::zeros<arma::mat>(KerexpgamZ_list(i*n+0).n_rows, 1);
//
//     for(l=0;l<n;l++){
//       temp = arma::sum(KerXexpgamZ_list(i*n + l),1);
//       temp1 = temp1 + temp;
//       temp0 = temp0 + arma::sum(KerexpgamZ_list(i*n + l),1);
//     }
//     Xbar_list(i) = temp1.each_col() /temp0;
//     //Xbar_list(i).transform( [](double val) { return (std::isnan(val) ? 0 : val); } );
//   }
//
//   // TODO futher explain
//   // Calculate \bar{XXt}
//   //Rcpp::Rcout <<  "2.5step" <<endl;
//   temp_vec1 = vec(p);
//   temp_vec2 = vec(p);
//   for(i=0;i<n;i++){
//     //temp_cube = arma::cube(p+1,p+1,KerexpgamZ_list(i*n+0).n_rows);
//     Jn = KerexpgamZ_list(i*n +0).n_rows;
//     XXtbar_list(i) = arma::cube(p+1,p+1,Jn);
//     for(j=0;j<Jn;j++){
//       temp1 = arma::zeros<arma::mat>(p+1, p+1);
//       temp_dvalue = 0;
//       for(l=0;l<n;l++){
//         Kn = KerexpgamZ_list(i*n + l).n_cols;
//         for(k=0;k<Kn;k++){
//           temp_vec1 = Xmat_list(i*n+l).tube(j,k);
//           temp_vec2 = KerXexpgamZ_list(i*n + l).tube(j,k);
//           temp1 = temp1 + temp_vec2 * temp_vec1.t();
//         }
//         temp_dvalue = temp_dvalue + arma::accu(KerexpgamZ_list(i*n + l).row(j));
//       }
//       XXtbar_list(i).slice(j) = temp1 / temp_dvalue;
//     }
//     //XXtbar_list(i).transform( [](double val) { return (std::isnan(val) ? 0 : val); } );
//   }
//
//   // TODO fix outerker_Y_C, use outer input, change the type of temp1 and temp0, they are vec
//   // Calculate \bar{Y}
//   // arma::field<arma::vec> Ybar_list(n);
//   // arma::vec temp_response_i,temp_response_l ;
//   // arma::mat temp_kermat_T;
//   // for(i=0;i<n;i++){
//   //   temp_response_i = vec(response[i].begin(),response[i].size(),false);
//   //   temp1 = arma::zeros<arma::mat>(KerexpgamZ_list(i*n+0).n_rows, 1);
//   //   temp0 = arma::zeros<arma::mat>(KerexpgamZ_list(i*n+0).n_rows, 1);
//   //
//   //
//   //   for(l=0;l<n;l++){
//   //     temp_response_l = vec(response[l].begin(),response[l].size(),false);
//   //     temp_kermat_T = outerker_Y_C(temp_response_i,temp_response_l,pow(n,-0.8));
//   //     temp_kermat = mat(kerMat[i * n + l].begin(),kerMat[i * n + l].nrow(),kerMat[i * n + l].ncol(),false);
//   //
//   //     //temp = arma::sum(KerexpgamZ_list(i*n + l),1);
//   //     temp = arma::sum(temp_kermat,1);
//   //     temp_vec1 = temp_kermat_T * temp_response_l;
//   //     //temp_vec1.transform( [](double val) { return (std::isnan(val) ? 0 : val); } );
//   //     temp1 = temp1 + temp % temp_vec1 ;
//   //     temp0 = temp0 + temp % arma::sum(temp_kermat_T,1);
//   //
//   //   }
//   //   Ybar_list(i) = temp1 /temp0;
//   //   for(j=0;j<Ybar_list(i).size();j++){
//   //     if(std::isnan(Ybar_list(i).at(j))){
//   //       Ybar_list(i).at(j) = temp_response_i.at(j);
//   //     }
//   //   }
//   //
//   //Rcpp::Rcout << temp0 <<endl;
//   //Ybar_list(i).transform( [](double val) { return (std::isnan(val) ? 0 : val); } );
//   //}
//
//
//
//   // Calculate theta
//   //Rcpp::Rcout <<  "3step" <<endl;
//
//
//   temp1 = arma::zeros<arma::vec>(p+1);// numerator
//   temp0 = arma::zeros<arma::mat>(p+1,p+1); // denominator
//   arma::vec temp_dlambda,temp_response;
//   // for(i=0;i<n;i++){
//   //
//   //   temp_dlambda = vec(dlambda[i].begin(),dlambda[i].size(),false);
//   //   for(l=0;l<n;l++){
//   //
//   //     Jn = KerexpgamZ_list(i*n + l).n_rows;
//   //     Kn = KerexpgamZ_list(i*n + l).n_cols;
//   //
//   //     for(j=0;j<Jn;j++){
//   //       for(k=0;k<Kn;k++){
//   //         temp_vec1 = KerXexpgamZ_list(i*n+l).tube(j,k);
//   //         temp_vec2 = Xmat_list(i*n+l).tube(j,k);
//   //         temp_vec2 = temp_vec2-Xbar_list(i).row(j).t();
//   //         temp0 = temp0+temp_dlambda(j)*temp_vec1*temp_vec2.t();
//   //       }
//   //     }
//   //   }
//   // }
//   //
//   // temp_test = temp0;
//   //
//   // Rcpp::Rcout << temp0 << endl;
//
//   temp0 = arma::zeros<arma::mat>(p+1,p+1); // denominator
//   for(i=0;i<n;i++){
//     Jn = KerexpgamZ_list(i*n + i).n_rows;
//     Kn = KerexpgamZ_list(i*n + i).n_cols;
//     for(j=0;j<Jn;j++){
//       for(k=0;k<Kn;k++){
//         temp_kermat = mat(kerMat[i * n + i].begin(),kerMat[i * n + i].nrow(),kerMat[i * n + i].ncol(),false);
//         //temp = Xbar_list(i).row(j);
//         temp0 = temp0 + temp_kermat(j,k)*(XXtbar_list(i).slice(j) - Xbar_list(i).row(j).t() * Xbar_list(i).row(j));
//       }
//     }
//   }
//
//   //Rcpp::Rcout << temp0 << endl;
//   //Rcpp::Rcout <<  "4step" <<endl;
//   for(i=0;i<n;i++){
//     Jn = KerexpgamZ_list(i*n + i).n_rows;
//     Kn = KerexpgamZ_list(i*n + i).n_cols;
//     temp_response = vec(response[i].begin(),response[i].size(),false);
//     temp_kermat = mat(kerMat[i * n + i].begin(),kerMat[i * n + i].nrow(),kerMat[i * n + i].ncol(),false);
//     temp_meas_time_i = vec(meas_times[i].begin(),meas_times[i].size(),false);
//     for(j=0;j<Jn;j++){
//       for(k=0;k<Kn;k++){
//         temp_vec2 = Xmat_list(i*n+i).tube(j,k);
//         //temp1 = temp1+(temp_response(j)-Ybar_list(i).at(j))*(temp_vec2-Xbar_list(i).row(j).t()) * temp_kermat.at(j,k) ;
//         //temp1 = temp1+(temp_response(j)*temp_vec2-Ybar_list(i).at(j)*Xbar_list(i).row(j).t()) * temp_kermat.at(j,k) ;
//         temp1 = temp1+temp_response(j)*(temp_vec2-Xbar_list(i).row(j).t()) * temp_kermat.at(j,k);
//       }
//     }
//     //temp1.t().print();
//     //if(temp1.has_nan()) {
//     //  Rcpp::Rcout << i <<  endl;
//     //}
//   }
//   arma::vec  thetaest= inv_sympd(temp0) * temp1;
//   //Rcpp::Rcout << inv_sympd(temp_test) * temp1 << endl;
//   // return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
//   //                           Rcpp::Named("expgamZ_list") =  expgamZ_list,
//   //                           Rcpp::Named("KerexpgamZ_list") = KerexpgamZ_list,
//   //                           Rcpp::Named("Xmat_list") = Xmat_list,
//   //                           Rcpp::Named("KerXexpgamZ_list") = KerXexpgamZ_list,
//   //                           Rcpp::Named("Xbar_list") = Xbar_list,
//   //                           Rcpp::Named("temp0") = temp0,
//   //                           Rcpp::Named("temp1") = temp1
//   //                         );
//   return Rcpp::List::create(Rcpp::Named("thetaest") = thetaest,
//                             Rcpp::Named("Xbar_list") = Xbar_list,
//                             Rcpp::Named("temp0") = temp0,
//                             Rcpp::Named("temp1") = temp1
//
//   );
// }

