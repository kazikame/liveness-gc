;key=int
;entity=(cons int (cons int (cons int ())))
;join=(cons int (cons int (cons int (cons int (cons int ())))))
;Tree e=(cons (cons "node" key) (cons Tree(e) (cons Tree(e) ())))|(cons (cons "leaf" key) (cons e ())
;             |(cons "empty ())
(define (or orx ory) (if (eq? orx #t) orx ory))

(define (makeNode k t1 t2)
  (cons (cons "node" k) (cons t1 (cons t2 ())))
  )

(define (makeLeaf k e)
  (cons (cons "leaf" k) (cons e ()))
  )

;Interesting entities are 3-tuples of integers.
;The joins of two such entities are 5-tuples of integers.
;insertT :: Key -> entity -> Tree entity -> Tree entity
(define (insertT k e t)
  (if (eq? (car t) "empty") ;; how can both (car t) and (car (car t)) be of the same type?
      (makeLeaf k e)
      (if (eq? (car (car t)) "node")
          (insertTNode k e t)
          (insertTLeaf k e t) ;leaf
          ))
  )

;insertT :: Key -> entity -> Tree entity -> Tree entity
(define (insertTLeaf k e t)
  (let ((key (cdr(car t)) ))
    (if (< k key)
        (makeNode k (makeLeaf k e) t)
        (if (> k key)
            (makeNode key t (makeLeaf k e))
            (cons 99 99) ;error
            )))
  )
            
;insertT :: Key -> entity -> Tree entity -> Tree entity
(define (insertTNode k e t)
  (let ((key (cdr(car t)) ))
    (let ((tree1 (car(cdr t)) ))
      (let ((tree2 (car(cdr(cdr t))) ))
        (if (> k key)
            (makeNode key tree1 (insertT k e tree2))
            (makeNode key (insertT k e tree1) tree2)
            ))))
  )

;"lookupT" looks up the record (entity) whose key is specified,
;in the tree argument. It returns Just e if the key value
;exists, or Nothing otherwise.

;lookupT :: Key -> Tree entity -> Maybe entity
(define (lookupT k t)
  (if (eq? (car(car t)) "empty")
      ()
      (if (eq? (car(car t)) "node")
          (lookupTNode k t)
          (lookupTLeaf k t)
          ))
  )

(define (lookupTLeaf k t)
  (let ((key (cdr(car t)) ))
    (let ((e (car(cdr t)) ))
      (if (eq? k key)
          e
          ()
          )))
  )

(define (lookupTNode k t)
  (let ((key (cdr(car t)) ))
    (let ((tree1 (car(cdr t)) ))
      (let ((tree2 (car(cdr(cdr t))) ))
        (if (> k key)
            (lookupT k tree2)
            (lookupT k tree1)
            ))))
  )

;"forceTree" forces a tree to normal form.
;forceTree :: Tree Join -> ()
(define (forceTree t)
  ()
  )

; "readTree" reads a tree of entities from a string. Each entity is
;represented by 3 space-separated positive integers.
;The keys are derived from the entities read in using the function "fk".
;readTree :: (Entity->Key) -> String -> Tree Entity -> Tree Entity
(define (readTree n t)
  (if (eq? 0 n)
      t
      (let ((en (makeEntity n) ))
        (let ((e (cons (car en) (cons (car(cdr en)) (cons (car(cdr(cdr en))) ()))) ))
          (let ((k (makeKey e) ))
            (readTree (- n 3) (insertT k e t))
            ))))
  )

;readInt of haskell has been changed to makeEntity.
(define (makeEntity n)
  (let ((f (- n 2) ))
    (let ((g (- n 1) ))
      (let ((h n ))
        (cons f (cons g (cons h ())))
        )))
  )

(define (makeKey e)
  (car e)
  )

; "join" joins two trees of "Entities" (3-tuples) to produce a
;tree of "Joins" (5-tuples). The relations are joined on the
;third component of each record.
;join :: Tree Entity -> Tree Entity -> Tree Join -> Tree Join
(define (join t1 t2 tj)
  (if (or (eq? (car t1) "empty") (eq? (car t2) "empty"))
      tj
      (if (eq? (car(car t1)) "node")
          (let ((tree1 (car(cdr t1)) ))
            (let ((tree2 (car(cdr(cdr t1))) ))
              (join tree1 t2 (join tree2 t2 tj))
              ))
          ;Leaf case
          (let ((key (cdr(car t1)) ))
            (let ((e (car(cdr t1)) ))
              (let ((a (car e) ))
                (let ((b (car(cdr e)) ))
                  (let ((c (car(cdr(cdr e))) ))
                    (let ((entity (lookupT a t2) ))
                      (if (null? entity)
                          tj
                          (let ((d (car entity) ))
                            (let ((f (car(cdr entity)) ))
                              (let ((newe (cons a (cons b (cons c (cons d (cons f ()))))) ))
                                (insertT c newe tj)
                                ))))
                      )))))))
)
  )

; n should be multiple of 3
(define (main)
  (if (eq? 0 0)
      (let ((a (readTree 516 (cons "empty" ())) ))
        (let ((b (readTree 1530 (cons "empty" ())) ))
          (join a b (cons "empty" ()));(forceTree (join a b (cons "empty" ())))
          ))
      (cons 9 9);n is wrong
      )
  )

(main)
