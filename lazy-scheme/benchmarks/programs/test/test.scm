(let ((a0 '()))
  (let ((b1 1))
    (let ((a1 (cons b1 a0)))
      (let ((b2 2))
	(let ((a2 (cons b2 a1)))
	  (let ((b3 3))
	    (let ((a3 (cons b3 a2)))
	      (let ((c (null? a3)))
		(if c
		    (let ((num (car a3)))
		      num)
		    (let ((tl (cdr a3)))
		      (let ((hd (car tl)))
			hd)))))))))))
