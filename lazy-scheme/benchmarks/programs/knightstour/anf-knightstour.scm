''''''''''(define (and a b)
        (if a 
               (if b 
                      #t 
                      #f ) 
               #f ) )

(define (or a b)
        (if a 
               a 
               b ) )

(define (append ax ay)
        (let ((v12 (null? ax ) ))
                (if v12 
                       ay 
                       (let ((v15 (car ax ) ))
                               (let ((v0 (cdr ax ) ))
                                       (let ((v19 (append v0 ay ) ))
                                               (cons v15 v19 ) )))) ))

(define (inResult x y result)
        (let ((v23 (null? result ) ))
                (if v23 
                       0 
                       (let ((posxy (car result ) ))
                               (let ((v401 (car posxy ) ))
                                       (let ((v414 (eq? x v401 ) ))
                                               (let ((v425 (cdr posxy ) ))
                                                       (let ((v434 (eq? y v425 ) ))
                                                               (let ((v395 (and v414 v434 ) ))
                                                                       (if v395 
                                                                              1 
                                                                              (let ((v40 (cdr result ) ))
                                                                                      (inResult x y v40 ) )) ))))))) ))

(define (check x y curlst boardsize)
        (let ((v559 (> x boardsize ) ))
                (let ((v576 (< x 1 ) ))
                        (let ((v545 (or v559 v576 ) ))
                                (if v545 
                                        () 
                                       (let ((v510 (> y boardsize ) ))
                                               (let ((v523 (< y 1 ) ))
                                                       (let ((v500 (or v510 v523 ) ))
                                                               (if v500 
                                                                       () 
                                                                      (let ((v482 (inResult x y curlst ) ))
                                                                              (let ((v475 (eq? 1 v482 ) ))
                                                                                      (if v475 
                                                                                              () 
                                                                                             (let ((v466 (cons x y ) ))
                                                                                                     (let ((v73 (cons v466  () ) ))
                                                                                                             (append curlst v73 ) ))) ))) )))) ))))

(define (join ax ay)
        (let ((v79 (null? ax ) ))
                (if v79 
                       ay 
                       (let ((v82 (null? ay ) ))
                               (if v82 
                                      (cons ax  () ) 
                                      (cons ax ay ) ) )) ))

(define (generateMoves posx posy)
        (let ((v1049 (+ posx 1 ) ))
                 (let ((v1084 (+ posy 2 ) ))
                          (let ((mv1 (cons v1049 v1084 ) ))
                                   (let ((v978 (+ posx 1 ) ))
                                            (let ((v1010 (- posy 2 ) ))
                                                     (let ((mv2 (cons v978 v1010 ) ))
                                                              (let ((v913 (+ posx 2 ) ))
                                                                       (let ((v942 (+ posy 1 ) ))
                                                                                (let ((mv3 (cons v913 v942 ) ))
                                                                                         (let ((v854 (+ posx 2 ) ))
                                                                                                  (let ((v880 (- posy 1 ) ))
                                                                                                           (let ((mv4 (cons v854 v880 ) ))
                                                                                                                    (let ((v801 (- posx 1 ) ))
                                                                                                                             (let ((v824 (+ posy 2 ) ))
                                                                                                                                      (let ((mv5 (cons v801 v824 ) ))
                                                                                                                                               (let ((v754 (- posx 1 ) ))
                                                                                                                                                        (let ((v774 (- posy 2 ) ))
                                                                                                                                                                 (let ((mv6 (cons v754 v774 ) ))
                                                                                                                                                                          (let ((v713 (- posx 2 ) ))
                                                                                                                                                                                   (let ((v730 (+ posy 1 ) ))
                                                                                                                                                                                            (let ((mv7 (cons v713 v730 ) ))
                                                                                                                                                                                                     (let ((v633 (- posx 2 ) ))
                                                                                                                                                                                                              (let ((v692 (- posy 1 ) ))
                                                                                                                                                                                                                       (let ((mv8 (cons v633 v692 ) ))
                                                                                                                                                                                                                                (let ((v680 (cons mv8  () ) ))
                                                                                                                                                                                                                                         (let ((v670 (cons mv7 v680 ) ))
                                                                                                                                                                                                                                                  (let ((v661 (cons mv6 v670 ) ))
                                                                                                                                                                                                                                                           (let ((v653 (cons mv5 v661 ) ))
                                                                                                                                                                                                                                                                    (let ((v646 (cons mv4 v653 ) ))
                                                                                                                                                                                                                                                                             (let ((v640 (cons mv3 v646 ) ))
                                                                                                                                                                                                                                                                                      (let ((v162 (cons mv2 v640 ) ))
                                                                                                                                                                                                                                                                                               (cons mv1 v162 ) ))))))))))))))))))))))))))))))))

(define (tries plst move bs)
        (let ((v173 (car move ) ))
                 (let ((v175 (cdr move ) ))
                          (check v173 v175 plst bs ) )))

(define (try plst movelst bs newlst)
        (let ((v180 (null? movelst ) ))
                 (if v180 
                         newlst 
                         (let ((v1138 (car movelst ) ))
                                  (let ((n (tries plst v1138 bs ) ))
                                           (let ((nlst (join n newlst ) ))
                                                    (let ((v192 (cdr movelst ) ))
                                                             (try plst v192 bs nlst ) ))))) ))

(define (getLastMove plst)
        (let ((v1168 (cdr plst ) ))
                 (let ((v1162 (null? v1168 ) ))
                          (if v1162 
                                  (car plst ) 
                                  (let ((v205 (cdr plst ) ))
                                           (getLastMove v205 ) )) )))

(define (explore bs pathlst)
        (let ((lastmove (getLastMove pathlst ) ))
                 (let ((v1186 (car lastmove ) ))
                          (let ((v1193 (cdr lastmove ) ))
                                   (let ((moves (generateMoves v1186 v1193 ) ))
                                            (try pathlst moves bs  () ) )))))

(define (nextStep bs result)
        (let ((v223 (null? result ) ))
                 (if v223 
                          () 
                         (let ((v1225 (car result ) ))
                                  (let ((v228 (explore bs v1225 ) ))
                                           (let ((v1215 (cdr result ) ))
                                                    (let ((v232 (nextStep bs v1215 ) ))
                                                             (append v228 v232 ) ))))) ))

(define (divide1 a b n op)
        (let ((v237 (< a b ) ))
                 (if v237 
                         (let ((v240 (eq? op 0 ) ))
                                  (if v240 
                                          n 
                                          a ) )
                         (let ((v246 (- a b ) ))
                                  (let ((v250 (+ 1 n ) ))
                                           (divide1 v246 b v250 op ) ))) ))

(define (divide a b)
        (divide1 a b 0 0 ) )

(define (modr a b)
        (divide1 a b 0 1 ) )

(define (getLength lst n)
        (let ((v265 (null? lst ) ))
                 (if v265 
                         n 
                         (let ((v268 (cdr lst ) ))
                                  (let ((v271 (+ n 1 ) ))
                                           (getLength v268 v271 ) ))) ))

(define (splitHalf lst len)
        (let ((v276 (eq? 0 len ) ))
                 (if v276 
                         lst 
                         (let ((v279 (cdr lst ) ))
                                  (let ((v282 (- len 1 ) ))
                                           (splitHalf v279 v282 ) ))) ))

(define (filter lst)
        (let ((len (getLength lst 0 ) ))
                 (let ((v291 (divide len 2 ) ))
                          (splitHalf lst v291 ) )))

(define (solve step sx sy bs)
        (let ((v296 (eq? step 1 ) ))
                 (if v296 
                         (let ((v1348 (cons sx sy ) ))
                                  (let ((v301 (cons v1348  () ) ))
                                           (cons v301  () ) ))
                         (let ((v1334 (- step 1 ) ))
                                  (let ((ans (solve v1334 sx sy bs ) ))
                                           (let ((v1321 (modr step 5 ) ))
                                                    (let ((v1315 (eq? 0 v1321 ) ))
                                                             (if v1315 
                                                                     (let ((v318 (filter ans ) ))
                                                                              (nextStep bs v318 ) )
                                                                     (nextStep bs ans ) ) ))))) ))

(define (knightTour boardsize startposx startposy)
        (let ((v328 (* boardsize boardsize ) ))
                 (solve v328 startposx startposy boardsize ) ))

(knightTour 5 3 3 ) 

