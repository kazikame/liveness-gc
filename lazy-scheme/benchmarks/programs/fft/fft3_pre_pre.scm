(define ( or a b)
 (if a a 
 b 
 ))

(define (or a b ) 
(if a a 
b 
) ) 

(define (makeComplex1 lst1 lst2 ) 
(if (null? lst1 ) 
lst1 
(cons (cons (car lst1 ) (car lst2 ) ) (makeComplex1 (cdr lst1 ) (cdr lst2 ) ) ) 
) 
) 

(define (makeComplex2 i n ) 
(if (eq? i n ) 
() 
(let ((num (cons (* 1.0 i ) 0.0 ) ) ) 
(cons num (makeComplex2 (+ i 1 ) n ) ) 
) ) 
) 

(define (subComplex lstcomplex1 lstcomplex2 ) 
(if (null? lstcomplex1 ) 
() 
(if (null? lstcomplex2 ) 
() 
(let ((cmp1 (car lstcomplex1 ) ) ) 
(let ((cmp2 (car lstcomplex2 ) ) ) 
(let ((sub (cons (- (car cmp1 ) (car cmp2 ) ) (- (cdr cmp1 ) (cdr cmp2 ) ) ) ) ) 
(cons sub (subComplex (cdr lstcomplex1 ) (cdr lstcomplex2 ) ) ) 
) ) ) 
) ) 
) 

(define (plusComplex lstcomplex1 lstcomplex2 ) 
(if (null? lstcomplex1 ) 
() 
(let ((cmp1 (car lstcomplex1 ) ) ) 
(let ((cmp2 (car lstcomplex2 ) ) ) 
(let ((sub (cons (+ (car cmp1 ) (car cmp2 ) ) (+ (cdr cmp1 ) (cdr cmp2 ) ) ) ) ) 
(cons sub (plusComplex (cdr lstcomplex1 ) (cdr lstcomplex2 ) ) ) 
) ) 
) ) 
) 

(define (mulComplex xs ys ) 
(if (null? xs ) 
() 
(let ((x (car (car xs ) ) ) ) 
(let ((y (cdr (car xs ) ) ) ) 
(let ((u (car (car ys ) ) ) ) 
(let ((v (cdr (car ys ) ) ) ) 
(let ((r (- (* x u ) (* y v ) ) ) ) 
(let ((i (+ (* x v ) (* y u ) ) ) ) 
(cons (cons r i ) (mulComplex (cdr xs ) (cdr ys ) ) ) ) 
) ) ) ) ) 
) 
) 

(define (rootsOfUnity n ) 
(let ((a (thetas n 0 ) ) ) 
(makeComplex1 (map "cos" a ) (map "sin" a ) ) ) 
) 

(define (thetas n k ) 
(if (eq? k n ) 
() 
(cons (* (/ (* 2 3.141592653589793 ) (* n 1.0 ) ) (* k 1.0 ) ) (thetas n (+ k 1 ) ) ) 
) 
) 

(define (norm lst1 ) 
(let ((a (sq_magnitude lst1 ) ) ) 
(let ((b (sum a ) ) ) 
(sqrt b ) 
) ) 
) 


(define (sq_magnitude lst ) 
(if (null? lst ) 
() 
(let ((cmp (car lst ) ) ) 
(let ((a (+ (* (car cmp ) (car cmp ) ) (* (cdr cmp ) (cdr cmp ) ) ) ) ) 
(cons a (sq_magnitude (cdr lst ) ) ) ) ) ) 
) 

(define (sum lst ) 
(if (null? lst ) 
0 
(+ (car lst ) (sum (cdr lst ) ) ) 
) 
) 

(define (distance lstcomplex1 lstcomplex2 ) 
(let ((a (subComplex lstcomplex1 lstcomplex2 ) ) ) 
(norm a ) ) 
) 

(define (fft xs ) 
(let ((n (length xs ) ) ) 
(let ((us (mapConjugate (rootsOfUnity n ) ) ) ) 
(let ((a (ffth xs us ) ) ) 
(map1 (/ 1.0 n ) a ) 
) ) ) 
) 

(define (fftinv xs ) 
(let ((n (length xs ) ) ) 
(let ((us (rootsOfUnity n ) ) ) 
(ffth xs us ) 
) ) 
) 

(define (ffth xs us ) 
(let ((n (length xs ) ) ) 
(if (eq? 1 n ) 
xs 
(if (> n 1 ) 
(let ((fftOdd (ffth (odds xs ) (evns us ) ) ) ) 
(let ((fftEvn (ffth (evns xs ) (evns us ) ) ) ) 
(let ((b (times us fftOdd ) ) ) 
(plus b fftEvn ) ) 
) ) 
(cons 11 11 ) 
) ) ) 
) 

(define (times xs fftOdd ) 
(let ((odd (replikate fftOdd (length xs ) ) ) ) 
(let ((a (mulComplex xs odd ) ) ) 
a ) 
) ) 

(define (plus xs fftEvn ) 
(let ((evn (replikate fftEvn (length xs ) ) ) ) 
(plusComplex evn xs ) 
) 
) 

(define (evns xs ) 
(everyNth 2 xs ) 
) 

(define (odds xs ) 
(everyNth 2 (cdr xs ) ) 
) 

(define (everyNth n xs ) 
(let ((ys (takeWhile n xs ) ) ) 
(mapHead ys ) 
) 
) 

(define (takeWhile n xs ) 
(cons xs (takeWhile2 n xs ) ) 
) 

(define (takeWhile2 n xs ) 
(let ((len (length xs ) ) ) 
(if (or (eq? n len ) (< len n ) ) 
() 
(let ((ys (drop n xs ) ) ) 
(cons ys (takeWhile2 n ys ) ) 
) ) ) 
) 

(define (main n ) 
(let ((lstcmplx (makeComplex2 0 n ) ) ) 
(distance lstcmplx (fftinv (fft lstcmplx ) ) ) 
) 
) 



(define (divide1 a b n op ) 
(if (< a b ) 
(if (eq? op 0 ) 
n 
a 
) 
(divide1 (- a b ) b (+ 1 n ) op ) 
) 
) 

(define (divide a b ) 
(divide1 a b 0 0 ) 
) 

(define (map tag lst ) 
(if (null? lst ) 
() 
(if (eq? tag "cos" ) 
(cons (cos (car lst ) ) (map tag (cdr lst ) ) ) 
(if (eq? tag "sin" ) 
(cons (sin (car lst ) ) (map tag (cdr lst ) ) ) 
() 
) ) ) 
) 

(define (mapConjugate cmplx ) 
(if (null? cmplx ) 
() 
(let ((a (car cmplx ) ) ) 
(let ((b (cons (car a ) (* (- 0 1 ) (cdr a ) ) ) ) ) 
(cons b (mapConjugate (cdr cmplx ) ) ) 
) ) ) 
) 


(define (map1 n cmplx ) 
(if (null? cmplx ) 
() 
(let ((a (car cmplx ) ) ) 
(let ((b (cons (* n (car a ) ) (* n (cdr a ) ) ) ) ) 
(cons b (map1 n (cdr cmplx ) ) ) 
) ) ) 
) 


(define (mapHead lst ) 
(if (null? lst ) 
() 
(let ((elem (car lst ) ) ) 
(cons (car elem ) (mapHead (cdr lst ) ) ) 
) ) 
) 

(define (length lst ) 
(if (null? lst ) 
0 
(+ 1 (length (cdr lst ) ) ) 
) 
) 

(define (replikate lst n ) 
(let ((len (length lst ) ) ) 
(if (< len n ) 
(cycle lst lst n ) 
(if (eq? len n ) 
lst 
(take lst n ) 
) ) ) 
) 

(define (cycle lst1 lst2 n ) 
(if (eq? 0 n ) 
() 
(if (null? lst1 ) 
(cycle lst2 lst2 n ) 
(cons (car lst1 ) (cycle (cdr lst1 ) lst2 (- n 1 ) ) ) 
) ) 
) 

(define (take lst n ) 
(if (eq? 0 n ) 
() 
(cons (car lst ) (take (cdr lst ) (- n 1 ) ) ) 
) 
) 

(define (drop n lst ) 
(if (eq? 0 n ) 
lst 
(drop (- n 1 ) (cdr lst ) ) 
) 
) 

(main 1100 ) 
