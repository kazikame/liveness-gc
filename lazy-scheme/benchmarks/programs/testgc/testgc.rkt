#lang racket
(define (and anx any) (if (eq? anx #t) any #f))

(define (or orx ory) (if (eq? orx #t) orx ory))

(define (divide a b)
  (divide1 a b 0)
)

(define (divide1 a b i)
  (if (< a b)
        i
	(divide1 (- a b) b (+ i 1) )
  )
)

(define (Node left right)
  (cons left right)
)

(define (Power2 i)
  (if (eq? 1 i)
	2
	(* 2 (Power2 (- i 1)))
       )
)


(define (TreeSize i)
   (- (Power2 (+ i 1)) 1)

)

(define (NumIters i kStretchTreeDepth)
  (* 2 (divide (TreeSize kStretchTreeDepth) (TreeSize i)) )
)


(define (MakeTree iDepth )
  (if (eq? 0 iDepth )
	(Node '() '())   
	(Node (MakeTree (- iDepth 1)) (MakeTree (- iDepth 1)) ) 
  )
)




(define (BottomUp depth)
  (MakeTree depth)
)


(define (LoopBottomUp i depth)
  (if (eq? i 0)
	'() 
	(let ((tempTree (MakeTree depth)))  
		(let ((tempTree '() ))
			(LoopBottomUp (- i 1) depth)
		)
	)
  )
)
		

(define (TimeConstruction depth kStretchTreeDepth)
  (let ((iNumIters (NumIters depth kStretchTreeDepth)))
	(LoopBottomUp iNumIters depth)
   )
) 


(define (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)
  (if (> kMinTreeDepth kMaxTreeDepth)
	'()
	(let ((iter (TimeConstruction kMinTreeDepth kStretchTreeDepth) ))
		(loopTimeConstruction (+ kMinTreeDepth 2) kMaxTreeDepth kStretchTreeDepth)
	)
  )
)


(define (createArray kArraySize i)
  (if (eq? i kArraySize)
      (cons '() '() )
      (if (> i (divide kArraySize 2) )
	 (cons '() (createArray kArraySize (+ i 1) ) )  
	 (cons  i (createArray kArraySize (+ i 1) ) )  
      )
  )
)


(define (checkArray array index)
  (if (eq? index 1)
      (car array)
      (checkArray (cdr array) (- index 1))
  )
)
  

(define (main kStretchTreeDepth kLongLivedTreeDepth kArraySize kMinTreeDepth kMaxTreeDepth)
  (let ((tempTree (MakeTree kStretchTreeDepth) ))    
    (let ((tempTree '() ))
      (let ((longLivedTree (MakeTree kLongLivedTreeDepth) ))  
	(let (( array (createArray kArraySize 1) )) 
	  (let (( loopTC (loopTimeConstruction kMinTreeDepth kMaxTreeDepth kStretchTreeDepth)))
	    (if (or (null? longLivedTree) (eq? #f (eq? (checkArray array 1) 1 )) )
	      '()
	      (cons 100 100)
	    )
	  )
	)
      )
    )
  )
)


(define (maketree-helper n k lefttree)
  (if (= n 0) (cons k (cons lefttree '(())))
      (let ((l (maketree-helper (- n 1) k lefttree)))
        (let ((r (maketree-helper (- n 1) (+ 1 (car l)) lefttree)))
          (cons (+ 1 (car r)) (cons l (cons r '())))))))

(define (maketree n)
  (let ((rt-tree (maketree-helper (- n 1) 1 '())))
        (let ((left-tree (maketree-helper (- n 1) (+ 1 (car rt-tree)) rt-tree)))
          (cons (+ 1 (car left-tree)) (cons left-tree rt-tree)))))


(define (zig t)
  (let ((v (main  16 10 5 4 10)))
    (if (or (null? t) (null? (car (cdr t)))) (cons (+ (car t) (car v)) '())
    (cons (+ (car t) (car v)) (zag (car (cdr t)))))))

(define (zag t)
  (let ((v (main  16 10 5 4 10)))
    (if (or (null? t) (null? (car (cdr (cdr t))))) (cons (+ (car t) (car v)) '())
    (cons (+ (car t) (car v)) (zig (car (cdr (cdr t))))))))

(define t (maketree 11))

(define z (zig t))