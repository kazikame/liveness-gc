(define (fun l1 l2 l3)
  (let ((one 1))
    (let ((l (cons one l1)))
      (let ((c (null? l)))
	(if c
	    (let ((ret1 (car l)))
	      (let ((ret (cons ret1 l2)))
	      ret))
	    (let ((ans1 1))
	      (let ((ans (cons ans1 l3)))
	      ans)))))))


(let ((a0 '()))
  (let ((b1 1))
    (let ((a1 (cons b1 a0)))
      (let ((b2 2))
	(let ((a2 (cons b2 a1)))
	  (let ((b3 3))
	    (let ((a3 (cons b3 a2)))
	      (let ((c (null? a3)))
		(if c
		    (let ((tl (cdr a3)))
		      (let ((hd (car tl)))
			hd))
		    (let ((num (fun a3 a1 a0)))
		      num))))))))))
