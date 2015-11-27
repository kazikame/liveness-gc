#lang racket

;;; PARAFFINS -- Compute how many paraffins exist with N carbon atoms.
(define (quotient n k)
  (if (< n k) 0
      (+ 1 (quotient (- n k) k))))

(define (make-vector k val)
  (if (= k 0) '()
      (cons val (make-vector (- k 1) val))))

(define (vector-ref v k)
  (if (= k 0) (car v)
      (vector-ref (cdr v) (- k 1))))

(define (vector-1 val)
  (cons val '()))

(define (vector-2 val1 val2)
  (cons val1 (vector-1 val2)))

(define (vector-3 val1 val2 val3)
  (cons val1 (vector-2 val2 val3)))

(define (vector-4 val1 val2 val3 val4)
  (cons val1 (vector-3 val2 val3 val4)))

(define (vector-5 val1 val2 val3 val4 val5)
  (cons val1 (vector-4 val2 val3 val4 val5)))

(define (vector-set! vec i val)
  (if (= i 0) (cons val (cdr vec))
      (cons (car vec) (vector-set! (cdr vec) (- i 1) val))))


(define (gen n)
  (let* ((n/2 (quotient n 2))
         (radicals (make-vector (+ n/2 1) '(H))))

    (define (rads-of-size n radicals)
      (let loop1 ((ps
                   (three-partitions (- n 1)))
                  (lst
                   '()))
        (if (null? ps)
          lst
          (let* ((p (car ps))
                 (nc1 (vector-ref p 0))
                 (nc2 (vector-ref p 1))
                 (nc3 (vector-ref p 2)))
            (let loop2 ((rads1
                         (vector-ref radicals nc1))
                        (lst
                         (loop1 (cdr ps)
                                lst)))
              (if (null? rads1)
                lst
                (let loop3 ((rads2
                             (if (= nc1 nc2)
                               rads1
                               (vector-ref radicals nc2)))
                            (lst
                             (loop2 (cdr rads1)
                                    lst)))
                  (if (null? rads2)
                    lst
                    (let loop4 ((rads3
                                 (if (= nc2 nc3)
                                   rads2
                                   (vector-ref radicals nc3)))
                                (lst
                                 (loop3 (cdr rads2)
                                        lst)))
                      (if (null? rads3)
                        lst
                        (cons (vector-4 'C
                                      (car rads1)
                                      (car rads2)
                                      (car rads3))
                              (loop4 (cdr rads3)
                                     lst))))))))))))

    (define (bcp-generator j radicals)
      (if (odd? j)
        '()
        (let loop1 ((rads1
                     (vector-ref radicals (quotient j 2)))
                    (lst
                     '()))
          (if (null? rads1)
            lst
            (let loop2 ((rads2
                         rads1)
                        (lst
                         (loop1 (cdr rads1)
                                lst)))
              (if (null? rads2)
                lst
                (cons (vector-3 'BCP
                              (car rads1)
                              (car rads2))
                      (loop2 (cdr rads2)
                             lst))))))))

    (define (ccp-generator j radicals)
      (let loop1 ((ps
                   (four-partitions (- j 1)))
                  (lst
                   '()))
        (if (null? ps)
          lst
          (let* ((p (car ps))
                 (nc1 (vector-ref p 0))
                 (nc2 (vector-ref p 1))
                 (nc3 (vector-ref p 2))
                 (nc4 (vector-ref p 3)))
            (let loop2 ((rads1
                         (vector-ref radicals nc1))
                        (lst
                         (loop1 (cdr ps)
                                lst)))
              (if (null? rads1)
                lst
                (let loop3 ((rads2
                             (if (= nc1 nc2)
                               rads1
                               (vector-ref radicals nc2)))
                            (lst
                             (loop2 (cdr rads1)
                                    lst)))
                  (if (null? rads2)
                    lst
                    (let loop4 ((rads3
                                 (if (= nc2 nc3)
                                   rads2
                                   (vector-ref radicals nc3)))
                                (lst
                                 (loop3 (cdr rads2)
                                        lst)))
                      (if (null? rads3)
                        lst
                        (let loop5 ((rads4
                                     (if (= nc3 nc4)
                                       rads3
                                       (vector-ref radicals nc4)))
                                    (lst
                                     (loop4 (cdr rads3)
                                            lst)))
                          (if (null? rads4)
                            lst
                            (cons (vector-5 'CCP
                                          (car rads1)
                                          (car rads2)
                                          (car rads3)
                                          (car rads4))
                                  (loop5 (cdr rads4)
                                         lst))))))))))))))

    (let loop ((i 1))
      (if (> i n/2)
        (vector-2 (bcp-generator n radicals)
                (ccp-generator n radicals))
        (begin
          (vector-set! radicals i (rads-of-size i radicals))
          (loop (+ i 1)))))
;    (my-loop 1 n radicals)
    ))



;(define (rads-of-size n radicals)
;      (let loop1 ((ps
;                   (three-partitions (- n 1)))
;                  (lst
;                   '()))
;        (if (null? ps)
;          lst
;          (let* ((p (car ps))
;                 (nc1 (vector-ref p 0))
;                 (nc2 (vector-ref p 1))
;                 (nc3 (vector-ref p 2)))
;            (let loop2 ((rads1
;                         (vector-ref radicals nc1))
;                        (lst
;                         (loop1 (cdr ps)
;                                lst)))
;              (if (null? rads1)
;                lst
;                (let loop3 ((rads2
;                             (if (= nc1 nc2)
;                               rads1
;                               (vector-ref radicals nc2)))
;                            (lst
;                             (loop2 (cdr rads1)
;                                    lst)))
;                  (if (null? rads2)
;                    lst
;                    (let loop4 ((rads3
;                                 (if (= nc2 nc3)
;                                   rads2
;                                   (vector-ref radicals nc3)))
;                                (lst
;                                 (loop3 (cdr rads2)
;                                        lst)))
;                      (if (null? rads3)
;                        lst
;                        (cons (vector-4 'C
;                                      (car rads1)
;                                      (car rads2)
;                                      (car rads3))
;                              (loop4 (cdr rads3)
;                                     lst))))))))))))
;
;
;(define (bcp-generator j radicals rads1 rads2 lst)
;      (if (odd? j)
;        '()
;        (let loop1 ((rads1
;                     (vector-ref radicals (quotient j 2)))
;                    (lst
;                     '()))
;          (if (null? rads1)
;            lst
;            (let loop2 ((rads2
;                         rads1)
;                        (lst
;                         (loop1 (cdr rads1)
;                                lst)))
;              (if (null? rads2)
;                lst
;                (cons (vector-3 'BCP
;                              (car rads1)
;                              (car rads2))
;                      (loop2 (cdr rads2)
;                             lst))))))))
;
;(define (ccp-generator j radicals rads1 rads2 rads3 rads4 lst)
;      (let loop1 ((ps
;                   (four-partitions (- j 1)))
;                  (lst
;                   '()))
;        (if (null? ps)
;          lst
;          (let* ((p (car ps))
;                 (nc1 (vector-ref p 0))
;                 (nc2 (vector-ref p 1))
;                 (nc3 (vector-ref p 2))
;                 (nc4 (vector-ref p 3)))
;            (let loop2 ((rads1
;                         (vector-ref radicals nc1))
;                        (lst
;                         (loop1 (cdr ps)
;                                lst)))
;              (if (null? rads1)
;                lst
;                (let loop3 ((rads2
;                             (if (= nc1 nc2)
;                               rads1
;                               (vector-ref radicals nc2)))
;                            (lst
;                             (loop2 (cdr rads1)
;                                    lst)))
;                  (if (null? rads2)
;                    lst
;                    (let loop4 ((rads3
;                                 (if (= nc2 nc3)
;                                   rads2
;                                   (vector-ref radicals nc3)))
;                                (lst
;                                 (loop3 (cdr rads2)
;                                        lst)))
;                      (if (null? rads3)
;                        lst
;                        (let loop5 ((rads4
;                                     (if (= nc3 nc4)
;                                       rads3
;                                       (vector-ref radicals nc4)))
;                                    (lst
;                                     (loop4 (cdr rads3)
;                                            lst)))
;                          (if (null? rads4)
;                            lst
;                            (cons (vector-5 'CCP
;                                          (car rads1)
;                                          (car rads2)
;                                          (car rads3)
;                                          (car rads4))
;                                  (loop5 (cdr rads4)
;                                         lst))))))))))))))
;
;
;
;(define (my-loop i n radicals)
;  (if (> i (/ n 2))
;      (vector-2 (bcp-generator n radicals)
;                (ccp-generator n radicals ))
;      (let ((new-rad (vector-set! radicals i (rads-of-size i radicals))))
;        (my-loop (+ i 1) n new-rad))))

(define (three-partitions m)
  (let loop1 ((lst '())
              (nc1 (quotient m 3)))
    (if (< nc1 0)
      lst
      (let loop2 ((lst lst)
                  (nc2 (quotient (- m nc1) 2)))
        (if (< nc2 nc1)
          (loop1 lst
                 (- nc1 1))
          (loop2 (cons (vector-3 nc1 nc2 (- m (+ nc1 nc2))) lst)
                 (- nc2 1)))))))

(define (four-partitions m)
  (let loop1 ((lst '())
              (nc1 (quotient m 4)))
    (if (< nc1 0)
      lst
      (let loop2 ((lst lst)
                  (nc2 (quotient (- m nc1) 3)))
        (if (< nc2 nc1)
          (loop1 lst
                 (- nc1 1))
          (let ((start (max nc2 (- (quotient (+ m 1) 2) (+ nc1 nc2)))))
            (let loop3 ((lst lst)
                        (nc3 (quotient (- m (+ nc1 nc2)) 2)))
              (if (< nc3 start)
                (loop2 lst (- nc2 1))
                (loop3 (cons (vector-4 nc1 nc2 nc3 (- m (+ nc1 (+ nc2 nc3)))) lst)
                       (- nc3 1))))))))))

(define (nb n)
  (let ((x (gen n)))
    (+ (length (vector-ref x 0))
       (length (vector-ref x 1)))))

(let ((input 1))
  (time
   (let loop ((n 100) (v 0))
     (if (zero? n)
         v
         (loop (- n 1) (nb (if input 17 0)))))))
