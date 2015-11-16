(define nil '())

(define (list1 x)
  (cons x nil))

(define (list2 x y)
  (cons x (list1 y)))

(define (list3 x y z)
  (cons x (list2 y z)))

(define (list4 x y z w)
  (cons x (list3 y z w)))

(define (make-leaf symbol weight)
  (list3 'leaf symbol weight))

(define (leaf? object)
  (eq? (car object) 'leaf))

(define (symbol-leaf x) (car (cdr x)))

(define (weight-leaf x) (car (cdr (cdr x))))

(define (make-code-tree left right)
  (list4 left
	 right
	 (append (symbols left) (symbols right))
	 (+ (weight left) (weight right))))

(define (left-branch tree) (car tree))

(define (right-branch tree) (car (cdr tree)))

(define (symbols tree)
  (if (leaf? tree)
      (list1 (symbol-leaf tree))
      (car (cdr (cdr tree)))))

(define (weight tree)
  (if (leaf? tree)
      (weight-leaf tree)
      (car (cdr (cdr (cdr tree))))))

(define (decode-1 bits current-branch tree)
  (if (null? bits)
      nil
      (let ((next-branch
	     (choose-branch (car bits) current-branch)))
	(if (leaf? next-branch)
	    (cons (symbol-leaf next-branch)
		  (decode-1 (cdr bits) tree tree))
	    (decode-1 (cdr bits) next-branch tree)))))

(define (decode bits tree)
  (decode-1 bits tree tree))

(define (choose-branch bit branch)
  (if (= bit 0) 
      (left-branch branch)
      (right-branch branch)))

(define (adjoin-set x set)
  (if (null? set)
      (list1 x)
      (if (< (weight x) (weight (car set)))
	  (cons x set)
	  (cons (car set) (adjoin-set x (cdr set))))))

(define (make-leaf-set pairs)
  (if (null? pairs)
      nil
      (let ((pair (car pairs)))
        (adjoin-set (make-leaf (car pair)         ; symbol
                               (car (cdr pair)))  ; frequency
                    (make-leaf-set (cdr pairs))))))

(define (encode message tree)
  (if (null? message)
      nil
      (append (encode-symbol (car message) tree)
              (encode (cdr message) tree))))

(define (encode-symbol symbol tree) 
  (if (leaf? tree)
      nil
      (if (member symbol (symbols (left-branch tree)))
	  (cons 0 (encode-symbol symbol (left-branch tree)))	
	  (cons 1 (encode-symbol symbol (right-branch tree))))))

(define (generate-huffman-tree pairs)
  (successive-merge (make-leaf-set pairs)))

(define (successive-merge leafs)
  (if (null? (cdr leafs))
      (car leafs) ; only single leaf => tree in itself
      (successive-merge (adjoin-set 
			 (make-code-tree (car leafs)
					 (car (cdr leafs)))
			 (cdr (cdr leafs))))))


;; examples ....
(define sample-tree
  (make-code-tree (make-leaf 'A 4)
                  (make-code-tree
                   (make-leaf 'B 2)
                   (make-code-tree (make-leaf 'D 1)
                                   (make-leaf 'C 1)))))


(define sample-message '(0 1 1 0 0 1 0 1 0 1 1 1 0))

(define encoded-messgae (encode '(a d a b b c a) sample-tree))

(define enc-dec (decode (encode '(A D A B B C A) sample-tree) sample-tree))

(define (print-tree tree) tree)

(define complex-tree (print-tree 
		      (generate-huffman-tree '((a 8 ) (b 3) (c 1) (d 1) (e 1) (f 1) (g 1) (h 1)))))

(define rock-50s-tree 
  (generate-huffman-tree
   '((a 2) (boom 1) (get 2) (job 2)
     (na 16) (sha 3) (yip 9) (wah 1))))

(define rock-enc
  (encode 
   '(get a job sha na na na na na na na na
     get a job sha na na na na na na na na
     wah yip yip yip yip yip yip yip yip yip
     sha boom)
   rock-50s-tree))

