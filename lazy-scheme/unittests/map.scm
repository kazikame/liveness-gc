(define (mapsq l)
  (let ((cond (null? l)))
    (if cond
	(let ((res ()))
	  res)
	(let ((x (car l)))
	  (let ((y (* x x)))
	    (let ((xs (cdr l)))
	      (let ((ys (mapsq xs)))
		(let ((res (cons y ys)))
		  res))))))))

(let ((a ()))
  (let ((b 1))
    (let ((c 2))
      (let ((d (cons b a)))
	 (let ((e (cons c d)))
	   (let ((f 5))
	     (let ((g (cons f e)))
	       (let ((res (mapsq g)))
		 res))))))))