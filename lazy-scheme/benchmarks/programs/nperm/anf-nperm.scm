''''''(define (and a b)
        (if a 
               (if b 
                      #t 
                      #f ) 
               #f ) )

(define (append res1 res2)
        (let ((v8 (null? res1 ) ))
                (if v8 
                       res2 
                       (let ((v11 (car res1 ) ))
                               (let ((v0 (cdr res1 ) ))
                                       (let ((v15 (append v0 res2 ) ))
                                               (cons v11 v15 ) )))) ))

(define (getVal list i)
        (let ((v20 (eq? 0 i ) ))
                (if v20 
                       (car list ) 
                       (let ((v24 (cdr list ) ))
                               (let ((v27 (- i 1 ) ))
                                       (getVal v24 v27 ) ))) ))

(define (swapij list i j ival jval)
        (let ((v553 (< i 0 ) ))
                (let ((v578 (< j 0 ) ))
                        (let ((v531 (and v553 v578 ) ))
                                (if v531 
                                       list 
                                       (let ((v40 (eq? 0 i ) ))
                                               (if v40 
                                                      (let ((v485 (cdr list ) ))
                                                              (let ((v494 (- i 1 ) ))
                                                                      (let ((v501 (- j 1 ) ))
                                                                              (let ((v52 (swapij v485 v494 v501 ival jval ) ))
                                                                                      (cons jval v52 ) ))))
                                                      (let ((v56 (eq? 0 j ) ))
                                                              (if v56 
                                                                     (let ((v429 (cdr list ) ))
                                                                             (let ((v438 (- i 1 ) ))
                                                                                     (let ((v445 (- j 1 ) ))
                                                                                             (let ((v68 (swapij v429 v438 v445 ival jval ) ))
                                                                                                     (cons ival v68 ) ))))
                                                                     (let ((v71 (car list ) ))
                                                                             (let ((v454 (cdr list ) ))
                                                                                     (let ((v463 (- i 1 ) ))
                                                                                             (let ((v470 (- j 1 ) ))
                                                                                                     (let ((v82 (swapij v454 v463 v470 ival jval ) ))
                                                                                                             (cons v71 v82 ) )))))) )) )) ))))

(define (swap list i j)
        (let ((ival (getVal list i ) ))
                (let ((jval (getVal list j ) ))
                        (swapij list i j ival jval ) )))

(define (loop list i k n result)
        (let ((v103 (eq? i n ) ))
                (if v103 
                       result 
                       (let ((v632 (- n 1 ) ))
                               (let ((newlist (swap list i v632 ) ))
                                       (let ((v619 (- n 1 ) ))
                                               (let ((cresult (permute newlist k v619 ) ))
                                                       (let ((v120 (+ i 1 ) ))
                                                               (let ((v125 (append result cresult ) ))
                                                                       (loop list v120 k n v125 ) ))))))) ))

(define (permute list k n)
        (let ((v132 (eq? k n ) ))
                (if v132 
                       (cons list  () ) 
                       (loop list k k n  () ) ) ))

(define (makeList n list)
        (let ((v145 (eq? 0 n ) ))
                (if v145 
                       list 
                       (let ((v149 (- n 1 ) ))
                               (let ((v152 (cons n list ) ))
                                       (makeList v149 v152 ) ))) ))

(define (permutation n)
        (let ((v157 (makeList n  () ) ))
                (permute v157 0 n ) ))

(define (make-queue size)
        (let ((v163 (eq? size 0 ) ))
                (if v163 
                        () 
                       (let ((v689 (- size 1 ) ))
                               (let ((v169 (make-queue v689 ) ))
                                       (cons  () v169 ) ))) ))

(define (fill i n queue)
        (let ((v174 (eq? 0 i ) ))
                (if v174 
                       (let ((v176 (permutation n ) ))
                               (let ((v178 (cdr queue ) ))
                                       (cons v176 v178 ) ))
                       (let ((v181 (car queue ) ))
                               (let ((v724 (- i 1 ) ))
                                       (let ((v731 (cdr queue ) ))
                                               (let ((v188 (fill v724 n v731 ) ))
                                                       (cons v181 v188 ) ))))) ))

(define (fill-queue i j n queue)
        (let ((v193 (< i j ) ))
                (if v193 
                       (let ((newq (fill i n queue ) ))
                               (let ((v200 (+ i 1 ) ))
                                       (fill-queue v200 j n newq ) ))
                       queue ) ))

(define (get-ref j queue)
        (let ((v210 (eq? 0 j ) ))
                (if v210 
                       (car queue ) 
                       (let ((v215 (- j 1 ) ))
                               (let ((v217 (cdr queue ) ))
                                       (get-ref v215 v217 ) ))) ))

(define (set-ref i ref queue)
        (let ((v222 (eq? 0 i ) ))
                (if v222 
                       (let ((v225 (cdr queue ) ))
                               (cons ref v225 ) )
                       (let ((v228 (car queue ) ))
                               (let ((v792 (- i 1 ) ))
                                       (let ((v799 (cdr queue ) ))
                                               (let ((v235 (set-ref v792 ref v799 ) ))
                                                       (cons v228 v235 ) ))))) ))

(define (loop-flush i k l queue)
        (let ((v240 (< i k ) ))
                (if v240 
                       (let ((j (+ i l ) ))
                               (let ((v246 (< j k ) ))
                                       (if v246 
                                              (let ((ref (get-ref j queue ) ))
                                                      (let ((newq (set-ref i ref queue ) ))
                                                              (let ((v256 (+ 1 i ) ))
                                                                      (loop-flush v256 k l newq ) )))
                                              (let ((newq (set-ref i  () queue ) ))
                                                      (let ((v269 (+ 1 i ) ))
                                                              (loop-flush v269 k l newq ) ))) ))
                       queue ) ))

(define (flush-queue k l queue)
        (loop-flush 0 k l queue ) )

(define (run-operations m n k l queue)
        (let ((v286 (eq? 0 m ) ))
                (if v286 
                       queue 
                       (let ((v854 (- k l ) ))
                               (let ((queuefill (fill-queue v854 k n queue ) ))
                                       (let ((queueflush (flush-queue k l queuefill ) ))
                                               (let ((v301 (- m 1 ) ))
                                                       (run-operations v301 n k l queueflush ) ))))) ))

(define (MpermNKL-benchmark m n k l)
        (let ((queue (make-queue k ) ))
                (run-operations m n k l queue ) ))

(define (tenperm-benchmark m n k l)
        (MpermNKL-benchmark m n k l ) )

(tenperm-benchmark 10 7 2 1 ) 

