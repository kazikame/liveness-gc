#!/usr/bin/mzscheme -qu
;; The Computer Language Benchmarks Game
;; http://shootout.alioth.debian.org/
;;
;; Imperative-style implementation based on the SBCL implementation by
;; Patrick Frankenberger and Juho Snellman, but using only native Scheme
;; idioms like 'named let' and 'do' special form.
;;
;; Contributed by Anthony Borla, then converted for mzscheme
;; by Matthew Flatt and Brent Fulgham

#|
Correct output N = 1000 is

-0.169075164
-0.169087605
|#

#lang scheme/base
(require scheme/cmdline)

;; ------------------------------
;; define planetary masses, initial positions & velocity

(define +pi+ 3.141592653589793)
(define +days-per-year+ 365.24)

(define +solar-mass+ (* 4 +pi+ +pi+))

(define +dt+ 0.01)

(define-struct body (x y z vx vy vz mass)
  #:mutable)

(define *sun*
  (make-body 0.0 0.0 0.0 0.0 0.0 0.0 +solar-mass+))

(define *jupiter*
  (make-body 4.84143144246472090
             -1.16032004402742839
             -1.03622044471123109e-1
             (* 1.66007664274403694e-3 +days-per-year+)
             (* 7.69901118419740425e-3 +days-per-year+)
             (* -6.90460016972063023e-5 +days-per-year+)
             (* 9.54791938424326609e-4 +solar-mass+)))

(define *saturn*
  (make-body 8.34336671824457987
             4.12479856412430479
             -4.03523417114321381e-1
             (* -2.76742510726862411e-3 +days-per-year+)
             (* 4.99852801234917238e-3 +days-per-year+)
             (* 2.30417297573763929e-5 +days-per-year+)
             (* 2.85885980666130812e-4 +solar-mass+)))

(define *uranus*
  (make-body 1.28943695621391310e1
             -1.51111514016986312e1
             -2.23307578892655734e-1
             (* 2.96460137564761618e-03 +days-per-year+)
             (* 2.37847173959480950e-03 +days-per-year+)
             (* -2.96589568540237556e-05 +days-per-year+)
             (*  4.36624404335156298e-05 +solar-mass+)))

(define *neptune*
  (make-body 1.53796971148509165e+01
             -2.59193146099879641e+01
             1.79258772950371181e-01
             (* 2.68067772490389322e-03 +days-per-year+)
             (* 1.62824170038242295e-03 +days-per-year+)
             (* -9.51592254519715870e-05 +days-per-year+)
             (* 5.15138902046611451e-05 +solar-mass+)))

(define *system* (list *sun* *jupiter* *saturn* *uranus* *neptune*))

;; -------------------------------
(define (offset-momentum)
  (let loop-i ([i *system*] [px 0.0] [py 0.0] [pz 0.0])
    (if (null? i)
      (begin
        (set-body-vx! (car *system*) (/ (- px) +solar-mass+))
        (set-body-vy! (car *system*) (/ (- py) +solar-mass+))
        (set-body-vz! (car *system*) (/ (- pz) +solar-mass+)))
      (let ([i1 (car i)])
        (loop-i (cdr i)
                (+ px (* (body-vx i1) (body-mass i1)))
                (+ py (* (body-vy i1) (body-mass i1)))
                (+ pz (* (body-vz i1) (body-mass i1))))))))

;; -------------------------------
(define (energy)
  (let loop-o ([o *system*] [e 0.0])
    (if (null? o)
      e
      (let* ([o1 (car o)]
             [e (+ e (* 0.5 
                        (body-mass o1)
                        (+ (* (body-vx o1) (body-vx o1))
                           (* (body-vy o1) (body-vy o1))
                           (* (body-vz o1) (body-vz o1)))))])
        (let loop-i ([i (cdr o)] [e e])
          (if (null? i)
            (loop-o (cdr o) e)
            (let* ([i1   (car i)]
                   [dx   (- (body-x o1) (body-x i1))]
                   [dy   (- (body-y o1) (body-y i1))]
                   [dz   (- (body-z o1) (body-z i1))]
                   [dist (sqrt (+ (* dx dx) (* dy dy) (* dz dz)))]
                   [e    (- e (/ (* (body-mass o1) (body-mass i1)) dist))])
              (loop-i (cdr i) e))))))))

;; -------------------------------
(define (advance)
  (let loop-o ([o *system*])
    (when (pair? o)
      (let* ([o1  (car o)]
             [o1x (body-x o1)]
             [o1y (body-y o1)]
             [o1z (body-z o1)]
             [om  (body-mass o1)])
        (let loop-i ([i  (cdr o)]
                     [vx (body-vx o1)]
                     [vy (body-vy o1)]
                     [vz (body-vz o1)])
          (if (pair? i)
            (let* ([i1    (car i)]
                   [dx    (- o1x (body-x i1))]
                   [dy    (- o1y (body-y i1))]
                   [dz    (- o1z (body-z i1))]
                   [dist2 (+ (* dx dx) (* dy dy) (* dz dz))]
                   [mag   (/ +dt+ (* dist2 (sqrt dist2)))]
                   [dxmag (* dx mag)]
                   [dymag (* dy mag)]
                   [dzmag (* dz mag)]
                   [im    (body-mass i1)])
              (set-body-vx! i1 (+ (body-vx i1) (* dxmag om)))
              (set-body-vy! i1 (+ (body-vy i1) (* dymag om)))
              (set-body-vz! i1 (+ (body-vz i1) (* dzmag om)))
              (loop-i (cdr i)
                      (- vx (* dxmag im))
                      (- vy (* dymag im))
                      (- vz (* dzmag im))))
            (begin (set-body-vx! o1 vx)
                   (set-body-vy! o1 vy)
                   (set-body-vz! o1 vz)
                   (set-body-x! o1 (+ o1x (* +dt+ vx)))
                   (set-body-y! o1 (+ o1y (* +dt+ vy)))
                   (set-body-z! o1 (+ o1z (* +dt+ vz)))))))
      (loop-o (cdr o)))))

;; -------------------------------

(let ([n (command-line #:args (n) (string->number n))])
  (offset-momentum)
  (printf "~a\n" (real->decimal-string (energy) 9))
  (for ([i (in-range n)]) (advance))
  (printf "~a\n" (real->decimal-string (energy) 9)))
