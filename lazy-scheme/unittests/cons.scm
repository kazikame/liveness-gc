(define (f a b)
  (let ((res (cons a b)))
    res))

(let ((a ()))
  (let ((b 1))
    (let ((c 2))
      (let ((d 3))
	(let ((e (f b a)))
	  (let ((g (f c e)))
	    (let ((h (f d g)))
	      h)))))))
     