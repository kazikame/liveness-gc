;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; File:         deriv.sch
; Description:  The DERIV benchmark from the Gabriel tests.
; Author:       Vaughan Pratt
; Created:      8-Apr-85
; Modified:     10-Apr-85 14:53:50 (Bob Shaw)
;               23-Jul-87 (Will Clinger)
;               9-Feb-88 (Will Clinger)
; Language:     Scheme
; Status:       Public Domain
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
;;; DERIV -- Symbolic derivative benchmark written by Vaughn Pratt.
;;; It uses a simple subset of Lisp and does a lot of  CONSing.

; Returns the wrong answer for quotients.
; Fortunately these aren't used in the benchmark.
(define (equal? a b) (eq? a b))

(define (not x) (if x #f #t))

(define (myderiv a)
  (if (null? a)
      '()
      (cons (deriv (car a))
	    (myderiv (cdr a)))))

(define (myderiv-aux a)
  (if (null? a)
      '()
      (cons (deriv-aux (car a))
	    (myderiv-aux (cdr a)))))

(define (list1 x)
        (let ((_var13 '()))
             (let ((_var14 (cons x
                                 _var13)))
                  _var14)))

(define (list2 x y)
        (let ((_var15 (list1 y)))
             (let ((_var16 (cons x
                                 _var15)))
                  _var16)))

(define (list3 x y z)
        (let ((_var17 (list2 y
                             z)))
             (let ((_var18 (cons x
                                 _var17)))
                  _var18)))

(define (list4 x y z w)
        (let ((_var19 (list3 y
                             z
                             w)))
             (let ((_var20 (cons x
                                 _var19)))
                  _var20)))

(define (list5 x y z w v)
        (let ((_var29 (list4 y
                             z
                             w
			     v)))
             (let ((_var30 (cons x
                                 _var29)))
                  _var30)))


(define (deriv-aux a) (list3 "/" (deriv a) a))

(define (caddr a) (car (cdr (cdr a))))

(define (cadr a) (car (cdr a)))

 
(define (deriv a)
  (if (pair? a)
       (if (equal? (car a) "+")
	  (cons "+" (myderiv (cdr a)))
	  (if (equal? (car a) "-")
	      (cons "-" (myderiv (cdr a)))
	      (if (equal? (car a) "*")
		  (list3 "*" a (cons "+" (myderiv-aux (cdr a))))
		  (if (equal? (car a) "/")
		      (list3 "-" (list3 "/"
					(deriv (cadr a))
					(caddr a))
			     (list3 "/"
				    (cadr a)
				    (list3 "*"
					   (caddr a)
					   (caddr a)
					   (deriv (caddr a)))))
		      '()))))
(if (equal? a "x") 
	  1 
	  0)))

(deriv (list5 "+" (list4 "*" 3 "x" "x") (list4 "*" "a" "x" "x") (list3 "*" "b" "x") 5)) 
;(deriv (list3 "+" "x" "x"))
