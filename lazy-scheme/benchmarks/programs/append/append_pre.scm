(define (append ax ay ) 
(if (null? ax ) 
ay 
(cons (car ax ) 
(append (cdr ax ) ay ) ) ) ) 

(append (cons 1 (cons 2 () ) ) (cons 3 () ) ) 
