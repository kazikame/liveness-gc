(define (append ax ay)
        (let ((v1 (null? ax ) ))
                (if v1 
                       ay 
                       (let ((v4 (car ax ) ))
                               (let ((v0 (cdr ax ) ))
                                       (let ((v8 (append v0 ay ) ))
                                               (cons v4 v8 ) )))) ))

(define (valid vs vp)
        (let ((v12 (null? vs ) ))
                (if v12 
                       #t 
                       (let ((vf (car vs ) ))
                               (let ((v393 (car vf ) ))
                                       (let ((v418 (car vp ) ))
                                               (let ((v371 (eq? v393 v418 ) ))
                                                       (if v371 
                                                              #f 
                                                              (let ((v323 (cdr vf ) ))
                                                                      (let ((v344 (cdr vp ) ))
                                                                              (let ((v305 (eq? v323 v344 ) ))
                                                                                      (if v305 
                                                                                             #f 
                                                                                             (let ((v265 (car vf ) ))
                                                                                                     (let ((v282 (car vp ) ))
                                                                                                             (let ((rd (- v265 v282 ) ))
                                                                                                                     (let ((v230 (cdr vf ) ))
                                                                                                                             (let ((v244 (cdr vp ) ))
                                                                                                                                     (let ((cd (- v230 v244 ) ))
                                                                                                                                             (let ((v212 (- rd cd ) ))
                                                                                                                                                     (let ((v203 (eq? v212 0 ) ))
                                                                                                                                                             (if v203 
                                                                                                                                                                    #f 
                                                                                                                                                                    (let ((v192 (+ rd cd ) ))
                                                                                                                                                                            (let ((v186 (eq? v192 0 ) ))
                                                                                                                                                                                    (if v186 
                                                                                                                                                                                           #f 
                                                                                                                                                                                           (let ((v51 (cdr vs ) ))
                                                                                                                                                                                                   (valid v51 vp ) )) ))) ))))))))) )))) ))))) ))

(define (addto ats atps)
        (let ((v63 (null? atps ) ))
                (if v63 
                        () 
                       (let ((v555 (car atps ) ))
                               (let ((v543 (valid ats v555 ) ))
                                       (if v543 
                                              (let ((v515 (car atps ) ))
                                                      (let ((v525 (cons v515  () ) ))
                                                              (let ((v507 (append ats v525 ) ))
                                                                      (let ((v76 (cons v507  () ) ))
                                                                              (let ((v497 (cdr atps ) ))
                                                                                      (let ((v80 (addto ats v497 ) ))
                                                                                              (append v76 v80 ) ))))))
                                              (let ((v84 (cdr atps ) ))
                                                      (addto ats v84 ) )) ))) ))

(define (tries tn tk)
        (let ((v90 (eq? tn 0 ) ))
                (if v90 
                        () 
                       (let ((v607 (- tn 1 ) ))
                               (let ((v96 (tries v607 tk ) ))
                                       (let ((v597 (cons tk tn ) ))
                                               (let ((v101 (cons v597  () ) ))
                                                       (append v96 v101 ) ))))) ))

(define (extend ess en ek)
        (let ((v105 (null? ess ) ))
                (if v105 
                        () 
                       (let ((v653 (car ess ) ))
                               (let ((v662 (tries en ek ) ))
                                       (let ((v112 (addto v653 v662 ) ))
                                               (let ((v643 (cdr ess ) ))
                                                       (let ((v117 (extend v643 en ek ) ))
                                                               (append v112 v117 ) )))))) ))

(define (base bn)
        (let ((v122 (eq? bn 0 ) ))
                (if v122 
                        () 
                       (let ((v715 (- bn 1 ) ))
                               (let ((v127 (base v715 ) ))
                                       (let ((v703 (cons 1 bn ) ))
                                               (let ((v697 (cons v703  () ) ))
                                                       (let ((v134 (cons v697  () ) ))
                                                               (append v127 v134 ) )))))) ))

(define (solve sn sk)
        (let ((v139 (eq? sk 1 ) ))
                (if v139 
                       (base sn ) 
                       (let ((v739 (- sk 1 ) ))
                               (let ((ss (solve sn v739 ) ))
                                       (extend ss sn sk ) ))) ))

(define (nqueens nqn)
        (solve nqn nqn ) )

(nqueens 11 ) 

