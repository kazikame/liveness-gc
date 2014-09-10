;Complex Number structure is : (cons real img)
 

;This will make a list of complex numbers. 
;lst1 has cos terms and lst2 has sin terms.
;Output: (cons (complex no.) (cons (complex no.) ....null))
;makeComplex1::[Double]->[Double]->[Complex No.]
(define (makeComplex1 lst1 lst2)
  (if (null? lst1)
      lst1
      (cons (cons (car lst1) (car lst2)) (makeComplex1 (cdr lst1) (cdr lst2)))
      )
  ) 

;makeComplex2::int->[Complex No.]
(define (makeComplex2 i n)
  (if (eq? i n)
      '()
      (let ((num (cons (* 1.0 i) 0.0) ))
        (cons num (makeComplex2 (+ i 1) n))
        ))
  )

;This will take two list containing complex numbers and subtract the corresponding index one's and return the final list.
;subComplex:: [Complex No] -> [Complex No] -> [Complex no]
(define (subComplex lstcomplex1 lstcomplex2)
  (if (null? lstcomplex1)
      '()
      (if (null? lstcomplex2)
          '()
      (let ((cmp1 (car lstcomplex1) ))
        (let ((cmp2 (car lstcomplex2) ))
          (let ((sub (cons (- (car cmp1) (car cmp2)) (- (cdr cmp1) (cdr cmp2)))))
            (cons sub (subComplex (cdr lstcomplex1) (cdr lstcomplex2)))
            )))
        ))
  )

;plusComplex:: [Complex No] -> [Complex No] -> [Complex no]
(define (plusComplex lstcomplex1 lstcomplex2)
  (if (null? lstcomplex1)
      '()
      (let ((cmp1 (car lstcomplex1) ))
        (let ((cmp2 (car lstcomplex2) ))
          (let ((sub (cons (+ (car cmp1) (car cmp2)) (+ (cdr cmp1) (cdr cmp2)))))
            (cons sub (plusComplex (cdr lstcomplex1) (cdr lstcomplex2)))
            ))
        ))
  )

;mulComplex:: [Complex No] -> [Complex No] -> [Complex no]
(define (mulComplex xs ys)
  (if (null? xs)
      '()
      (let ((x (car(car xs)) ))
        (let ((y (cdr(car xs)) ))
          (let ((u (car(car ys)) ))
            (let ((v (cdr(car ys)) ))
              (let ((r (- (* x u) (* y v)) ))
                (let ((i (+ (* x v) (* y u)) ))
                  (cons (cons r i) (mulComplex (cdr xs) (cdr ys)) ))
                  )))))
      )
  )
;rootsOfUnity:: Int -> [complex no.]
(define (rootsOfUnity n)
  (let ((a (thetas n 0) ))
    (makeComplex1 (map "cos" a) (map "sin" a)))
  )

;thetas:: Int -> [Double]
(define (thetas n k)
  (if (eq? k n)
      '()
      (cons (*(/ (* 2 3.141592653589793) (* n 1.0)) (* k 1.0)) (thetas n (+ k 1)))
      )
  )

;norm:: [Complex No] -> Double
(define (norm lst1)
  (let ((a (sq_magnitude lst1) ))
    (let ((b (sum a) ))
      (sqrt b)
      ))
  )

;sq_magnitude:: [Complex No.] -> [Double]
;first take magnitude of complex number and then square it.
(define (sq_magnitude lst)
  (if (null? lst)
      '()
      (let ((cmp (car lst) ))
        (let ((a (+ (* (car cmp) (car cmp)) (* (cdr cmp) (cdr cmp))) ))
          (cons a (sq_magnitude (cdr lst))))))
  )
;sum :: [Double] -> Double
(define (sum lst)
  (if (null? lst)
      0
      (+ (car lst) (sum (cdr lst)))
      )
  )

;distance:: [Complex No] -> [Complex No] -> Double
(define (distance lstcomplex1 lstcomplex2)
  (let ((a (subComplex lstcomplex1 lstcomplex2) ))
    (norm a))
  )

; Warning: works only for n=2^km
;time=O(n log(n)) algorithm
;fft:: [Complex No] -> [Complex No] 
(define (fft xs)
  (let ((n (length xs) ))
    (let ((us (mapConjugate (rootsOfUnity n) ) ))
      (let ((a (ffth xs us) ))
        (map1 (/ 1.0 n) a)
        )))
  )

;-- Warning: works only for n=2^km
;-- time=O(n log(n)) algorithm
;fftinv:: [Complex No.] -> [Complex No.] 
(define (fftinv xs)
  (let ((n (length xs) ))
    (let ((us (rootsOfUnity n) ))
      (ffth xs us)
      ))
  )

;ffth:: [ComplexF] -> [ComplexF] -> [ComplexF]
(define (ffth xs us)
  (let ((n (length xs) ))
    (if (eq? 1 n)
        xs
        (if (> n 1)
            (let ((fftOdd (ffth (odds xs) (evns us)) ))
              (let ((fftEvn (ffth (evns xs) (evns us)) ))
                (let ((b (times us fftOdd) ))
                  (plus b fftEvn))
                  ))
            (cons 11 11) ;error
            )))
  )

;multiply complex no.
;times:: [complex No.]->[complex No.]->[complex No.]
(define (times xs fftOdd)
  (let ((odd (replikate fftOdd (length xs)) ))
    (let ((a (mulComplex xs odd) ))
       a)
  ))
    
;add complex no.
;plus:: [complex No.]->[complex No.]->[complex No.]
(define (plus xs fftEvn)
  (let ((evn (replikate fftEvn (length xs)) ))
    (plusComplex evn xs)
    )
  )

;evns::[Complex No.]->[Complex No.]
(define (evns xs)
  (everyNth 2 xs)
  )
;odds::[Complex No.]->[Complex No.]
(define (odds xs)
  (everyNth 2 (cdr xs))   ;tail
  )
;everyNth::int->[Complex No.]->[Complex No.]
(define (everyNth n xs)
 (let ((ys (takeWhile n xs) ))
     (mapHead ys)
    )
  )
    
;takeWhile::int->[Complex No.]->[Complex No.]  
(define (takeWhile n xs)
  (cons xs (takeWhile2 n xs))
  )
;takeWhile2::int->[Complex No.]->[Complex No.]
(define (takeWhile2 n xs)
  (let ((len (length xs) ))
    (if (or (eq? n len) (< len n) )
      '()
      (let ((ys (drop n xs) ))
        (cons ys (takeWhile2 n ys))
        )))
  )

;main:: Int->      Double
(define (main n)
  (let (( lstcmplx (makeComplex2 0 n) ))
    (distance lstcmplx (fftinv (fft lstcmplx)))
    )
  )
    
;----------------------------------------------------------
;Utility functions  


(define (divide1 a b n op)
  (if (< a b)
      (if (eq? op 0)
          n
          a
          )
      (divide1 (- a b) b (+ 1 n) op)
      )
  )

(define (divide a b)
  (divide1 a b 0 0)
  )

;map:: tag [Double] -> [Double]
(define (map tag lst)
  (if (null? lst)
      '()
      (if (eq? tag "cos")
          (cons (cos (car lst)) (map tag (cdr lst)))
          (if (eq? tag "sin")
              (cons (sin (car lst)) (map tag (cdr lst)))
              '()
              )))
  )

;calculate conjugate of complex no.
;map conjugate to list of complext no.
;mapConjugate:: [Complex No.]->[Complex No.]      
(define (mapConjugate cmplx)
  (if (null? cmplx)
      '()
      (let ((a (car cmplx) ))
        (let ((b (cons (car a) (* (- 0 1) (cdr a))) ))
          (cons b (mapConjugate (cdr cmplx)))
          )))
  )

;map1:: Double->[Complex no.]->[Complex No.]
(define (map1 n cmplx)
  (if (null? cmplx)
      '()
      (let ((a (car cmplx) ))
        (let ((b (cons (* n (car a)) (* n (cdr a)) ) ))
          (cons b (map1 n (cdr cmplx)))
          )))
  )

;mad function head to list
;mapHead::[Complex No.]->[Complex No.]
(define (mapHead lst)
  (if (null? lst)
      '()
      (let ((elem (car lst) ))
        (cons (car elem) (mapHead (cdr lst)))
        ))
  )

;find the length of the list
;length :: [Complex No]->Int    
(define (length lst)
  (if (null? lst)
      0
      (+ 1 (length (cdr lst)))
      )
  )

;make the list of size n wither by copying or dropping elements
;replicate::[Complex No.]->[int]->[Complex No.]
(define (replikate lst n)
  (let ((len (length lst) ))
    (if (< len n)
        (cycle lst lst n)
        (if (eq? len n)
            lst
            (take lst n)
        )))
  )

;copy the elements to make list of size n.
;cycle::[Complex No.]-> [Comlex No.]->[int]->[complex No.]
(define (cycle lst1 lst2 n)
  (if (eq? 0 n)
      '() 
      (if (null? lst1)
          (cycle lst2 lst2 n)
          (cons (car lst1) (cycle (cdr lst1) lst2 (- n 1)))
          ))
  )

;take will take first n elements from list
;take::[Complex No.]->int->[complex No.]
(define (take lst n)
  (if (eq? 0 n)
      '()
      (cons (car lst) (take (cdr lst) (- n 1)))
      )
  )

;drop::int->[Complex No.]->[complex No.]
(define (drop n lst)
  (if (eq? 0 n)
      lst
      (drop (- n 1)(cdr lst) )
      )
  )

(main 30)
