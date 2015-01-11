#lang racket
(define (and a b)
        (if a 
               (if b 
                      #t 
                      #f ) 
               #f ) )

(define (isEqual lst1 lst2)
        (let ((v8 (null? lst1 ) ))
                (if v8 
                       (let ((v10 (null? lst2 ) ))
                               (if v10 
                                      #t 
                                      #f ) )
                       (let ((v15 (null? lst2 ) ))
                               (if v15 
                                      (let ((v17 (null? lst1 ) ))
                                              (if v17 
                                                     #t 
                                                     #f ) )
                                      (let ((v0 (car lst1 ) ))
                                              (let ((v378 (car lst2 ) ))
                                                      (let ((v362 (eq? v0 v378 ) ))
                                                              (if v362 
                                                                     (let ((v27 (cdr lst1 ) ))
                                                                             (let ((v29 (cdr lst2 ) ))
                                                                                     (isEqual v27 v29 ) ))
                                                                     #f ) )))) )) ))

(define (mkVar s)
        (let ((v38 (cons s  '() ) ))
                (cons "Var" v38 ) ))

(define (mkCon i)
        (let ((v43 (cons i  '() ) ))
                (cons "Con" v43 ) ))

(define (bkCon c)
        (let ((v46 (cdr c ) ))
                (car v46 ) ))

(define (mkIncr)
        (cons "Incr"  '() ) )

(define (mkAdd t1 t2)
        (let ((v56 (let ((v55 (cons t2  '() ) ))
                             (cons t1 v55 ) )))
                (cons "Add" v56 ) ))

(define (mkLam s t)
        (let ((v63 (let ((v62 (cons t  '() ) ))
                             (cons s v62 ) )))
                (cons "Lam" v63 ) ))

(define (mkApp t1 t2)
        (let ((v70 (let ((v69 (cons t2  '() ) ))
                             (cons t1 v69 ) )))
                (cons "App" v70 ) ))

(define (mkIfZero t1 t2 t3)
        (let ((v79 (let ((v78 (let ((v77 (cons t3  '() ) ))
                                          (cons t2 v77 ) )))
                             (cons t1 v78 ) )))
                (cons "IfZero" v79 ) ))

(define (mkThunk t e)
        (let ((v86 (let ((v85 (cons e  '() ) ))
                             (cons t v85 ) )))
                (cons "Thunk" v86 ) ))

(define (mkEnvPair s t)
        (let ((v91 (cons t  '() ) ))
                (cons s v91 ) ))

(define (addToEnv e env)
        (cons e env ) )

(define (lookup s v)
        (let ((v97 (null? s ) ))
                (if v97 
                       (cons "error1-unknown-variable" 1 ) 
                       (let ((elt (car s ) ))
                               (let ((v480 (car elt ) ))
                                       (let ((v473 (eq? v v480 ) ))
                                               (if v473 
                                                      (let ((v108 (cdr elt ) ))
                                                              (car v108 ) )
                                                      (let ((v111 (cdr s ) ))
                                                              (lookup v111 v ) )) )))) ))

(define (simpleEval env p)
        (let ((tag (car p ) ))
                 (let ((v121 (eq? "Var" tag ) ))
                          (if v121 
                                  (let ((v518 (cdr p ) ))
                                           (let ((v525 (car v518 ) ))
                                                    (let ((v127 (lookup env v525 ) ))
                                                             (simpleEval env v127 ) )))
                                  (let ((v131 (eq? "Add" tag ) ))
                                           (if v131 
                                                   (let ((v627 (cdr p ) ))
                                                            (let ((v641 (car v627 ) ))
                                                                     (let ((u (simpleEval env v641 ) ))
                                                                              (let ((v580 (cdr p ) ))
                                                                                       (let ((v592 (cdr v580 ) ))
                                                                                                (let ((v602 (car v592 ) ))
                                                                                                         (let ((v (simpleEval env v602 ) ))
                                                                                                                  (let ((v542 (bkCon u ) ))
                                                                                                                           (let ((v549 (bkCon v ) ))
                                                                                                                                    (let ((v147 (+ v542 v549 ) ))
                                                                                                                                             (mkCon v147 ) ))))))))))
                                                   (let ((v153 (eq? "Thunk" tag ) ))
                                                            (if v153 
                                                                    (let ((v688 (cdr p ) ))
                                                                             (let ((v680 (cdr v688 ) ))
                                                                                      (let ((v157 (car v680 ) ))
                                                                                               (let ((v670 (cdr p ) ))
                                                                                                        (let ((v160 (car v670 ) ))
                                                                                                                 (simpleEval v157 v160 ) )))))
                                                                    (let ((v164 (eq? "Lam" tag ) ))
                                                                             (if v164 
                                                                                     (mkThunk p env ) 
                                                                                     (let ((v170 (eq? "App" tag ) ))
                                                                                              (if v170 
                                                                                                      (let ((v734 (cdr p ) ))
                                                                                                               (let ((v744 (car v734 ) ))
                                                                                                                        (let ((u (simpleEval env v744 ) ))
                                                                                                                                 (let ((v713 (cdr p ) ))
                                                                                                                                          (let ((v707 (cdr v713 ) ))
                                                                                                                                                   (let ((v181 (car v707 ) ))
                                                                                                                                                            (simpleApply env u v181 ) ))))))
                                                                                                      (let ((v186 (eq? "IfZero" tag ) ))
                                                                                                               (if v186 
                                                                                                                       (let ((v855 (cdr p ) ))
                                                                                                                                (let ((v872 (car v855 ) ))
                                                                                                                                         (let ((val (simpleEval env v872 ) ))
                                                                                                                                                  (let ((v827 (mkCon 0 ) ))
                                                                                                                                                           (let ((v815 (isEqual v827 val ) ))
                                                                                                                                                                    (if v815 
                                                                                                                                                                            (let ((v770 (cdr p ) ))
                                                                                                                                                                                     (let ((v764 (cdr v770 ) ))
                                                                                                                                                                                              (let ((v200 (car v764 ) ))
                                                                                                                                                                                                       (simpleEval env v200 ) )))
                                                                                                                                                                            (let ((v803 (cdr p ) ))
                                                                                                                                                                                     (let ((v796 (cdr v803 ) ))
                                                                                                                                                                                              (let ((v790 (cdr v796 ) ))
                                                                                                                                                                                                       (let ((v207 (car v790 ) ))
                                                                                                                                                                                                                (simpleEval env v207 ) ))))) )))))
                                                                                                                       (let ((v213 (eq? "Con" tag ) ))
                                                                                                                                (if v213 
                                                                                                                                        p 
                                                                                                                                        (let ((v217 (eq? "Incr" tag ) ))
                                                                                                                                                 (if v217 
                                                                                                                                                         (mkCon 0 ) 
                                                                                                                                                         (cons "error2-in-simpleEval" p ) ) )) )) )) )) )) )) )) )))

(define (simpleApply env t a)
        (let ((tag (car t ) ))
                 (let ((v236 (eq? "Thunk" tag ) ))
                          (if v236 
                                  (let ((v1299 (cdr t ) ))
                                           (let ((v1281 (car v1299 ) ))
                                                    (let ((v1264 (cdr v1281 ) ))
                                                             (let ((x (car v1264 ) ))
                                                                      (let ((v1239 (cdr t ) ))
                                                                               (let ((v1225 (car v1239 ) ))
                                                                                        (let ((v1212 (cdr v1225 ) ))
                                                                                                 (let ((v1200 (cdr v1212 ) ))
                                                                                                          (let ((b (car v1200 ) ))
                                                                                                                   (let ((v1177 (cdr t ) ))
                                                                                                                            (let ((v1168 (cdr v1177 ) ))
                                                                                                                                     (let ((e (car v1168 ) ))
                                                                                                                                              (let ((v1147 (mkThunk a env ) ))
                                                                                                                                                       (let ((v1154 (mkEnvPair x v1147 ) ))
                                                                                                                                                                (let ((env2 (addToEnv v1154 e ) ))
                                                                                                                                                                         (simpleEval env2 b ) )))))))))))))))
                                  (cons "error3-cannot-simpleApply" 3 ) ) )))

(define (simpleMain arg)
        (let ((v273 (eq? arg 0 ) ))
                 (if v273 
                         0 
                         (let ((v1355 (pgm arg ) ))
                                  (let ((x (simpleEval  '() v1355 ) ))
                                           (let ((v1344 (/ arg 2 ) ))
                                                    (let ((y (simpleMain v1344 ) ))
                                                             (cons x y ) ))))) ))

(define (pgm arg)
        (let ((v289 (sum0) ))
                 (let ((v291 (mkCon arg ) ))
                          (mkApp v289 v291 ) )))

(define (nMinus1)
        (let ((v294 (mkVar "n" ) ))
                 (let ((v1391 (- 0 1 ) ))
                          (let ((v298 (mkCon v1391 ) ))
                                   (mkAdd v294 v298 ) ))))

(define (partialSum0)
        (let ((v1586 (mkVar "n" ) ))
                 (let ((v1605 (mkCon 0 ) ))
                          (let ((v1622 (mkVar "n" ) ))
                                   (let ((v1637 (mkVar "sum" ) ))
                                            (let ((v1650 (nMinus1) ))
                                                     (let ((v1661 (mkApp v1637 v1650 ) ))
                                                              (let ((v1670 (mkAdd v1622 v1661 ) ))
                                                                       (let ((v1677 (mkIfZero v1586 v1605 v1670 ) ))
                                                                                (let ((v314 (mkLam "n" v1677 ) ))
                                                                                         (mkLam "sum" v314 ) ))))))))))

(define (lfxx)
        (let ((v1728 (mkVar "F" ) ))
                 (let ((v1739 (mkVar "x" ) ))
                          (let ((v1748 (mkVar "x" ) ))
                                   (let ((v1755 (mkApp v1739 v1748 ) ))
                                            (let ((v324 (mkApp v1728 v1755 ) ))
                                                     (mkLam "x" v324 ) ))))))

(define (fix)
        (let ((v1777 (lfxx) ))
                 (let ((v1784 (lfxx) ))
                          (let ((v329 (mkApp v1777 v1784 ) ))
                                   (mkLam "F" v329 ) ))))

(define (sum0)
        (let ((v331 (fix) ))
                 (let ((v332 (partialSum0) ))
                          (mkApp v331 v332 ) )))

(simpleMain 32 ) 

