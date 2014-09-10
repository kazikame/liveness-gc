(define (or a b)
        (if a 
               a 
               b ) )

(define (append ax ay)
        (let ((v5 (null? ax ) ))
                (if v5 
                       ay 
                       (let ((v8 (car ax ) ))
                               (let ((v0 (cdr ax ) ))
                                       (let ((v12 (append v0 ay ) ))
                                               (cons v8 v12 ) )))) ))

(define (divide1 a b n op)
        (let ((v17 (< a b ) ))
                (if v17 
                       (let ((v20 (eq? op 0 ) ))
                               (if v20 
                                      n 
                                      a ) )
                       (let ((v26 (- a b ) ))
                               (let ((v30 (+ 1 n ) ))
                                       (divide1 v26 b v30 op ) ))) ))

(define (divide a b)
        (divide1 a b 0 0 ) )

(define (take n lst)
        (let ((v41 (eq? 0 n ) ))
                (if v41 
                        () 
                       (let ((v44 (car lst ) ))
                               (let ((v400 (- n 1 ) ))
                                       (let ((v407 (cdr lst ) ))
                                               (let ((v50 (take v400 v407 ) ))
                                                       (cons v44 v50 ) ))))) ))

(define (drop n lst)
        (let ((v55 (eq? 0 n ) ))
                (if v55 
                       lst 
                       (let ((v59 (- n 1 ) ))
                               (let ((v61 (cdr lst ) ))
                                       (drop v59 v61 ) ))) ))

(define (reverse lst)
        (let ((v65 (null? lst ) ))
                (if v65 
                        () 
                       (let ((v456 (cdr lst ) ))
                               (let ((v69 (reverse v456 ) ))
                                       (let ((v446 (car lst ) ))
                                               (let ((v73 (cons v446  () ) ))
                                                       (append v69 v73 ) ))))) ))

(define (zip xs ys)
        (let ((v544 (null? xs ) ))
                (let ((v558 (null? ys ) ))
                        (let ((v503 (or v544 v558 ) ))
                                (if v503 
                                        () 
                                       (let ((v522 (car xs ) ))
                                               (let ((v532 (car ys ) ))
                                                       (let ((v86 (cons v522 v532 ) ))
                                                               (let ((v507 (cdr xs ) ))
                                                                       (let ((v514 (cdr ys ) ))
                                                                               (let ((v91 (zip v507 v514 ) ))
                                                                                       (cons v86 v91 ) ))))))) ))))

(define (getLength lst)
        (let ((v95 (null? lst ) ))
                (if v95 
                       0 
                       (let ((v583 (cdr lst ) ))
                               (let ((v100 (getLength v583 ) ))
                                       (+ 1 v100 ) ))) ))

(define (findk k km m pairs)
        (let ((v104 (null? pairs ) ))
                (if v104 
                       km 
                       (let ((v672 (car pairs ) ))
                               (let ((v631 (car v672 ) ))
                                       (let ((v658 (car pairs ) ))
                                               (let ((v646 (cdr v658 ) ))
                                                       (let ((s (+ v631 v646 ) ))
                                                               (let ((v115 (< s m ) ))
                                                                       (if v115 
                                                                              (let ((v118 (+ 1 k ) ))
                                                                                      (let ((v122 (cdr pairs ) ))
                                                                                              (findk v118 km m v122 ) ))
                                                                              (let ((v126 (+ 1 k ) ))
                                                                                      (let ((v130 (cdr pairs ) ))
                                                                                              (findk v126 k s v130 ) ))) ))))))) ))

(define (elem n lst)
        (let ((v136 (null? lst ) ))
                (if v136 
                       0 
                       (let ((v716 (car lst ) ))
                               (let ((v710 (eq? v716 n ) ))
                                       (if v710 
                                              1 
                                              (let ((v145 (cdr lst ) ))
                                                      (elem n v145 ) )) ))) ))

(define (zip0 lst)
        (let ((v150 (null? lst ) ))
                (if v150 
                        () 
                       (let ((v757 (car lst ) ))
                               (let ((v155 (cons v757 0 ) ))
                                       (let ((v750 (cdr lst ) ))
                                               (let ((v158 (zip0 v750 ) ))
                                                       (cons v155 v158 ) ))))) ))

(define (mapsnd pairs)
        (let ((v162 (null? pairs ) ))
                (if v162 
                        () 
                       (let ((v791 (car pairs ) ))
                               (let ((v166 (cdr v791 ) ))
                                       (let ((v784 (cdr pairs ) ))
                                               (let ((v169 (mapsnd v784 ) ))
                                                       (cons v166 v169 ) ))))) ))

(define (max a b)
        (let ((v174 (> a b ) ))
                (if v174 
                       a 
                       b ) ))

(define (algb2 k0j1 k1j1 pairs x)
        (let ((v179 (null? pairs ) ))
                (if v179 
                        () 
                       (let ((v866 (car pairs ) ))
                               (let ((k0j (cdr v866 ) ))
                                       (let ((v846 (car pairs ) ))
                                               (let ((y (car v846 ) ))
                                                       (let ((v817 (eq? x y ) ))
                                                               (if v817 
                                                                      (let ((kjcurr (+ 1 k0j1 ) ))
                                                                              (let ((v199 (cons y kjcurr ) ))
                                                                                      (let ((v823 (cdr pairs ) ))
                                                                                              (let ((v205 (algb2 k0j kjcurr v823 x ) ))
                                                                                                      (cons v199 v205 ) ))))
                                                                      (let ((kjcurr (max k1j1 k0j ) ))
                                                                              (let ((v199 (cons y kjcurr ) ))
                                                                                      (let ((v832 (cdr pairs ) ))
                                                                                              (let ((v205 (algb2 k0j kjcurr v832 x ) ))
                                                                                                      (cons v199 v205 ) ))))) )))))) ))

(define (algb1 xs pairs)
        (let ((v212 (null? xs ) ))
                (if v212 
                       (mapsnd pairs ) 
                       (let ((v912 (car xs ) ))
                               (let ((n (algb2 0 0 pairs v912 ) ))
                                       (let ((v222 (cdr xs ) ))
                                               (algb1 v222 n ) )))) ))

(define (algb xs ys)
        (let ((v231 (let ((v230 (zip0 ys ) ))
                              (algb1 xs v230 ) )))
                (cons 0 v231 ) ))

(define (algc m n xs ys lst)
        (let ((v234 (null? ys ) ))
                 (if v234 
                         lst 
                         (let ((v1157 (getLength xs ) ))
                                  (let ((v1131 (eq? 1 v1157 ) ))
                                           (if v1131 
                                                   (let ((v958 (car xs ) ))
                                                            (let ((v950 (elem v958 ys ) ))
                                                                     (let ((v944 (eq? 1 v950 ) ))
                                                                              (if v944 
                                                                                      (let ((v247 (car xs ) ))
                                                                                               (cons v247 lst ) )
                                                                                      lst ) )))
                                                   (let ((m2 (divide m 2 ) ))
                                                            (let ((xs1 (take m2 xs ) ))
                                                                     (let ((xs2 (drop m2 xs ) ))
                                                                              (let ((l1 (algb xs1 ys ) ))
                                                                                       (let ((v1076 (reverse xs2 ) ))
                                                                                                (let ((v1093 (reverse ys ) ))
                                                                                                         (let ((v1108 (algb v1076 v1093 ) ))
                                                                                                                  (let ((l2 (reverse v1108 ) ))
                                                                                                                           (let ((v1024 (- 0 1 ) ))
                                                                                                                                    (let ((v1036 (zip l1 l2 ) ))
                                                                                                                                             (let ((k (findk 0 0 v1024 v1036 ) ))
                                                                                                                                                      (let ((v987 (- m m2 ) ))
                                                                                                                                                               (let ((v997 (- n k ) ))
                                                                                                                                                                        (let ((v1005 (drop k ys ) ))
                                                                                                                                                                                 (let ((ex1 (algc v987 v997 xs2 v1005 lst ) ))
                                                                                                                                                                                          (let ((v296 (take k ys ) ))
                                                                                                                                                                                                   (algc m2 k xs1 v296 ex1 ) ))))))))))))))))) ))) ))

(define (lcss xs ys)
        (let ((v309 (getLength xs ) ))
                 (let ((v311 (getLength ys ) ))
                          (algc v309 v311 xs ys  () ) )))

(define (makeList n)
        (let ((v318 (eq? 0 n ) ))
                 (if v318 
                          () 
                         (let ((v1234 (- n 1 ) ))
                                  (let ((v324 (makeList v1234 ) ))
                                           (cons n v324 ) ))) ))

(let ((a (makeList 10 ) ))
         (let ((b (makeList 5 ) ))
                  (lcss a b ) ))

