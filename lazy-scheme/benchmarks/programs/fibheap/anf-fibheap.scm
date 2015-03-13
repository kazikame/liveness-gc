(define (and a b)
        (if a 
               (if b 
                      #t 
                      #f ) 
               #f ) )

(define (divide1 a b n op)
        (let ((v9 (< a b ) ))
                (if v9 
                       (let ((v12 (eq? op 0 ) ))
                               (if v12 
                                      n 
                                      a ) )
                       (let ((v18 (- a b ) ))
                               (let ((v22 (+ 1 n ) ))
                                       (divide1 v18 b v22 op ) ))) ))

(define (modr a b)
        (divide1 a b 0 1 ) )

(define (append ax ay)
        (let ((v32 (null? ax ) ))
                (if v32 
                       ay 
                       (let ((v35 (car ax ) ))
                               (let ((v0 (cdr ax ) ))
                                       (let ((v39 (append v0 ay ) ))
                                               (cons v35 v39 ) )))) ))

(define (insert x heap)
        (let ((v405 (cons 0  () ) ))
                (let ((newnode (cons x v405 ) ))
                        (let ((v48 (null? heap ) ))
                                (if v48 
                                       (cons newnode  () ) 
                                       (cons newnode heap ) ) ))))

(define (updateMin curmin heap)
        (let ((v456 (car curmin ) ))
                (let ((v430 (car v456 ) ))
                        (let ((v447 (car heap ) ))
                                (let ((v440 (car v447 ) ))
                                        (let ((v425 (< v430 v440 ) ))
                                                (if v425 
                                                       curmin 
                                                       heap ) ))))))

(define (addChild root node)
        (let ((v494 (car root ) ))
                (let ((v486 (cdr v494 ) ))
                        (let ((ptchild (cdr v486 ) ))
                                (let ((v473 (car node ) ))
                                        (let ((v75 (cons v473  () ) ))
                                                (append ptchild v75 ) ))))))

(define (updateArray nchild root array)
        (let ((v79 (null? array ) ))
                (if v79 
                       (let ((v81 (car root ) ))
                               (cons v81  () ) )
                       (let ((v872 (car array ) ))
                               (let ((v839 (cdr v872 ) ))
                                       (let ((achild (car v839 ) ))
                                               (let ((v90 (eq? nchild achild ) ))
                                                       (if v90 
                                                              (let ((v761 (car root ) ))
                                                                      (let ((v687 (car v761 ) ))
                                                                              (let ((v736 (car array ) ))
                                                                                      (let ((v713 (car v736 ) ))
                                                                                              (let ((v666 (< v687 v713 ) ))
                                                                                                      (if v666 
                                                                                                             (let ((ptchild (addChild root array ) ))
                                                                                                                     (let ((v568 (car root ) ))
                                                                                                                             (let ((v539 (car v568 ) ))
                                                                                                                                     (let ((v558 (+ 1 nchild ) ))
                                                                                                                                             (let ((v550 (cons v558 ptchild ) ))
                                                                                                                                                     (let ((v525 (cons v539 v550 ) ))
                                                                                                                                                             (let ((v532 (cdr array ) ))
                                                                                                                                                                     (let ((newnode (cons v525 v532 ) ))
                                                                                                                                                                             newnode ))))))))
                                                                                                             (let ((ptchild (addChild array root ) ))
                                                                                                                     (let ((v642 (car array ) ))
                                                                                                                             (let ((v613 (car v642 ) ))
                                                                                                                                     (let ((v632 (+ 1 achild ) ))
                                                                                                                                             (let ((v624 (cons v632 ptchild ) ))
                                                                                                                                                     (let ((v599 (cons v613 v624 ) ))
                                                                                                                                                             (let ((v606 (cdr array ) ))
                                                                                                                                                                     (let ((newnode (cons v599 v606 ) ))
                                                                                                                                                                             newnode ))))))))) )))))
                                                              (let ((v136 (car array ) ))
                                                                      (let ((v823 (cdr array ) ))
                                                                              (let ((v141 (updateArray nchild root v823 ) ))
                                                                                      (cons v136 v141 ) )))) ))))) ))

(define (getLen root)
        (let ((v147 (null? root ) ))
                (if v147 
                       0 
                       (let ((v953 (cdr root ) ))
                               (let ((v152 (getLen v953 ) ))
                                       (+ 1 v152 ) ))) ))

(define (calculateM root min)
        (let ((v156 (null? root ) ))
                 (if v156 
                         min 
                         (let ((v1005 (car root ) ))
                                  (let ((m (car v1005 ) ))
                                           (let ((v991 (car min ) ))
                                                    (let ((v982 (car v991 ) ))
                                                             (let ((v975 (< m v982 ) ))
                                                                      (if v975 
                                                                              (let ((v167 (cdr root ) ))
                                                                                       (calculateM v167 root ) )
                                                                              (let ((v171 (cdr root ) ))
                                                                                       (calculateM v171 min ) )) )))))) ))

(define (calculateMin root)
        (let ((v178 (cdr root ) ))
                 (calculateM v178 root ) ))

(define (merge2 root array)
        (let ((v182 (null? root ) ))
                 (if v182 
                         array 
                         (let ((v1054 (car root ) ))
                                  (let ((v1046 (cdr v1054 ) ))
                                           (let ((nchild (car v1046 ) ))
                                                    (let ((narray (updateArray nchild root array ) ))
                                                             (let ((v193 (cdr root ) ))
                                                                      (merge2 v193 narray ) )))))) ))

(define (merge1 root curlen)
        (let ((newroot (merge2 root  () ) ))
                 (let ((lenheap (getLen newroot ) ))
                          (let ((v206 (eq? lenheap curlen ) ))
                                   (if v206 
                                           newroot 
                                           (merge1 newroot lenheap ) ) ))))

(define (merge root)
        (let ((curlen (getLen root ) ))
                 (merge1 root curlen ) ))

(define (children_to_rootlist node n)
        (let ((v222 (eq? 0 n ) ))
                 (if v222 
                          () 
                         (let ((v225 (car node ) ))
                                  (let ((v1100 (cdr node ) ))
                                           (let ((v1107 (- n 1 ) ))
                                                    (let ((v231 (children_to_rootlist v1100 v1107 ) ))
                                                             (cons v225 v231 ) ))))) ))

(define (deleteAndJoinChildren node heap)
        (let ((v1202 (car node ) ))
                 (let ((v1189 (cdr v1202 ) ))
                          (let ((nchildren (car v1189 ) ))
                                   (let ((v240 (eq? 0 nchildren ) ))
                                            (if v240 
                                                    (cdr heap ) 
                                                    (let ((v1145 (car node ) ))
                                                             (let ((v1155 (cdr v1145 ) ))
                                                                      (let ((v1163 (cdr v1155 ) ))
                                                                               (let ((chlst (children_to_rootlist v1163 nchildren ) ))
                                                                                        (let ((v251 (cdr heap ) ))
                                                                                                 (append chlst v251 ) )))))) )))))

(define (delete curmin heap)
        (let ((v257 (null? heap ) ))
                 (if v257 
                          () 
                         (let ((v1286 (car curmin ) ))
                                  (let ((v1251 (car v1286 ) ))
                                           (let ((v1274 (car heap ) ))
                                                    (let ((v1264 (car v1274 ) ))
                                                             (let ((v1238 (eq? v1251 v1264 ) ))
                                                                      (if v1238 
                                                                              (deleteAndJoinChildren curmin heap ) 
                                                                              (let ((v270 (car heap ) ))
                                                                                       (let ((v1242 (cdr heap ) ))
                                                                                                (let ((v274 (delete curmin v1242 ) ))
                                                                                                         (cons v270 v274 ) )))) )))))) ))

(define (deleteMin curmin heap)
        (let ((step1 (delete curmin heap ) ))
                 (let ((step2 (merge step1 ) ))
                          step2 )))

(define (testdelete i min heap ret)
        (let ((nheap (deleteMin min heap ) ))
                 (let ((v291 (eq? 1 i ) ))
                          (if v291 
                                  ret 
                                  (let ((nmin (calculateMin nheap ) ))
                                           (let ((v297 (- i 1 ) ))
                                                    (let ((v302 (cons nmin ret ) ))
                                                             (testdelete v297 nmin nheap v302 ) )))) )))

(define (testinsert i min heap)
        (let ((v309 (eq? 0 i ) ))
                 (if v309 
                         (cons min heap ) 
                         (let ((v1367 (* i 5 ) ))
                                  (let ((v1378 (+ i 5 ) ))
                                           (let ((v1387 (modr v1367 v1378 ) ))
                                                    (let ((nheap (insert v1387 heap ) ))
                                                             (let ((nmin (updateMin min nheap ) ))
                                                                      (let ((v327 (- i 1 ) ))
                                                                               (testinsert v327 nmin nheap ) ))))))) ))

(define (test n)
        (let ((v1422 (insert 100000  () ) ))
                 (let ((ins (testinsert n v1422  () ) ))
                          (let ((v342 (car ins ) ))
                                   (let ((v344 (cdr ins ) ))
                                            (testdelete n v342 v344  () ) )))))

(test 1000 ) 

