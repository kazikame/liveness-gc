(define (or a b)
        (if a 
               a 
               b ) )

(define (makeNode k t1 t2)
        (let ((v6 (cons "node" k ) ))
                (let ((v11 (let ((v10 (cons t2  () ) ))
                                     (cons t1 v10 ) )))
                        (cons v6 v11 ) )))

(define (makeLeaf k e)
        (let ((v15 (cons "leaf" k ) ))
                (let ((v18 (cons e  () ) ))
                        (cons v15 v18 ) )))

(define (insertT k e t)
        (let ((v371 (car t ) ))
                (let ((v362 (eq? v371 "empty" ) ))
                        (if v362 
                               (makeLeaf k e ) 
                               (let ((v351 (car t ) ))
                                       (let ((v0 (car v351 ) ))
                                               (let ((v340 (eq? v0 "node" ) ))
                                                       (if v340 
                                                              (insertTNode k e t ) 
                                                              (insertTLeaf k e t ) ) )))) )))

(define (insertTLeaf k e t)
        (let ((v409 (car t ) ))
                (let ((key (cdr v409 ) ))
                        (let ((v47 (< k key ) ))
                                (if v47 
                                       (let ((v51 (makeLeaf k e ) ))
                                               (makeNode k v51 t ) )
                                       (let ((v56 (> k key ) ))
                                               (if v56 
                                                      (let ((v61 (makeLeaf k e ) ))
                                                              (makeNode key t v61 ) )
                                                      (cons 99 99 ) ) )) ))))

(define (insertTNode k e t)
        (let ((v485 (car t ) ))
                (let ((key (cdr v485 ) ))
                        (let ((v468 (cdr t ) ))
                                (let ((tree1 (car v468 ) ))
                                        (let ((v453 (cdr t ) ))
                                                (let ((v443 (cdr v453 ) ))
                                                        (let ((tree2 (car v443 ) ))
                                                                (let ((v81 (> k key ) ))
                                                                        (if v81 
                                                                               (let ((v87 (insertT k e tree2 ) ))
                                                                                       (makeNode key tree1 v87 ) )
                                                                               (let ((v93 (insertT k e tree1 ) ))
                                                                                       (makeNode key v93 tree2 ) )) )))))))))

(define (lookupT k t)
        (let ((v552 (car t ) ))
                (let ((v541 (car v552 ) ))
                        (let ((v532 (eq? v541 "empty" ) ))
                                (if v532 
                                        () 
                                       (let ((v518 (car t ) ))
                                               (let ((v511 (car v518 ) ))
                                                       (let ((v506 (eq? v511 "node" ) ))
                                                               (if v506 
                                                                      (lookupTNode k t ) 
                                                                      (lookupTLeaf k t ) ) )))) ))))

(define (lookupTLeaf k t)
        (let ((v585 (car t ) ))
                (let ((key (cdr v585 ) ))
                        (let ((v573 (cdr t ) ))
                                (let ((e (car v573 ) ))
                                        (let ((v127 (eq? k key ) ))
                                                (if v127 
                                                       e 
                                                        () ) ))))))

(define (lookupTNode k t)
        (let ((v643 (car t ) ))
                (let ((key (cdr v643 ) ))
                        (let ((v628 (cdr t ) ))
                                (let ((tree1 (car v628 ) ))
                                        (let ((v615 (cdr t ) ))
                                                (let ((v607 (cdr v615 ) ))
                                                        (let ((tree2 (car v607 ) ))
                                                                (let ((v145 (> k key ) ))
                                                                        (if v145 
                                                                               (lookupT k tree2 ) 
                                                                               (lookupT k tree1 ) ) )))))))))

(define (forceTree t)
         () )

(define (readTree n t)
        (let ((v159 (eq? 0 n ) ))
                (if v159 
                       t 
                       (let ((en (makeEntity n ) ))
                               (let ((v691 (car en ) ))
                                       (let ((v786 (cdr en ) ))
                                               (let ((v722 (car v786 ) ))
                                                       (let ((v772 (cdr en ) ))
                                                               (let ((v760 (cdr v772 ) ))
                                                                       (let ((v749 (car v760 ) ))
                                                                               (let ((v739 (cons v749  () ) ))
                                                                                       (let ((v713 (cons v722 v739 ) ))
                                                                                               (let ((e (cons v691 v713 ) ))
                                                                                                       (let ((k (makeKey e ) ))
                                                                                                               (let ((v180 (- n 3 ) ))
                                                                                                                       (let ((v184 (insertT k e t ) ))
                                                                                                                               (readTree v180 v184 ) )))))))))))))) ))

(define (makeEntity n)
        (let ((f (- n 2 ) ))
                (let ((g (- n 1 ) ))
                        (let ((h n ))
                                (let ((v202 (let ((v201 (cons h  () ) ))
                                                      (cons g v201 ) )))
                                        (cons f v202 ) )))))

(define (makeKey e)
        (car e ) )

(define (join t1 t2 tj)
        (let ((v1239 (car t1 ) ))
                 (let ((v1282 (eq? v1239 "empty" ) ))
                          (let ((v1323 (car t2 ) ))
                                   (let ((v1362 (eq? v1323 "empty" ) ))
                                            (let ((v1203 (or v1282 v1362 ) ))
                                                     (if v1203 
                                                             tj 
                                                             (let ((v1138 (car t1 ) ))
                                                                      (let ((v1104 (car v1138 ) ))
                                                                               (let ((v1072 (eq? v1104 "node" ) ))
                                                                                        (if v1072 
                                                                                                (let ((v860 (cdr t1 ) ))
                                                                                                         (let ((tree1 (car v860 ) ))
                                                                                                                  (let ((v848 (cdr t1 ) ))
                                                                                                                           (let ((v841 (cdr v848 ) ))
                                                                                                                                    (let ((tree2 (car v841 ) ))
                                                                                                                                             (let ((v236 (join tree2 t2 tj ) ))
                                                                                                                                                      (join tree1 t2 v236 ) ))))))
                                                                                                (let ((v1041 (car t1 ) ))
                                                                                                         (let ((key (cdr v1041 ) ))
                                                                                                                  (let ((v1014 (cdr t1 ) ))
                                                                                                                           (let ((e (car v1014 ) ))
                                                                                                                                    (let ((a (car e ) ))
                                                                                                                                             (let ((v989 (cdr e ) ))
                                                                                                                                                      (let ((b (car v989 ) ))
                                                                                                                                                               (let ((v967 (cdr e ) ))
                                                                                                                                                                        (let ((v950 (cdr v967 ) ))
                                                                                                                                                                                 (let ((c (car v950 ) ))
                                                                                                                                                                                          (let ((entity (lookupT a t2 ) ))
                                                                                                                                                                                                   (let ((v259 (null? entity ) ))
                                                                                                                                                                                                            (if v259 
                                                                                                                                                                                                                    tj 
                                                                                                                                                                                                                    (let ((d (car entity ) ))
                                                                                                                                                                                                                             (let ((v917 (cdr entity ) ))
                                                                                                                                                                                                                                      (let ((f (car v917 ) ))
                                                                                                                                                                                                                                               (let ((v904 (cons f  () ) ))
                                                                                                                                                                                                                                                        (let ((v896 (cons d v904 ) ))
                                                                                                                                                                                                                                                                 (let ((v889 (cons c v896 ) ))
                                                                                                                                                                                                                                                                          (let ((v883 (cons b v889 ) ))
                                                                                                                                                                                                                                                                                   (let ((newe (cons a v883 ) ))
                                                                                                                                                                                                                                                                                            (insertT c newe tj ) ))))))))) ))))))))))))) )))) ))))))

(define (main)
        (let ((v295 (eq? 0 0 ) ))
                 (if v295 
                         (let ((v1424 (cons "empty"  () ) ))
                                  (let ((a (readTree 516 v1424 ) ))
                                           (let ((v1412 (cons "empty"  () ) ))
                                                    (let ((b (readTree 1530 v1412 ) ))
                                                             (let ((v310 (cons "empty"  () ) ))
                                                                      (join a b v310 ) )))))
                         (cons 9 9 ) ) ))

(main) 

