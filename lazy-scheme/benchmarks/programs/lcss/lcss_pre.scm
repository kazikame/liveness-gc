#lang racket
(define ( or a b)
 (if a a 
 b 
 ))

(define (append ax ay ) 
(if (null? ax ) 
ay 
(cons (car ax ) (append (cdr ax ) ay ) ) ) ) 


(define (divide1 a b n op ) 
(if (< a b ) 
(if (eq? op 0 ) 
n 
a 
) 
(divide1 (- a b ) b (+ 1 n ) op ) 
) 
) 

(define (divide a b ) 
(divide1 a b 0 0 ) 
) 

(define (take n lst ) 
(if (eq? 0 n ) 
'() 
(cons (car lst ) (take (- n 1 ) (cdr lst ) ) ) 
) 
) 
(define (drop n lst ) 
(if (eq? 0 n ) 
lst 
(drop (- n 1 ) (cdr lst ) ) 
) 
) 

(define (reverse lst ) 
(if (null? lst ) 
'() 
(append (reverse (cdr lst ) ) (cons (car lst ) '() ) ) 
) 
) 

(define (zip xs ys ) 
(if (or (null? xs ) (null? ys ) ) 
'() 
(cons (cons (car xs ) (car ys ) ) (zip (cdr xs ) (cdr ys ) ) ) 
) 
) 

(define (getLength lst ) 
(if (null? lst ) 
0 
(+ 1 (getLength (cdr lst ) ) ) 
) 
) 

(define (findk k km m pairs ) 
(if (null? pairs ) 
km 
(let ((s (+ (car (car pairs ) ) (cdr (car pairs ) ) ) ) ) 
(if (< s m ) 
(findk (+ 1 k ) km m (cdr pairs ) ) 
(findk (+ 1 k ) k s (cdr pairs ) ) 
) 
) 
) 
) 
(define (elem n lst ) 
(if (null? lst ) 
0 
(if (eq? (car lst ) n ) 
1 
(elem n (cdr lst ) ) 
) 
) 
) 

(define (zip0 lst ) 
(if (null? lst ) 
'() 
(cons (cons (car lst ) 0 ) (zip0 (cdr lst ) ) ) 
) 
) 

(define (mapsnd pairs ) 
(if (null? pairs ) 
'() 
(cons (cdr (car pairs ) ) (mapsnd (cdr pairs ) ) ) 
) 
) 
(define (max a b ) 
(if (> a b ) 
a 
b 
) 
) 

(define (algb2 k0j1 k1j1 pairs x ) 
(if (null? pairs ) 
'() 
(let ((k0j (cdr (car pairs ) ) ) ) 
(let ((y (car (car pairs ) ) ) ) 
(let ((kjcurr (if (eq? x y ) 
(+ 1 k0j1 ) 
(max k1j1 k0j ) 
) ) ) 
(cons (cons y kjcurr ) (algb2 k0j kjcurr (cdr pairs ) x ) ) 
) ) ) 
) 
) 



(define (algb1 xs pairs ) 
(if (null? xs ) 
(mapsnd pairs ) 
(let ((n (algb2 0 0 pairs (car xs ) ) ) ) 
(algb1 (cdr xs ) n ) 
) 
) 
) 

(define (algb xs ys ) 
(cons 0 (algb1 xs (zip0 ys ) ) ) 

) 


(define (algc m n xs ys lst ) 
(if (null? ys ) 
lst 
(if (eq? 1 (getLength xs ) ) 
(if (eq? 1 (elem (car xs ) ys ) ) 
(cons (car xs ) lst ) 
lst 
) 
(let ((m2 (divide m 2 ) ) ) 
(let ((xs1 (take m2 xs ) ) ) 
(let ((xs2 (drop m2 xs ) ) ) 
(let ((l1 (algb xs1 ys ) ) ) 
(let ((l2 (reverse (algb (reverse xs2 ) (reverse ys ) ) ) ) ) 
(let ((k (findk 0 0 (- 0 1 ) (zip l1 l2 ) ) ) ) 
(let ((ex1 (algc (- m m2 ) (- n k ) xs2 (drop k ys ) lst ) ) ) 
(algc m2 k xs1 (take k ys ) ex1 ) ) ) ) ) ) ) ) 
) ) 
) 

(define (lcss xs ys ) 
(algc (getLength xs ) (getLength ys ) xs ys '() ) 
) 

(define (makeList n ) 
(if (eq? 0 n ) 
	 '() 
	 (cons n (makeList (- n 1 ) ) ) 
) 
) 


(define  (run-in-loop a b n)
  (if (= n 0) '()
      (cons (lcss a b) (run-in-loop a b (- n 1)))))


(let ((a (makeList 1000 ) ) ) 
	 (let ((b (makeList 50 ) ) ) 
	 (run-in-loop a b 100)
) 
) 
