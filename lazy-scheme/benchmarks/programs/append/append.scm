(define (sq x) (* x x))

(define (double x) (+ x x))

(define (append ax ay)
   (if (null? ax)
       ay
       (cons (car ax)
             (append (cdr ax) ay))))

(append (cons 1 (cons (sq (double 2)) ())) (cons 3 ()))
