(define (fun)
  (let ((a 5))
    (let ((b 10))
      (let ((c (+ a b)))
	(let ((d 0))
	  (if d
	      (let ((x 5))
		(let ((y 7))
		  (let ((z (+ x y)))
		    (let ((res (+ z a)))
		      res))))
	      c))))))

(let ((res (fun)))
   res)


