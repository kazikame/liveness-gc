#lang racket
(define (append ax ay)
  (if (null? ax)
      ay
      (cons (car ax) (append (cdr ax) ay))))

;with a partial solution 'vs', is the position 'vp' a valid extension?
(define (valid vs vp)
  (if (null? vs)
      #t
      (let ((vf (car vs))) ;first position in solution
        (if (eq? (car vf) (car vp)) ;same row?
            #f
            (if (eq? (cdr vf) (cdr vp)) ;same column?
                #f
                (let ((rd (- (car vf) (car vp)))) ;row difference
                  (let ((cd (- (cdr vf) (cdr vp)))) ;column difference
                    (if (eq? (- rd cd) 0) ;same diagonal (top left to bottom right)?
                        #f
                        (if (eq? (+ rd cd) 0) ;same diagonal (top right to bottom left) ?
                            #f
                            (valid (cdr vs) vp)))))))))) ;check against remaining positions in solution

;add all valid postions from 'atps' to the partial solution 'ats'
(define (addto ats atps)
  (if (null? atps)
      '()
      (if (valid ats (car atps))
          (append (cons (append ats (cons (car atps) '())) '()) (addto ats (cdr atps)))
          (addto ats (cdr atps)))))

;generate all posibble positions on row 'tk' of a 'tn' size board
(define (tries tn tk)
  (if (eq? tn 0)
      '()
      (append (tries (- tn 1) tk) (cons (cons tk tn) '()))))

;extend all solutions in 'ess' by adding a queen on row 'ek' for a 'en' size board
(define (extend ess en ek)
  (if (null? ess)
      '()
      (append (addto (car ess) (tries en ek))
              (extend (cdr ess) en ek))))

;base solution for 'bn' size board, which is always 'bn' number of solutions with a queen on each column on row 1
(define (base bn)
  (if (eq? bn 0)
      '()
      (append (base (- bn 1)) (cons (cons (cons 1 bn) '()) '()))))

;find all solutions of placing 'sk' queens on the first 'sk' rows of a 'sn' size board
(define (solve sn sk)
  (if (eq? sk 1)
      (base sn)
      (let ((ss (solve sn (- sk 1)) ))
         (extend ss sn sk))))

;place 'nqn' queens on a 'nqn' sized board
(define (nqueens nqn)
  (solve nqn nqn))

;main program expression
(nqueens 11)
