(define (add a b)
  (let ((c (> a b)))
	(if c 
	    (let ((n ()))
		(let ((r (cons  b n)))
		  (let ((res (cons a r)))
		    res)))
	b)))

(define (mapsq l)
 (let 
     ((cond (null? l)))
   (if cond
       (let ((res ()))
	 res)
       (let ((x (car l)))
	 (let ((xsq (* x x)))
	   (let ((ys (cdr l)))
	     (let ((xs (mapsq ys)))
	       (let ((res (cons xsq xs)))
		 res))))))))

	 
(define (f a b)
  (let ((res (cons a b)))
    res))

(let ((x  5))
    (let ((y ()))
      (let ((y1 (/ x  x)))
	 (let ((z1  (cons x y)))
	     (let ((z2 (f x z1)))
	       (let ((res (mapsq z2)))
		 (let ((res1 (car res)))
		   (let ((res2 (cons res res)))
		   res2))))))))

