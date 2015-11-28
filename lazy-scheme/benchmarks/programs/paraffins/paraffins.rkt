#lang racket
;;; PARAFFINS -- Compute how many paraffins exist with N carbon atoms.
(define (remainder n k)
  (if (< n k) n
      (remainder (- n k) k)))

(define (odd? n)
  (= (remainder n 2) 1))

(define (max m n)
  (if (> m n) m n))

(define (length l)
  (if (null? l) 0
      (+ 1 (length (cdr l)))))

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

;; (define vector-1 vector)
;; (define vector-2 vector)
;; (define vector-3 vector)
;; (define vector-4 vector)
;; (define vector-5 vector)

(define (gen n)
  (let ((n/2 (quotient n 2)))
    (let ((radicals (make-vector (+ n/2 1) (cons "H" '()))))
      (gen-radicals 1 n radicals))))

(define (gen-radicals i n radicals)
  (let ((n/2 (quotient n 2)))
    (if (> i n/2)
	(vector-2 (bcp-generator n radicals)
		  (ccp-generator n radicals))
	(let ((new-rads (vector-set! radicals i
				     (rads-of-size i radicals))))
	  (gen-radicals (+ 1 i) n new-rads)))))




(define (myloop4 rads1 rads2 rads3 lst)
  (if (null? rads3)
      lst
      (cons (vector-4 "C"
		      (car rads1)
		      (car rads2)
		      (car rads3))
	    (myloop4 rads1 rads2 (cdr rads3) lst))))


(define (myloop3 nc2 nc3 radicals rads1 rads2 lst)
  (if (null? rads2)
      lst
      (myloop4 rads1 rads2
	       (if (= nc2 nc3)
		   rads2
		   (vector-ref radicals nc3))
	       (myloop3 nc2 nc3 radicals rads1 (cdr rads2) lst))))


(define (myloop2 nc1 nc2 nc3 radicals rads1 lst)
  (if (null? rads1)
      lst
      (myloop3 nc2 nc3 radicals rads1 
	       (if (= nc1 nc2) rads1
		   (vector-ref radicals nc2))
	       (myloop2 nc1 nc2 nc3 radicals (cdr rads1) lst))))

(define (myloop1 radicals ps lst)
  (if (null? ps)
      lst
      (let ((p (car ps)))
	(let ((nc1 (vector-ref p 0)))
	  (let ((nc2 (vector-ref p 1)))
	    (let ((nc3 (vector-ref p 2)))
	      (myloop2 nc1 nc2 nc3 radicals 
		       (vector-ref radicals nc1)
		       (myloop1 radicals (cdr ps) lst))))))))

(define (rads-of-size n radicals)  	
  (myloop1 radicals (three-partitions (- n 1)) '()))


(define (bcp-loop1 rads1 lst)
  (if (null? rads1)
      lst
      (bcp-loop2 rads1 rads1
		 (bcp-loop1 (cdr rads1) lst))))


(define (bcp-loop2 rads1 rads2 lst)
  (if (null? rads2)
      lst
      (cons (vector-3 "BCP"
		      (car rads1)
		      (car rads2))
	    (bcp-loop2 rads1 (cdr rads2) lst))))
  

(define (bcp-generator j radicals)
  (if (odd? j)
      '()
      (bcp-loop1 (vector-ref radicals (quotient j 2)) '())))

(define (ccp-loop1 radicals ps lst)
  (if (null? ps)
      lst
      (let ((p (car ps)))
	(let ((nc1 (vector-ref p 0)))
	  (let ((nc2 (vector-ref p 1)))
	    (let ((nc3 (vector-ref p 2)))
	      (let ((nc4 (vector-ref p 3)))
		(ccp-loop2 nc1 nc2 nc3 nc4 radicals
			   (vector-ref radicals nc1)
			   (ccp-loop1 radicals (cdr ps) lst)))))))))


(define (ccp-loop2 nc1 nc2 nc3 nc4 radicals rads1 lst)
  (if (null? rads1)
      lst
      (ccp-loop3 nc2 nc3 nc4 radicals rads1
		 (if (= nc1 nc2)
		     rads1
		     (vector-ref radicals nc2))
		 (ccp-loop2 nc1 nc2 nc3 nc4 radicals (cdr rads1) lst))))

(define (ccp-loop3 nc2 nc3 nc4 radicals rads1 rads2 lst)
  (if (null? rads2)
      lst
      (ccp-loop4 nc3 nc4 radicals rads1 rads2 
		 (if (= nc2 nc3)
		     rads2
		     (vector-ref radicals nc3))
		 (ccp-loop3 nc2 nc3 nc4 radicals rads1 (cdr rads2) lst))))

(define (ccp-loop4 nc3 nc4 radicals rads1 rads2 rads3 lst)
  (if (null? rads3)
      lst
      (ccp-loop5 rads1 rads2 rads3
		 (if (= nc3 nc4)
		     rads3
		     (vector-ref radicals nc4))
		 (ccp-loop4 nc3 nc4 radicals rads1 rads2 (cdr rads3) lst))))

(define (ccp-loop5 rads1 rads2 rads3 rads4 lst)
  (if (null? rads4)
      lst
      (cons (vector-5 "CCP"
		      (car rads1)
		      (car rads2)
		      (car rads3)
		      (car rads4))
	    (ccp-loop5 rads1 rads2 rads3 (cdr rads4) lst))))

(define (ccp-generator j radicals)
  (ccp-loop1 radicals (four-partitions (- j 1)) '()))


(define (tp-loop2 m nc1 lst nc2 )
  (if (< nc2 nc1)
      (tp-loop1 m lst (- nc1 1))
      (tp-loop2 m nc1 
		(cons (vector-3 nc1 nc2 (- m (+ nc1 nc2))) lst)
		(- nc2 1))))

(define (tp-loop1 m lst nc1)
  (if (< nc1 0)
      lst
      (tp-loop2 m nc1 lst (quotient (- m nc1) 2))))

(define (three-partitions m)
  (tp-loop1 m '() (quotient m 3)))


(define (fp-loop3 m nc1 nc2 start lst nc3)
  (if (< nc3 start)
      (fp-loop2 m nc1 lst (- nc2 1))
      (fp-loop3 m nc1 nc2 start
		(cons (vector-4 nc1 nc2 nc3 (- m (+ nc1 (+ nc2 nc3)))) lst)
		(- nc3 1))))

(define (fp-loop2 m nc1 lst nc2)
  (if (< nc2 nc1)
      (fp-loop1 m lst (- nc1 1))
      (let ((start (max nc2 (- (quotient (+ m 1) 2) (+ nc1 nc2)))))
	(fp-loop3 m nc1 nc2 start 
		  lst (quotient (- m (+ nc1 nc2)) 2)))))

(define (fp-loop1 m lst nc1)
    (if (< nc1 0)
	lst
	(fp-loop2 m nc1 lst (quotient (- m nc1) 3))))

(define (four-partitions m)
  (fp-loop1 m '() (quotient m 4)))

(define (nb n)
  (let ((x (gen n)))
    (+ (length (vector-ref x 0))
       (length (vector-ref x 1)))))


(nb 17)
