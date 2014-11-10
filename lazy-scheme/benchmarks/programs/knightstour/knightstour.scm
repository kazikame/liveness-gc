(define (or orx ory) (if (eq? orx #t) orx ory))

(define (and anx any) (if (eq? anx #t) any #f))

(define (append ax ay)
  (if (null? ax)
      ay
      (cons (car ax) (append (cdr ax) ay))))


(define (inResult x y result)
  (if (null? result)
      0
      (let ((posxy (car result)))
        (if (and (eq? x (car posxy)) (eq? y (cdr posxy)) )
            1
            (inResult x y (cdr result))
            )
        )
      )
  )

(define (check x y curlst boardsize)
  (if (or (> x boardsize) (< x 1) )
      '()
      (if (or (> y boardsize) (< y 1) )
          '()
          (if (eq? 1 (inResult x y curlst))
              '()
              (append curlst (cons (cons x y) '()))
              )
          )
      )
  )
  
          

            

(define (join ax ay)
  (if (null? ax)
      ay
      (if (null? ay)
      (cons ax '())
      (cons ax ay)
      )
      )
  )


(define (generateMoves posx posy)
  (let ((mv1 (cons (+ posx 1) (+ posy 2)) ))
    (let ((mv2 (cons (+ posx 1) (- posy 2)) ))
      (let ((mv3 (cons (+ posx 2) (+ posy 1)) ))
        (let ((mv4 (cons (+ posx 2) (- posy 1)) ))
          (let ((mv5 (cons (- posx 1) (+ posy 2)) ))
            (let ((mv6 (cons (- posx 1) (- posy 2)) ))
              (let ((mv7 (cons (- posx 2) (+ posy 1)) ))
                (let ((mv8 (cons (- posx 2) (- posy 1)) ))
                  (cons mv1 (cons mv2 (cons mv3 (cons mv4 (cons mv5 (cons mv6 (cons mv7 (cons mv8 '()))))))))
                  ))))))))
  )
  
(define (tries plst move bs)
  (check (car move) (cdr move) plst bs )
  )

(define (try plst movelst bs newlst)
  (if (null? movelst)
      newlst
      (let (( n (tries plst (car movelst) bs)))
        (let (( nlst(join n newlst)))
          (try plst (cdr movelst) bs nlst)
          ))
      )
  )   

(define (getLastMove plst)
  (if (null? (cdr plst))
      (car plst)
      (getLastMove (cdr plst))
      )
  )
  
(define (explore bs pathlst)
  (let ((lastmove (getLastMove pathlst) ))
    (let ((moves (generateMoves (car lastmove) (cdr lastmove)) ))
      (try pathlst moves bs '())
      )
    )
  )

(define (nextStep bs result)
  (if (null? result)
      '() 
      (append (explore bs (car result)) (nextStep bs (cdr result)))
      )
  )

(define (divide1 a b n op)
  (if (< a b)
      (if (eq? op 0)
          n
          a
          )
      (divide1 (- a b) b (+ 1 n) op)
      )
  )

(define (divide a b)
  (divide1 a b 0 0)
  )

(define (modr a b)
  (divide1 a b 0 1)
  )

(define (getLength lst n)
  (if(null? lst)
     n
     (getLength (cdr lst) (+ n 1))
     )
  )
(define (splitHalf lst len)
  (if (eq? 0 len)
      lst
      (splitHalf (cdr lst) (- len 1))
      )
  )
(define (filter lst)
  (let ((len (getLength lst 0)))
    (splitHalf lst (divide len 2))
    )
  )
(define (solve step sx sy bs)
  (if (eq? step 1)
      (cons (cons (cons sx sy) '()) '())
      (let ((ans (solve (- step 1) sx sy bs)))
        (if (eq? 0 (modr step 5))
            (nextStep bs (filter ans))
            (nextStep bs ans)
            )
        )
      )
  )

(define (knightTour boardsize startposx startposy)
  (solve (* boardsize boardsize) startposx startposy boardsize)
  )

(knightTour 5 3 3)  
