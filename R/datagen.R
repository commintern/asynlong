library("MASS")
library("reda")
library("extraDistr")

# TODO probably add codes to make sure the number of measurement times >1


# Simulate homogeneous poisson process
simhomoPoipro <- function(rate,endTime){
  num <- rtpois(1,rate*endTime,0)
  return(sort(endTime*runif(num)))
}

# Poisson process with stochastic intensity process is also called Cox process

simCoxpro <- function(cand_meas_times,covariates,gamma,lambda0,lmax){
  nobs <- length(cand_meas_times)
  intensity <- lambda0(cand_meas_times)*exp(gamma%*%covariates)
  # Thinning, drop observation with prob 1- intensity(t)/lmax, i.e. select observations with prob intensity(t)/lmax
  selidx <- runif(nobs) < intensity/lmax
  meas_times <- cand_meas_times[selidx]
  if(is.matrix(covariates)) {
    covres <- covariates[,selidx]
  } else {
    covres <- covariates[selidx]
  }
  return(list(meas_time=meas_times,covariates_meas=covres))
}

simresponse <- function(meas_times,covariates,beta, alpha, mu0,linkfun=log){
  #   generate correlated error terms
  Sigmat_error <- 2 ^ (-abs(outer(meas_times, meas_times, "-")))
  error <- mvrnorm(1, rep(0, length(meas_times)), Sigmat_error)
  # Choose different link functions and H()
  link <- mu0(meas_times)*exp(beta %*% covariates + alpha * (1:length(meas_times) - 1))
  #   log link
  response <- linkfun(link) + error
  return(as.vector(response))
}

simAsyLongdata <-
  function(obscov_rate, 
           lambda0,
           lmax,
           mu0,
           beta,
           alpha,
           gamma,
           cen = 1) {
    p <- length(beta)
    
    # 1.Generate observation times for covariates
    obscov_times <- simhomoPoipro(obscov_rate,cen)
    
    # 2.Generate response measurement times by thinning
    #   Find the maximum of intensity functions
    #if(is.null(lmax)) {
    #  lmax <- -1*optim(0,function(x) -1*lambda0(x),method="L-BFGS-B",lower=0,upper=cen)$value
    #}
    
    nmeas_time <-0
    # Make sure that the number of measurment times >0
    while(nmeas_time<=0){
      #   generate number of candidate measurement times by the max value of lamx
      cand_meas_times <- simhomoPoipro(lmax,cen)
      
      #   combine two batches of times
      comb_times <- sort(union(cand_meas_times, obscov_times))
      
      #   index of measurement times
      meas_idx <- which(comb_times %in% cand_meas_times)
      #   index of measurement times
      obscov_idx <- which(comb_times %in% obscov_times)
      
      # Generate covariates for two numbers of times
      #   Require MASS package
      #   Assume covarites for different beta1i i.i.d.
      #   Covariatce matrix for covariates
      Sigmat_cov <- exp(-abs(outer(comb_times, comb_times, "-")))
      covariates_comb <-
        mvrnorm(p, rep(0, length(comb_times)), Sigmat_cov)
      # By logstic transformation for bounded covariates
      covariates_comb <- pnorm(covariates_comb)
      
      # Seperate covariates matrix(vector)
      if (p > 1) {
        if(length(obscov_idx)==1){
          covariates_obscov <- matrix(covariates_comb[, obscov_idx],nrow=p)
        } else {
          covariates_obscov <- covariates_comb[, obscov_idx]
        }
        
        covariates_cand_meas <- covariates_comb[, meas_idx]
      } else {
        covariates_obscov <- matrix(covariates_comb[obscov_idx],nrow=1)
        covariates_cand_meas <- covariates_comb[meas_idx]
      }
      
      # Generate actual measurement times and corresponding covariates by thinning
      Coxprores <- simCoxpro(cand_meas_times,covariates_cand_meas,gamma,lambda0,lmax)
      
      nmeas_time <-  length(Coxprores$meas_time)
    }
    
    # Measurement time and covariates process 
    meas_times <- Coxprores$meas_time
    covariates_meas <- Coxprores$covariates_meas
    
    # 3.Generate responses Y
    response <- simresponse(meas_times,covariates_meas,beta, alpha, mu0,linkfun=log)
      
      
    # Results
    return(
      list(
        Y = response,
        meas_times = meas_times,
        covariates = covariates_obscov,
        obscov_times = obscov_times,
        censoring = cen
      )
    )
  }


