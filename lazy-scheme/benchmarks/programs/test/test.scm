(define (mapsq l)
  (let ((c (null? l)))
   (if  c
	l
	(let ((x (car l)))
	  (let ((xs (cdr l)))
	    (let ((y (* x x)))
	      (let ((ys (mapsq xs)))
		(let ((res (cons y ys)))
		  res))))))))

(let ((a ()))
  (let ((b (cons 1 a)))
    (let ((c (cons 2 b)))
      (let ((d (cons 3 c)))
	(let ((e (mapsq d)))
	  (let ((f (null? e)))
	    (let ((j (pair? e)))
	    (if j
		(let ((g (mapsq e)))
		  g)
		(let ((h (cons 0 a)))
		  h)))))))))