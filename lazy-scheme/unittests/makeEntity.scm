(define (makeEntity1 n)
        (let ((_var101 2))
	  (let ((f (- n _var101)))
	    (let ((g 1))
	      (let ((g2 (- n g)))
		(let ((g3 n))
		  (let ((g4 ()))
		    (let ((g5 (cons g3 g4)))
		      (let ((g6 (cons g2 g5)))
			(let ((g7 (cons f g6)))
            g7))))))))))

(define (makeEntity n)
        (let ((_var101 2))
	  (let ((_var103 1))
             (let ((f (- n  _var101)))
	       (let ((g (- n  _var103)))
		   (let ((_var105 ()))
		     (let ((_var106 (cons n  _var105)))
		       (let ((_var107 (cons g  _var106)))
			 (let ((_var108 (cons f  _var107)))
			   (let ((h 11))
			   _var108))))))))))
(define (makeEntity2 n)
        (let ((_var101 2))
	  (let ((_var102 1))
	    (let ((g1 (- n _var102)))
	      (let ((g2 (- n _var101)))
		(let ((g3 ()))
		  (let ((g4 (cons n g3)))
		    (let ((g5 (cons g1 g4)))
		      (let ((g6 (cons g2 g5)))
			(let ((g7 (cons f g6)))
            g6))))))))))

(let ((k 5))
  (let ((res (makeEntity k)))
    res))