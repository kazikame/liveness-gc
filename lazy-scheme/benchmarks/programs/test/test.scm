(define (makelist)
  (let ((a ()))
    (let ((a1 1))
      (let ((b2 2))
	(let ((c3 3))
	  (let ((a0 0))
	    (let ((b (cons a1 a)))
	      (let ((c (cons b2 b)))
		(let ((d (cons c3 c)))
		  d)))))))))

(define (loop i)
  (let ((j 0))
    (let ((c (eq? i j)))
      (if c
	  (let ((res '()))
	    res)
	  (let ((hd (makelist)))
	    (let ((one 1))
	      (let ((k (- i one)))
		(let ((tl (loop k)))
		  (let ((ret (cons hd tl)))
		    ret)))))))))

(let ((num 100000))
  (let ((ans (loop num)))
    ans))

	  
