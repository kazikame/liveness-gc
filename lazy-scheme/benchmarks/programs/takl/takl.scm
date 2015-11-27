#lang racket
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; File:         takl.sch
; Description:  TAKL benchmark from the Gabriel tests
; Author:       Richard Gabriel
; Created:      12-Apr-85
; Modified:     12-Apr-85 10:07:00 (Bob Shaw)
;               22-Jul-87 (Will Clinger)
; Language:     Scheme
; Status:       Public Domain
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
;;; TAKL -- The TAKeuchi function using lists as counters.

(define (not x) 
  (if x #f #t))

(define (listn n)
  (if (not (= 0 n))
      (cons n (listn (- n 1)))
      '()))
 
(define l18l (listn 18))
(define l12l (listn 12))
(define  l6l (listn 2))
 
(define (mas x y z)
  (if (not (shorterp y x))
      z
      (mas (mas (cdr x)
                 y z)
            (mas (cdr y)
                 z x)
            (mas (cdr z)
                 x y))))
 
(define (shorterp x y)
  (if (null? y)
      #f
      (if (null? x)
	  #t
	  (shorterp (cdr x)
		    (cdr y)))))
 

(mas (cons 18 '()) (cons 12 '()) (cons 6 '()))
