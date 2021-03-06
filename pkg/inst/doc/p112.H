\subsection{A Box Jenkins Example}
Example 4.1 from page 112 illustrates the Box-Jenkins
approach based on autocorrelations.  Here the data series is log changes
of seasonally adjusted real US GNP from 1947 to 1988,
available by simple transformations of the data in object "gnp1996".
The data is prepared by selecting quarterly date from as shown, then computing the log of differences.
\begin{Scode}{}
data(gnp1996,package = "RcompHam94")
selection <- subset( gnp1996, Quarter >= "1947-01-01" & Quarter <= "1988-10-01")
y <- diff(log(selection$GNPH))
\end{Scode}
Page 110 shows how to compute sample autocorrelations - we will generate the first 20 to be used in plotting the results below.
\begin{Scode}{}
max.lags <- 20
T <- length(y)
threshold <- 2 / sqrt(T)
gammas <- vector(mode="numeric",length=max.lags + 1)
gammas[[1]] <- 1/T*t(y - mean(y)) %*% (y - mean(y))
for ( j in 1:max.lags )

  gammas[j+1] <- 1/T*t((y - mean(y))[(j+1):T]) %*% (y - mean(y))[1:(T-j)]   # [4.8.6]
rhos <- gammas / gammas[[1]]
\end{Scode}
Page 111 shows how to compute sample partial autocorrelations.  
\begin{Scode}{}
subscripts <- outer( seq(1,max.lags) , seq(1,max.lags),

              function(i,j) { abs(i-j) } )
GAMMA <- array( gammas[ as.vector(subscripts) + 1 ], c(max.lags,max.lags) )
alphas <- vector(mode="numeric",length=max.lags)
for ( m in 1:max.lags )
  alphas[m] <- solve(GAMMA[1:m,1:m],gammas[2:(m+1)]) [[m]]
\end{Scode}
A plot of the outputs reproducing figure 4.2 is shown below.  The source code is provided in the demo.
\begin{center}
\begin{Scode}{fig=TRUE, echo=FALSE}
screens <- split.screen( figs=c(2,1) )
screen( n = screens[[1]], new = TRUE )
par( mar=c(4,2,1,2), cex=.75)
plot( data.frame(i=seq(0,max.lags),rhos=rhos ), type = "h", xlab="Figure 4.2(a) Sample autocorrelations", ylab="",lwd=5,lend=1)
lines( c(0, max.lags), c( threshold, threshold ) )
lines( c(0, max.lags), -c( threshold, threshold ) )
lines( c(0, max.lags), c( 0, 0 ) )
screen( n = screens[[2]], new = TRUE )
par( mar=c(4,2,1,2), cex=.75)
plot( data.frame(i=seq(0,max.lags),alphas=c(1,alphas)), type = "h", xlab="Figure 4.2(b) Sample partial autocorrelations", ylab="",lwd=5,lend=1)
lines( c(0, max.lags), c( threshold, threshold ) )
lines( c(0, max.lags), -c( threshold, threshold ) )
lines( c(0, max.lags), c( 0, 0 ) )
close.screen(all=TRUE)
\end{Scode}
\end{center}
\subsection{R Facilities for Sample Autocorrelations}
Function acf from R package "stats" performs the same function as acf, as we can readily
confirm.
\begin{Scode}{}
acf.correlation <- acf(y, lag.max = max.lags, type = "correlation", plot = FALSE, demean = TRUE)
print( as.vector(acf.correlation$acf) )
print( rhos )
acf.partial <- acf(y, lag.max = max.lags, type = "partial", plot = FALSE, demean = TRUE)
print( as.vector(acf.partial$acf))
print( alphas )
\end{Scode}

