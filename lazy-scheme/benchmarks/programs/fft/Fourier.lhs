> module Fourier

>  (fft, fftinv)

> where

> import Data.Complex--1.3
> import Data.List(transpose)--1.3
> import Complex_Vectors
> import Debug.Trace
                
> fft:: [ComplexF] -> [ComplexF] -- Warning: works only for n=2^km
>                                -- time=O(n log(n)) algorithm
> fft xs = map((1/(fromInt n))*)(ffth xs us)   where
>   us = map conjugate (rootsOfUnity n)
>   n = length xs
>   fromInt = fromInteger . toInteger -- partain addition

> fftinv:: [ComplexF] -> [ComplexF] -- Warning: works only for n=2^km
>                                   -- time=O(n log(n)) algorithm
> fftinv xs = ffth xs us   where
>   us = rootsOfUnity n
>   n = length xs

> ffth:: [ComplexF] -> [ComplexF] -> [ComplexF]
> ffth xs us
>  | n>1    =             ( replikate fftEvn) `plus` 
>             (us `times` (replikate fftOdd))
>  | n==1   = xs
>  where
>    fftEvn = ffth (evns xs) uEvns
>    fftOdd = ffth (odds xs) uEvns
>    uEvns = evns us
>    evns = everyNth 2
>    odds = everyNth 2 . tail
>    n = length xs


   Utilities

> plus  = zipWith (+)
> times = zipWith (*)
> replikate = cycle
> repl n = concat . take n . repeat
> everyNth n = (map head).(takeWhile (/=[])).(iterate (drop n))

