(define (or a b)
        (if a 
               a 
               b ) )

(define (divide a b)
        (divide1 a b 0 ) )

(define (divide1 a b i)
        (let ((v10 (< a b ) ))
                (if v10 
                       i 
                       (let ((v14 (- a b ) ))
                               (let ((v18 (+ i 1 ) ))
                                       (divide1 v14 b v18 ) ))) ))

(define (Node left right)
        (cons left right ) )

(define (Power2 i)
        (let ((v26 (eq? 1 i ) ))
                (if v26 
                       2 
                       (let ((v0 (- i 1 ) ))
                               (let ((v32 (Power2 v0 ) ))
                                       (* 2 v32 ) ))) ))

(define (TreeSize i)
        (let ((v38 (let ((v37 (+ i 1 ) ))
                             (Power2 v37 ) )))
                (- v38 1 ) ))

(define (NumIters i kStretchTreeDepth)
        (let ((v46 (let ((v43 (TreeSize kStretchTreeDepth ) ))
                             (let ((v45 (TreeSize i ) ))
                                     (divide v43 v45 ) ))))
                (* 2 v46 ) ))

(define (MakeTree iDepth)
        (let ((v50 (eq? 0 iDepth ) ))
                (if v50 
                       (Node  ()  () ) 
                       (let ((v257 (- iDepth 1 ) ))
                               (let ((v57 (MakeTree v257 ) ))
                                       (let ((v247 (- iDepth 1 ) ))
                                               (let ((v61 (MakeTree v247 ) ))
                                                       (Node v57 v61 ) ))))) ))

(define (BottomUp depth)
        (MakeTree depth ) )

(define (LoopBottomUp i depth)
        (let ((v68 (eq? i 0 ) ))
                (if v68 
                        () 
                       (let ((tempTree (MakeTree depth ) ))
                               (let ((tempTree  () ))
                                       (let ((v75 (- i 1 ) ))
                                               (LoopBottomUp v75 depth ) )))) ))

(define (TimeConstruction depth kStretchTreeDepth)
        (let ((iNumIters (NumIters depth kStretchTreeDepth ) ))
                (LoopBottomUp iNumIters depth ) ))

(define (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)
        (let ((v90 (> kMinTreeDepth kMaxTreeDepth ) ))
                (if v90 
                        () 
                       (let ((iter (TimeConstruction kMinTreeDepth kStretchTreeDepth ) ))
                               (let ((v97 (+ kMinTreeDepth 2 ) ))
                                       (loopTimeConstruction v97 kMaxTreeDepth kStretchTreeDepth ) ))) ))

(define (createArray kArraySize i)
        (let ((v105 (eq? i kArraySize ) ))
                (if v105 
                       (cons  ()  () ) 
                       (let ((v337 (divide kArraySize 2 ) ))
                               (let ((v318 (> i v337 ) ))
                                       (if v318 
                                              (let ((v322 (+ i 1 ) ))
                                                      (let ((v119 (createArray kArraySize v322 ) ))
                                                              (cons  () v119 ) ))
                                              (let ((v329 (+ i 1 ) ))
                                                      (let ((v126 (createArray kArraySize v329 ) ))
                                                              (cons i v126 ) ))) ))) ))

(define (checkArray array index)
        (let ((v132 (eq? index 1 ) ))
                (if v132 
                       (car array ) 
                       (let ((v136 (cdr array ) ))
                               (let ((v139 (- index 1 ) ))
                                       (checkArray v136 v139 ) ))) ))

(define (main kStretchTreeDepth kLongLivedTreeDepth kArraySize kMinTreeDepth kMaxTreeDepth)
        (let ((tempTree (MakeTree kStretchTreeDepth ) ))
                (let ((tempTree  () ))
                        (let ((longLivedTree (MakeTree kLongLivedTreeDepth ) ))
                                (let ((array (createArray kArraySize 1 ) ))
                                        (let ((loopTC (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth ) ))
                                                (let ((v411 (null? longLivedTree ) ))
                                                        (let ((v423 (checkArray array 1 ) ))
                                                                (let ((v433 (eq? v423 1 ) ))
                                                                        (let ((v441 (eq? #f v433 ) ))
                                                                                (let ((v406 (or v411 v441 ) ))
                                                                                        (if v406 
                                                                                                () 
                                                                                               (cons 100 100 ) ) )))))))))))

(main 16 10 5 4 10 ) 

