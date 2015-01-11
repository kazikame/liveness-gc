>module Main(main) -- floating point benchmark - Fourier transforms
> where             --     Rex Page (rpage@trc.amoco.com)
> import Fourier    --     Amoco Production Research, Sep 1992
> import Complex_Vectors
> import Data.Complex
> import System.Environment

> main = do 
>   (n:_) <- getArgs
>   let m = read n :: Double
>   putStr
>          ("result1 = " ++ show (result1 m) ++ "\n" )
> main1 m =show (result1 m)

> result1 m =
>         tstfft(rmwC  m)


  Test Apparatus

> tstfft zs  = distance zs (fftinv(fft zs))


> rampWave n = [0 .. n-1]
> rmwC = (map (:+0)).rampWave



  Haskell timings (in seconds)    hbc running on a SparcStation 1+
  rampWave
             n = 256         512          1024
    tstfft         8sec       17sec         41sec   22Sep92
    tstdft        14          36      out of heap   29Sep92
    tstsct        41         174           706      22Sep92
               8.4E6       67.E6         536.E6
  Note:  result should be approximately zero
         in all the above test cases, but isn't in sct cases
         (may indicate a problem with the hbc cosine routine;
          probably for large arguments -- such problems are
          unavoidable in general, but these results seem too
          far out, and they aren't consistent with Fortran
          and Miranda results)


