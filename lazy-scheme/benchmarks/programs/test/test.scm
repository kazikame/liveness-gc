(define (f l)
 (if (null? l) l
     (cons (f (car l)) 2)))

(f ())