(define ( and a b)
 (if a 
 (if b #t 
 #f ) 
 #f))

(define (divide1 a b n op ) 
(if (< a b ) 
(if (eq? op 0 ) 
n 
a 
) 
(divide1 (- a b ) b (+ 1 n ) op ) 
) 
) 

(define (modr a b ) 
(divide1 a b 0 1 ) 
) 

(define (append ax ay ) 
(if (null? ax ) 
ay 
(cons (car ax ) (append (cdr ax ) ay ) ) ) ) 

(define (insert x heap ) 
(let ((newnode (cons x (cons 0 () ) ) ) ) 
(if (null? heap ) 
(cons newnode () ) 
(cons newnode heap ) 
) 
) 
) 


(define (updateMin curmin heap ) 
(if (< (car (car curmin ) ) (car (car heap ) ) ) 
curmin 
heap 
) 
) 

(define (addChild root node ) 
(let ((ptchild (cdr (cdr (car root ) ) ) ) ) 
(append ptchild (cons (car node ) () ) ) ;to delete cdr link of node 
) 
) 

(define (updateArray nchild root array ) 
(if (null? array ) 
(cons (car root ) () ) ;to delete cdr link of root 
(let ((achild (car (cdr (car array ) ) ) ) ) 
(if (eq? nchild achild ) 
(if (< (car (car root ) ) (car (car array ) ) ) 
(let ((ptchild (addChild root array ) ) ) 
(let ((newnode (cons (cons (car (car root ) ) (cons (+ 1 nchild ) ptchild ) ) (cdr array ) ) ) ) 
newnode 
) 
) 
(let ((ptchild (addChild array root ) ) ) 
(let ((newnode (cons (cons (car (car array ) ) (cons (+ 1 achild ) ptchild ) ) (cdr array ) ) ) ) 
newnode 
) 
) 
) 
(cons (car array ) (updateArray nchild root (cdr array ) ) ) 
) 
) 
) 
) 

(define (getLen root ) 
(if (null? root ) 
0 
(+ 1 (getLen (cdr root ) ) ) 
) 
) 
(define (calculateM root min ) 
(if (null? root ) 
min 
(let ((m (car (car root ) ) ) ) 
(if (< m (car (car min ) ) ) 
(calculateM (cdr root ) root ) 
(calculateM (cdr root ) min ) 
) 
) 
) 
) 

(define (calculateMin root ) 
(calculateM (cdr root ) root ) 
) 

;this does the actuall merging process 
(define (merge2 root array ) 
(if (null? root ) 
array 
(let ((nchild (car (cdr (car root ) ) ) ) ) 
(let ((narray (updateArray nchild root array ) ) ) 
(merge2 (cdr root ) narray ) 
) 
) 
) 
) 

;this is equivalent to loop merge2 unitll there is no change 
(define (merge1 root curlen ) 
(let ((newroot (merge2 root () ) ) ) 
(let ((lenheap (getLen newroot ) ) ) 
(if (eq? lenheap curlen ) 
newroot 
(merge1 newroot lenheap ) 
) 
) ) 
) 

;Call supporting merge functions 
(define (merge root ) 
(let ((curlen (getLen root ) ) ) 
(merge1 root curlen ) 
) 
) 

(define (children_to_rootlist node n ) 
(if (eq? 0 n ) 
() 
(cons (car node ) (children_to_rootlist (cdr node ) (- n 1 ) ) ) 
) 
) 

;node and heap both are pointing to same root 
(define (deleteAndJoinChildren node heap ) 
(let ((nchildren (car (cdr (car node ) ) ) ) ) 
(if (eq? 0 nchildren ) 
(cdr heap ) 
(let ((chlst (children_to_rootlist (cdr (cdr (car node ) ) ) nchildren ) ) ) 
(append chlst (cdr heap ) ) 
) 
) 
) 
) 

;node and heap both are pointing to same root 
(define (delete curmin heap ) 
(if (null? heap ) 
() 
(if (eq? (car (car curmin ) ) (car (car heap ) ) ) 
(deleteAndJoinChildren curmin heap ) 
(cons (car heap ) (delete curmin (cdr heap ) ) ) 
) 
) 
) 

(define (deleteMin curmin heap ) 
(let ((step1 (delete curmin heap ) ) ) 
(let ((step2 (merge step1 ) ) ) 
; (let ((step3 (calculateMin step2 ) ) ) 
step2 
; ) 
) 
) 
) 

(define (testdelete i min heap ret ) 
(let ((nheap (deleteMin min heap ) ) ) 
	 (if (eq? 1 i ) 
		ret 
	 (let ((nmin (calculateMin nheap ) ) ) 
(testdelete (- i 1 ) nmin nheap (cons nmin ret ) ) 
) ) 
) 
) 

(define (testinsert i min heap ) 
(if (eq? 0 i ) 
(cons min heap ) 
(let ((nheap (insert (modr (* i 5 ) (+ i 5 ) ) heap ) ) ) 
(let ((nmin (updateMin min nheap ) ) ) 
(testinsert (- i 1 ) nmin nheap ) 
) 
) 
) 
) 

(define (test n ) 
(let ((ins (testinsert n (insert 100000 () ) () ) ) ) 

(testdelete n (car ins ) (cdr ins ) () ) 

) 
) 
(test 1000 ) 
