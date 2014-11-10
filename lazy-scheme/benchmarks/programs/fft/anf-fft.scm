(define (or a b)
        (if a 
               a 
               b ) )

(define (or a b)
        (if a 
               a 
               b ) )

(define (makeComplex1 lst1 lst2)
        (let ((v9 (null? lst1 ) ))
                (if v9 
                       lst1 
                       (let ((v582 (car lst1 ) ))
                               (let ((v592 (car lst2 ) ))
                                       (let ((v15 (cons v582 v592 ) ))
                                               (let ((v0 (cdr lst1 ) ))
                                                       (let ((v574 (cdr lst2 ) ))
                                                               (let ((v20 (makeComplex1 v0 v574 ) ))
                                                                       (cons v15 v20 ) ))))))) ))

(define (makeComplex2 i n)
        (let ((v25 (eq? i n ) ))
                (if v25 
                        () 
                       (let ((v615 (* 1 i ) ))
                               (let ((num (cons v615 0 ) ))
                                       (let ((v620 (+ i 1 ) ))
                                               (let ((v37 (makeComplex2 v620 n ) ))
                                                       (cons num v37 ) ))))) ))

(define (subComplex lstcomplex1 lstcomplex2)
        (let ((v42 (null? lstcomplex1 ) ))
                (if v42 
                        () 
                       (let ((v45 (null? lstcomplex2 ) ))
                               (if v45 
                                       () 
                                      (let ((cmp1 (car lstcomplex1 ) ))
                                              (let ((cmp2 (car lstcomplex2 ) ))
                                                      (let ((v726 (car cmp1 ) ))
                                                              (let ((v740 (car cmp2 ) ))
                                                                      (let ((v668 (- v726 v740 ) ))
                                                                              (let ((v703 (cdr cmp1 ) ))
                                                                                      (let ((v714 (cdr cmp2 ) ))
                                                                                              (let ((v694 (- v703 v714 ) ))
                                                                                                      (let ((sub (cons v668 v694 ) ))
                                                                                                              (let ((v679 (cdr lstcomplex1 ) ))
                                                                                                                      (let ((v686 (cdr lstcomplex2 ) ))
                                                                                                                              (let ((v67 (subComplex v679 v686 ) ))
                                                                                                                                      (cons sub v67 ) ))))))))))))) )) ))

(define (plusComplex lstcomplex1 lstcomplex2)
        (let ((v75 (null? lstcomplex1 ) ))
                (if v75 
                        () 
                       (let ((cmp1 (car lstcomplex1 ) ))
                               (let ((cmp2 (car lstcomplex2 ) ))
                                       (let ((v881 (car cmp1 ) ))
                                               (let ((v895 (car cmp2 ) ))
                                                       (let ((v823 (+ v881 v895 ) ))
                                                               (let ((v858 (cdr cmp1 ) ))
                                                                       (let ((v869 (cdr cmp2 ) ))
                                                                               (let ((v849 (+ v858 v869 ) ))
                                                                                       (let ((sub (cons v823 v849 ) ))
                                                                                               (let ((v834 (cdr lstcomplex1 ) ))
                                                                                                       (let ((v841 (cdr lstcomplex2 ) ))
                                                                                                               (let ((v97 (plusComplex v834 v841 ) ))
                                                                                                                       (cons sub v97 ) ))))))))))))) ))

(define (mulComplex xs ys)
        (let ((v104 (null? xs ) ))
                 (if v104 
                          () 
                         (let ((v1089 (car xs ) ))
                                  (let ((x (car v1089 ) ))
                                           (let ((v1065 (car xs ) ))
                                                    (let ((y (cdr v1065 ) ))
                                                             (let ((v1043 (car ys ) ))
                                                                      (let ((u (car v1043 ) ))
                                                                               (let ((v1023 (car ys ) ))
                                                                                        (let ((v (cdr v1023 ) ))
                                                                                                 (let ((v991 (* x u ) ))
                                                                                                          (let ((v1005 (* y v ) ))
                                                                                                                   (let ((r (- v991 v1005 ) ))
                                                                                                                            (let ((v950 (* x v ) ))
                                                                                                                                     (let ((v973 (* y u ) ))
                                                                                                                                              (let ((i (+ v950 v973 ) ))
                                                                                                                                                       (let ((v134 (cons r i ) ))
                                                                                                                                                                (let ((v957 (cdr xs ) ))
                                                                                                                                                                         (let ((v964 (cdr ys ) ))
                                                                                                                                                                                  (let ((v139 (mulComplex v957 v964 ) ))
                                                                                                                                                                                           (cons v134 v139 ) ))))))))))))))))))) ))

(define (rootsOfUnity n)
        (let ((a (thetas n 0 ) ))
                 (let ((v153 (map "cos" a ) ))
                          (let ((v156 (map "sin" a ) ))
                                   (makeComplex1 v153 v156 ) ))))

(define (thetas n k)
        (let ((v161 (eq? k n ) ))
                 (if v161 
                          () 
                         (let ((v1195 (* 2 3 ) ))
                                  (let ((v1206 (* n 1 ) ))
                                           (let ((v1177 (/ v1195 v1206 ) ))
                                                    (let ((v1186 (* k 1 ) ))
                                                             (let ((v173 (* v1177 v1186 ) ))
                                                                      (let ((v1170 (+ k 1 ) ))
                                                                               (let ((v178 (thetas n v1170 ) ))
                                                                                        (cons v173 v178 ) )))))))) ))

(define (norm lst1)
        (let ((a (sq_magnitude lst1 ) ))
                 (let ((b (sum a ) ))
                          (sqrt b ) )))

(define (sq_magnitude lst)
        (let ((v190 (null? lst ) ))
                 (if v190 
                          () 
                         (let ((cmp (car lst ) ))
                                  (let ((v1294 (car cmp ) ))
                                           (let ((v1307 (car cmp ) ))
                                                    (let ((v1247 (* v1294 v1307 ) ))
                                                             (let ((v1273 (cdr cmp ) ))
                                                                      (let ((v1283 (cdr cmp ) ))
                                                                               (let ((v1265 (* v1273 v1283 ) ))
                                                                                        (let ((a (+ v1247 v1265 ) ))
                                                                                                 (let ((v1258 (cdr lst ) ))
                                                                                                          (let ((v208 (sq_magnitude v1258 ) ))
                                                                                                                   (cons a v208 ) ))))))))))) ))

(define (sum lst)
        (let ((v214 (null? lst ) ))
                 (if v214 
                         0 
                         (let ((v217 (car lst ) ))
                                  (let ((v1350 (cdr lst ) ))
                                           (let ((v220 (sum v1350 ) ))
                                                    (+ v217 v220 ) )))) ))

(define (distance lstcomplex1 lstcomplex2)
        (let ((a (subComplex lstcomplex1 lstcomplex2 ) ))
                 (norm a ) ))

(define (fft xs)
        (let ((n (length xs ) ))
                 (let ((v1370 (rootsOfUnity n ) ))
                          (let ((us (mapConjugate v1370 ) ))
                                   (let ((a (ffth xs us ) ))
                                            (let ((v239 (/ 1 n ) ))
                                                     (map1 v239 a ) ))))))

(define (fftinv xs)
        (let ((n (length xs ) ))
                 (let ((us (rootsOfUnity n ) ))
                          (ffth xs us ) )))

(define (ffth xs us)
        (let ((n (length xs ) ))
                 (let ((v258 (eq? 1 n ) ))
                          (if v258 
                                  xs 
                                  (let ((v262 (> n 1 ) ))
                                           (if v262 
                                                   (let ((v1418 (odds xs ) ))
                                                            (let ((v1429 (evns us ) ))
                                                                     (let ((fftOdd (ffth v1418 v1429 ) ))
                                                                              (let ((v1392 (evns xs ) ))
                                                                                       (let ((v1400 (evns us ) ))
                                                                                                (let ((fftEvn (ffth v1392 v1400 ) ))
                                                                                                         (let ((b (times us fftOdd ) ))
                                                                                                                  (plus b fftEvn ) )))))))
                                                   (cons 11 11 ) ) )) )))

(define (times xs fftOdd)
        (let ((v1473 (length xs ) ))
                 (let ((odd (replikate fftOdd v1473 ) ))
                          (let ((a (mulComplex xs odd ) ))
                                   a ))))

(define (plus xs fftEvn)
        (let ((v1485 (length xs ) ))
                 (let ((evn (replikate fftEvn v1485 ) ))
                          (plusComplex evn xs ) )))

(define (evns xs)
        (everyNth 2 xs ) )

(define (odds xs)
        (let ((v311 (cdr xs ) ))
                 (everyNth 2 v311 ) ))

(define (everyNth n xs)
        (let ((ys (takeWhile n xs ) ))
                 (mapHead ys ) ))

(define (takeWhile n xs)
        (let ((v322 (takeWhile2 n xs ) ))
                 (cons xs v322 ) ))

(define (takeWhile2 n xs)
        (let ((len (length xs ) ))
                 (let ((v1519 (eq? n len ) ))
                          (let ((v1529 (< len n ) ))
                                   (let ((v1512 (or v1519 v1529 ) ))
                                            (if v1512 
                                                     () 
                                                    (let ((ys (drop n xs ) ))
                                                             (let ((v340 (takeWhile2 n ys ) ))
                                                                      (cons ys v340 ) ))) )))))

(define (main n)
        (let ((lstcmplx (makeComplex2 0 n ) ))
                 (let ((v1548 (fft lstcmplx ) ))
                          (let ((v351 (fftinv v1548 ) ))
                                   (distance lstcmplx v351 ) ))))

(define (divide1 a b n op)
        (let ((v356 (< a b ) ))
                 (if v356 
                         (let ((v359 (eq? op 0 ) ))
                                  (if v359 
                                          n 
                                          a ) )
                         (let ((v365 (- a b ) ))
                                  (let ((v369 (+ 1 n ) ))
                                           (divide1 v365 b v369 op ) ))) ))

(define (divide a b)
        (divide1 a b 0 0 ) )

(define (map tag lst)
        (let ((v379 (null? lst ) ))
                 (if v379 
                          () 
                         (let ((v383 (eq? tag "cos" ) ))
                                  (if v383 
                                          (let ((v1638 (car lst ) ))
                                                   (let ((v386 (cos v1638 ) ))
                                                            (let ((v1631 (cdr lst ) ))
                                                                     (let ((v390 (map tag v1631 ) ))
                                                                              (cons v386 v390 ) ))))
                                          (let ((v394 (eq? tag "sin" ) ))
                                                   (if v394 
                                                           (let ((v1617 (car lst ) ))
                                                                    (let ((v397 (sin v1617 ) ))
                                                                             (let ((v1610 (cdr lst ) ))
                                                                                      (let ((v401 (map tag v1610 ) ))
                                                                                               (cons v397 v401 ) ))))
                                                            () ) )) )) ))

(define (mapConjugate cmplx)
        (let ((v408 (null? cmplx ) ))
                 (if v408 
                          () 
                         (let ((a (car cmplx ) ))
                                  (let ((v1691 (car a ) ))
                                           (let ((v1717 (- 0 1 ) ))
                                                    (let ((v1727 (cdr a ) ))
                                                             (let ((v1709 (* v1717 v1727 ) ))
                                                                      (let ((b (cons v1691 v1709 ) ))
                                                                               (let ((v1702 (cdr cmplx ) ))
                                                                                        (let ((v424 (mapConjugate v1702 ) ))
                                                                                                 (cons b v424 ) ))))))))) ))

(define (map1 n cmplx)
        (let ((v430 (null? cmplx ) ))
                 (if v430 
                          () 
                         (let ((a (car cmplx ) ))
                                  (let ((v1806 (car a ) ))
                                           (let ((v1772 (* n v1806 ) ))
                                                    (let ((v1796 (cdr a ) ))
                                                             (let ((v1788 (* n v1796 ) ))
                                                                      (let ((b (cons v1772 v1788 ) ))
                                                                               (let ((v1781 (cdr cmplx ) ))
                                                                                        (let ((v447 (map1 n v1781 ) ))
                                                                                                 (cons b v447 ) ))))))))) ))

(define (mapHead lst)
        (let ((v453 (null? lst ) ))
                 (if v453 
                          () 
                         (let ((elem (car lst ) ))
                                  (let ((v458 (car elem ) ))
                                           (let ((v1846 (cdr lst ) ))
                                                    (let ((v461 (mapHead v1846 ) ))
                                                             (cons v458 v461 ) ))))) ))

(define (length lst)
        (let ((v466 (null? lst ) ))
                 (if v466 
                         0 
                         (let ((v1867 (cdr lst ) ))
                                  (let ((v471 (length v1867 ) ))
                                           (+ 1 v471 ) ))) ))

(define (replikate lst n)
        (let ((len (length lst ) ))
                 (let ((v478 (< len n ) ))
                          (if v478 
                                  (cycle lst lst n ) 
                                  (let ((v485 (eq? len n ) ))
                                           (if v485 
                                                   lst 
                                                   (take lst n ) ) )) )))

(define (cycle lst1 lst2 n)
        (let ((v495 (eq? 0 n ) ))
                 (if v495 
                          () 
                         (let ((v498 (null? lst1 ) ))
                                  (if v498 
                                          (cycle lst2 lst2 n ) 
                                          (let ((v504 (car lst1 ) ))
                                                   (let ((v1907 (cdr lst1 ) ))
                                                            (let ((v1914 (- n 1 ) ))
                                                                     (let ((v511 (cycle v1907 lst2 v1914 ) ))
                                                                              (cons v504 v511 ) ))))) )) ))

(define (take lst n)
        (let ((v517 (eq? 0 n ) ))
                 (if v517 
                          () 
                         (let ((v520 (car lst ) ))
                                  (let ((v1954 (cdr lst ) ))
                                           (let ((v1961 (- n 1 ) ))
                                                    (let ((v526 (take v1954 v1961 ) ))
                                                             (cons v520 v526 ) ))))) ))

(define (drop n lst)
        (let ((v531 (eq? 0 n ) ))
                 (if v531 
                         lst 
                         (let ((v535 (- n 1 ) ))
                                  (let ((v537 (cdr lst ) ))
                                           (drop v535 v537 ) ))) ))

(main 1100 ) 

